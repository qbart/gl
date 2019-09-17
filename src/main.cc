#include <iostream>
#include "glm.hh"
#include "gl.hh"
#include "glew.hh"
#include "glfw.hh"
#include "types.hh"
#include <vector>
#include <unordered_map>
#include <sstream>


class Shaders
{
private:
	GL gl;
	vector<uint> shaders;

public:
	uint add(uint shaderType, const string &source);
	auto addVertexShader(const string &source) { return add(GL_VERTEX_SHADER, source); }
	auto addFragmentShader(const string &source) { return add(GL_FRAGMENT_SHADER, source); }

	string compileLog(uint id);
};


uint Shaders::add(uint type, const string &source)
{
	auto id = gl.shader.create(type);
	gl.shader.source(id, source);
	gl.shader.compile(id);

	if (gl.shader.getInt(id, GL_COMPILE_STATUS))
		shaders.push_back(id);

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

string VERTEX_SHADER = R"(
#version 430 core

void main()
{

}

)";

string FRAGMENT_SHADER = R"(
#version 430 core

void main()
{
}

)";





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

	Shaders shaders;
	auto vert = shaders.addVertexShader(VERTEX_SHADER);
	auto frag = shaders.addFragmentShader(FRAGMENT_SHADER);
	std::cout << shaders.compileLog(vert);
	std::cout << shaders.compileLog(frag);

	auto prog = gl.program.create();
	gl.shader.attach(prog, vert);
	gl.shader.attach(prog, frag);
	gl.program.link(prog);
	if (!gl.program.getInt(prog, GL_LINK_STATUS))
		std::cerr << "Program not linked\n";
	else
		std::cout << "Program: ok\n";

	gl.program.use(prog);
	gl.shader.detach(prog, vert);
	gl.shader.detach(prog, frag);
	gl.shader.del(vert);
	gl.shader.del(frag);
	gl.program.use(0);

	while (!glfw.window.shouldClose(window))
	{
		gl.viewport.set(glfw.window.framebufferSize(window));

		glfw.window.swapBuffers(window);
		glfw.pollEvents();

		if (glfw.window.keyPress(window, GLFW_KEY_ESCAPE))
			glfw.window.shouldClose(window, true);
	}

	glfw.window.destroy(window);
	glfw.terminate();

    return 0;
}
