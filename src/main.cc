#include <iostream>
#include "glm.hh"
#include "gl.hh"
#include "glew.hh"
#include "glfw.hh"
#include "types.hh"
#include "files.hh"
#include <vector>
#include <unordered_map>
#include <sstream>
#include <tuple>
#include <functional>
#include <future>
#include <atomic>



static void gl_printInfo()
{
	GL gl;

	// print general info
	std::cout << "GL_VERSION: " << gl.get.str(GL_VERSION) << "\n";
	std::cout << "GL_SHADING_LANGUAGE_VERSION: " << gl.get.str(GL_SHADING_LANGUAGE_VERSION) << "\n";

	/*
	GLint maxI;
	Dimension dim;

	glGetIntegerv(GL_MAX_VIEWPORTS, &maxI);
	Log::i("OpenGL", String("GL_MAX_VIEWPORTS = ").append(maxI));

	glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, &maxI);
	Log::i("OpenGL", String("GL_MAX_DEPTH_TEXTURE_SAMPLES = ").append(maxI));

	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &dim.width);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &dim.height);
	Log::i("OpenGL", String("GL_MAX_FRAMEBUFFER_WIDTH x GL_MAX_FRAMEBUFFER_HEIGHT = ").append(dim));

	glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, &maxI);
	Log::i("OpenGL", String("GL_MAX_FRAMEBUFFER_LAYERS = ").append(maxI));

	glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, &maxI);
	Log::i("OpenGL", String("GL_MAX_FRAMEBUFFER_SAMPLES = ").append(maxI));

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxI);
	Log::i("OpenGL", String("GL_MAX_TEXTURE_SIZE = ").append(maxI));

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxI);
	Log::i("OpenGL", String("GL_MAX_TEXTURE_IMAGE_UNITS = ").append(maxI));

	glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxI);
	Log::i("OpenGL", String("GL_MAX_TEXTURE_BUFFER_SIZE = ").append(maxI));

	glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &maxI);
	Log::i("OpenGL", String("GL_MAX_RECTANGLE_TEXTURE_SIZE = ").append(maxI));

	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxI);
	Log::i("OpenGL", String("GL_MAX_RENDERBUFFER_SIZE = ").append(maxI));

	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &maxI);
	Log::i("OpenGL", String("GL_MAX_CUBE_MAP_TEXTURE_SIZE = ").append(maxI));

	if (GLEW_EXT_texture_filter_anisotropic)
	{
		GLfloat maxF;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxF);
		Log::i("OpenGL", String("GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT = ").append(maxI));
	}
	else
	{
		Log::i("OpenGL", String("GLEW_EXT_texture_filter_anisotropic = not supported"));
	}

	*/
}

static const char* gl_debugSourceAttr(GLenum source)
{
	switch (source)
	{
	case GL_DEBUG_SOURCE_API_ARB: return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: return "WINDOW_SYSTEM";
	case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: return "SHADER_COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: return "THIRD_PARTY";
	case GL_DEBUG_SOURCE_APPLICATION_ARB: return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER_ARB: return "OTHER";
	default: return "Unknown source";
	}
}

static const char* gl_debugTypeAttr(GLenum type)
{
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR_ARB: return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: return "DEPRECATED_BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: return "UNDEFINED_BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY_ARB: return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE_ARB: return "PERFORMANCE";
	case GL_DEBUG_TYPE_OTHER_ARB: return "OTHER";
	default: return "Unknown type";
	}
}

static const char* gl_debugServerityAttr(GLenum severity)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH_ARB: return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM_ARB: return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW_ARB: return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFY";
	default: return "Unknown severity";
	}
}

static void GLAPIENTRY gl_debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		std::cerr << gl_debugSourceAttr(source) << ", " <<
			 gl_debugTypeAttr(type) << ", " << 
			gl_debugServerityAttr(severity) << ", " 
			<< message;
	}
	else
	{
		std::cout << gl_debugSourceAttr(source) << ", " <<
			gl_debugTypeAttr(type) << ", " << gl_debugServerityAttr(severity) << ", "
			<< message;
	}
}

static void gl_bindDebugCallback()
{
	if (GLEW_ARB_debug_output)
		glDebugMessageCallbackARB(&gl_debugCallback, NULL);
	else
		std::cerr << "Failed to enable ARB_debug_output";
}

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
	GL gl;
	unordered_map<string, uint> shaders;
	std::atomic<uint> pendingTaskCount = 0;
	unordered_map<string, std::future<Files::ReadStatus>> pendingTasks;
	bool done = false;

