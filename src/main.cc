#include <iostream>
#include "glm.hh"
#include "gl.hh"
#include "glew.hh"
#include "glfw.hh"
#include "types.hh"
#include "files.hh"
#include "ui.hh"
#include <vector>
#include <unordered_map>
#include <sstream>
#include <tuple>
#include <functional>
#include <future>
#include <atomic>


void status(std::ostream& out, const string& name, bool success)
{
	out 
		<< name
		<< ": "
		<< (success ? "\033[32m" : "\033[91m")
		<< (success ? "ok" : "x")
		<< "\033[0m"
		<< "\n";
}

class Shaders
{
private:
	std::atomic<uint> pendingTaskCount = 0;
	unordered_map<string, std::future<Files::ReadStatus>> pendingTasks;
	bool done = true;

public:
	void load(const string &path);
	bool isLoading() const;
	void onFinish(const string &path, const Files::OnRead &callback);
	void onFinish(const std::function<void()> &callback);
	void updateLoadProgress();
};

void Shaders::load(const string &path)
{
	done = false;
	if (pendingTasks.count(path) == 0)
	{
		++pendingTaskCount;
		pendingTasks.emplace(path,
				std::async(std::launch::async, [this](const string &path) {
					--pendingTaskCount;
					return Files::readText(string(SHADERS_PATH) + path + string(".glsl"));
				}, path)
			);
	}

}

bool Shaders::isLoading() const
{
	return !done;
}

void Shaders::updateLoadProgress()
{
	done = (pendingTaskCount == 0);
}

void Shaders::onFinish(const string &path, const Files::OnRead &callback)
{
	if (done)
		callback(pendingTasks[path].get());
}

void Shaders::onFinish(const std::function<void()> &callback)
{
	if (done)
	{
		pendingTasks.clear();
		callback();
	}
}

class Programs
{
private:
	GL gl;
	unordered_map<string, uint> shaders;
	unordered_map<string, uint> programs;
	unordered_map<string, vector<uint>> attachedShaders;

public:
	Shaders loader;

	uint create(const string &program)
	{
		auto id = gl.program.create();
		programs[program] = id;

		return id;
	}

	uint use(const string& program)
	{
		auto found = programs.find(program);
		if (found != programs.end())
		{
			auto id = found->second;
			gl.program.use(id);
			return id;
		}

		return 0;
	}

	uint addShader(uint type, const string &key, const string &source)
	{
		auto id = gl.shader.create(type);
		gl.shader.source(id, source);
		gl.shader.compile(id);
		shaders[key] = id;

		return id;
	}
	auto addVertexShader(const string &key, const string &source) { return addShader(GL_VERTEX_SHADER, key, source); }
	auto addFragmentShader(const string &key, const string &source) { return addShader(GL_FRAGMENT_SHADER, key, source); }
	string compileLog(const string &shader)
	{
		auto id = shaders[shader];
		auto type = gl.shader.getInt(id, GL_SHADER_TYPE);
		auto maxLen = gl.shader.getInt(id, GL_INFO_LOG_LENGTH);

		string kind = "???";
		if (gl.shader.Types.find(type) != std::end(gl.shader.Types))
			kind = gl.shader.Types.at(type);

		std::stringstream ss;

		if (!gl.shader.getInt(id, GL_COMPILE_STATUS))
		{
			ss << kind << ":\n";
			ss << gl.shader.infoLog(id, maxLen) << "\n";
		}
		else
			ss << kind << ": ok\n";

		return ss.str();
	}


	bool link(const string& program)
	{
		auto id = programs[program];
		gl.program.link(id);
		use(program);
		bool success = gl.program.getInt(id, GL_LINK_STATUS);

		if (success)
		{
			// mark for deletion
			for (const auto& id : attachedShaders[program])
				gl.shader.detach(programs[program], id);

			for (const auto& id : attachedShaders[program])
				gl.shader.del(id);
		}
		gl.program.use(0);

		return success;
	}

	void del(const string &program)
	{
		if (exists(program))
		{
			gl.program.del(programs[program]);
			attachedShaders.erase(program);
		}
	}

	void attachShader(const string &program, const string &shader)
	{
		auto shaderID = shaders[shader];
		gl.shader.attach(programs[program], shaderID);
		attachedShaders[program].push_back(shaderID);
	}

	bool exists(const string &program)
	{
		auto found = programs.find(program);
		return found != programs.end();
	}
};

struct Resources
{
	Programs programs;
};