public:
	void load(const string &path);
	bool isLoading() const;
	void onFinish(const string &path, const Files::OnRead &callback);
	void onFinish(const std::function<void()> &callback);
	void updateLoadProgress();

	uint add(uint shaderType, const string &key, const string &source);
	auto addVertexShader(const string &key, const string &source) { return add(GL_VERTEX_SHADER, key, source); }
	auto addFragmentShader(const string &key, const string &source) { return add(GL_FRAGMENT_SHADER, key, source); }
	uint id(const string &key);

	string compileLog(uint id);
};

void Shaders::load(const string &path)
{
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

uint Shaders::add(uint type, const string &key, const string &source)
{
	auto id = gl.shader.create(type);
	gl.shader.source(id, source);
	gl.shader.compile(id);
	shaders[key] = id;

	return id;
}

string Shaders::compileLog(uint id)
{
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

uint Shaders::id(const string &key)
{
	return shaders.at(key);
}

class Programs
{
private:
	GL gl;
	unordered_map<string, uint> programs;

public:
	uint create(const string &key)
	{
		auto id = gl.program.create();
		programs[key] = id;

		return id;
	}

	uint id(const string& key) const
	{
		return programs.at(key);
	}

	uint use(const string& key)
	{
		auto id = programs[key];
		gl.program.use(id);
		return id;
	}

	bool link(const string& key)
	{
		auto id = programs[key];
		gl.program.link(id);
		return gl.program.getInt(id, GL_LINK_STATUS);
	}
};

struct Resources
{
	Programs programs;
	Shaders shaders;
};

int main(int argc, char *argv[])
{
	GLEW glew;
	GLFW glfw;
	GL gl;

	if (!glfw.init())
	{
		std::cerr << "Failed to init GLFW";
		return 1;
	}

	glfw.window.hintContextVersion(4, 3);
	glfw.window.hintCoreProfileForwardCompat();
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

	gl_printInfo();
	gl_bindDebugCallback();

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

	while (!glfw.window.shouldClose(window))
	{
		if (res.shaders.isLoading())
		{
			res.shaders.load("pass/vert");
			res.shaders.load("pass/frag");

			res.shaders.updateLoadProgress();

			res.shaders.onFinish("pass/vert", [&res, &gl](const Files::ReadStatus& stat) {
				res.shaders.addVertexShader("pass/vert", Files::text(stat));
			});
			res.shaders.onFinish("pass/frag", [&res, &gl](const Files::ReadStatus& stat) {
				res.shaders.addFragmentShader("pass/frag", Files::text(stat));
			});

			res.shaders.onFinish([&res, &gl]() {
				auto prog = res.programs.create("pass");
				auto vert = res.shaders.id("pass/vert");
				auto frag = res.shaders.id("pass/frag");
				gl.shader.attach(prog, vert);
				gl.shader.attach(prog, frag);

				if (!res.programs.link("pass"))
				{
					status(std::cerr, "program/pass", false);
					std::cerr << "-- Vertex --\n" << res.shaders.compileLog(vert);
					std::cerr << "-- Fragment --\n" << res.shaders.compileLog(frag);
				}
				else
				{
					gl.program.use(prog);
					gl.shader.detach(prog, vert);
					gl.shader.detach(prog, frag);
					gl.shader.del(vert);
					gl.shader.del(frag);
					gl.program.use(0);
					status(std::cout, "program/pass", true);
				}
			});
		}

		gl.viewport.set(glfw.window.framebufferSize(window));
		gl.clear.depthBuffer();
		gl.clear.colorBuffer(v3(0.3f, 0.3f, 0.3f));

		//
		gl.vertexArray.bind(vao);
		gl.buffer.element.bind(ibo);
		res.programs.use("pass");
		gl.draw.triangles.elements(indexes.size());
		//
		glfw.window.swapBuffers(window);
		glfw.pollEvents();

		if (glfw.window.keyPress(window, GLFW_KEY_ESCAPE))
			glfw.window.shouldClose(window, true);
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
	gl.program.del(res.programs.id("pass"));

	glfw.window.destroy(window);
	glfw.terminate();

    return 0;
}