int main(int argc, char *argv[])
{
	GLEW glew;
	GLFW glfw;
	GL gl;
	UI ui;

	if (!glfw.init())
	{
		std::cerr << "Failed to init GLFW";
		return 1;
	}

	glfw.window.hint430();
	glfw.window.hintResizable(true);

	auto window = glfw.window.create(1280, 1024, "GL");
	if (!window)
	{
		glfw.terminate();
		return 1;
	}
	glfw.window.makeContextCurrent(window);

	if (!glew.init())
	{
		std::cerr << "Failed to init GLEW";
		glfw.terminate();
		return 1;
	}
#ifdef _DEBUG
	glfw.window.moveToHalfRight(window);
#endif

	gl_printInfo();
	gl_bindDebugCallback();

	auto ctx = ui.init(window);

	Resources res;

	vector<v3> vertices = { v3(-1, 0, 0), v3(1, 0, 0), v3(1, 1, 0), v3(-1, 1, 0) };
	vector<v3> colors = { v3(1,0,0), v3(0,1,0), v3(0,0,1), v3(1,0,1) };
	vector<u32> indexes = { 0, 1, 2, 2, 3, 0 };

	uint vao = gl.vertexArray.create();
	gl.vertexArray.bind(vao);

	uint vbo = gl.buffer.gen();
	gl.buffer.array.bind(vbo);
	gl.buffer.array.data(vertices);

	uint cbo = gl.buffer.gen();
	gl.buffer.array.bind(cbo);
	gl.buffer.array.data(colors);

	uint ibo = gl.buffer.gen();
	gl.buffer.element.bind(ibo);
	gl.buffer.element.data(indexes);
		
	gl.vertexAttrib.enableArray(0);
	gl.buffer.array.bind(vbo);
	gl.vertexAttrib.pointerFloat(0, 3);

	gl.vertexAttrib.enableArray(1);
	gl.buffer.array.bind(cbo);
	gl.vertexAttrib.pointerFloat(1, 3);

	gl.buffer.element.bind(0);
	gl.buffer.array.bind(0);
	gl.vertexArray.bind(0);

	gl.cullFace.enable();
	gl.cullFace.back();

	res.programs.loader.load("pass/vert");
	res.programs.loader.load("pass/frag");

	bool reloading = false;

	while (!glfw.window.shouldClose(window))
	{
		if (res.programs.loader.isLoading())
		{
			res.programs.loader.updateLoadProgress();

			res.programs.loader.onFinish("pass/vert", [&res, &gl](const Files::ReadStatus& stat) {
				res.programs.addVertexShader("pass/vert", Files::text(stat));
			});
			res.programs.loader.onFinish("pass/frag", [&res, &gl](const Files::ReadStatus& stat) {
				res.programs.addFragmentShader("pass/frag", Files::text(stat));
			});

			res.programs.loader.onFinish([&res, &gl]() {
				res.programs.create("pass");
				res.programs.attachShader("pass", "pass/vert");
				res.programs.attachShader("pass", "pass/frag");

				if (!res.programs.link("pass"))
				{
					status(std::cerr, "program/pass", false);
					std::cerr << "-- Vert --\n" << res.programs.compileLog("pass/vert");
					std::cerr << "-- Frag --\n" << res.programs.compileLog("pass/frag");
				}
				else
					status(std::cout, "program/pass", true);
			});
		}
		ui.beginFrame();
		ui.endFrame();

		gl.viewport.set(glfw.window.framebufferSize(window));
		gl.clear.depthBuffer();
		gl.clear.colorBuffer(v3(0.3f, 0.3f, 0.3f));
		
		//
		gl.vertexArray.bind(vao);
		gl.buffer.element.bind(ibo);
		res.programs.use("pass");
		gl.draw.triangles.elements(indexes.size());
		//
		ui.draw();
		glfw.window.swapBuffers(window);
		glfw.pollEvents();

		if (glfw.window.keyPress(window, GLFW_KEY_ESCAPE))
			glfw.window.shouldClose(window, true);

		if (glfw.window.keyPress(window, GLFW_KEY_F5) && !reloading)
		{
			reloading = true;
			gl.program.use(0);
			res.programs.del("pass");
			res.programs.loader.load("pass/vert");
			res.programs.loader.load("pass/frag");
		}
		if (glfw.window.keyRelease(window, GLFW_KEY_F5))
			reloading = false;
	}

	gl.vertexArray.bind(vao);
	gl.vertexAttrib.disableArray(1);
	gl.vertexAttrib.disableArray(0);
	gl.vertexArray.bind(0);
	gl.vertexArray.del(vao);

	gl.buffer.element.bind(0);
	gl.buffer.del(ibo);
	gl.buffer.array.bind(0);
	gl.buffer.del(vbo);
	gl.buffer.del(cbo);

	gl.program.use(0);
	res.programs.del("pass");

	ui.terminate();

	glfw.window.destroy(window);
	glfw.terminate();

    return 0;
}
