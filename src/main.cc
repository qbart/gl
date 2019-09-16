#include <iostream>
#include "glm.hh"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <sstream>

using Dimension = struct { int w, h; };

struct GL
{
	struct Viewport
	{
		void set(int x, int y, int w, int h)
		{
			glViewport(x, y, w, h);
		}

		void set(const Dimension &dim)
		{
			glViewport(0, 0, dim.w, dim.h);
		}

	} viewport;

	struct Shader
	{
		static const std::unordered_map<int, std::string> Types;

		uint create(uint type)
		{
			return glCreateShader(type);
		}

		void source(uint shader, const std::string &src)
		{
			auto c_str = src.c_str();
			glShaderSource(shader, 1, &c_str, nullptr);
		}

		void compile(uint shader)
		{
			glCompileShader(shader);
		}

		int getInt(uint shader, uint param)
		{
			int val;
			glGetShaderiv(shader, param, &val);
			return val;
		}

		std::string infoLog(uint shader, int bufSize)
		{
			std::vector<char> log;
			if (bufSize > 0)
			{
				int len;
				glGetShaderInfoLog(shader, bufSize, &len, &log[0]);
			}
			return std::string(std::begin(log), std::end(log));
		}

		void attach(uint program, uint shader)
		{
			glAttachShader(program, shader);
		}

		void detach(uint program, uint shader)
		{
			glDetachShader(program, shader);
		}

		void del(uint shader)
		{
			glDeleteShader(shader);
		}
	} shader;

	struct Program
	{
		uint create()
		{
			return glCreateProgram();
		}

		void del(uint program)
		{
			glDeleteProgram(program);
		}

		void use(uint program)
		{
			glUseProgram(program);
		}

		void link(uint program)
		{
			glLinkProgram(program);
		}

		int getInt(uint program, uint param)
		{
			int val;
			glGetProgramiv(program, param, &val);
			return val;
		}

		std::string infoLog(uint program, int bufSize)
		{
			std::vector<char> log;
			if (bufSize > 0)
			{
				int len;
				glGetProgramInfoLog(program, bufSize, &len, &log[0]);
			}
			return std::string(std::begin(log), std::end(log));
		}
	} program;

	struct Uniform
	{
		int location(uint program, const std::string& name)
		{
			return glGetUniformLocation(program, name.c_str());
		}

	} uniform;
};

const std::unordered_map<int, std::string> GL::Shader::Types = {
	{GL_COMPUTE_SHADER, "GL_COMPUTE_SHADER"},
	{GL_VERTEX_SHADER, "GL_VERTEX_SHADER"},
	{GL_TESS_CONTROL_SHADER, "GL_TESS_CONTROL_SHADER"},
	{GL_TESS_EVALUATION_SHADER, "GL_TESS_EVALUATION_SHADER"},
	{GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"},
	{GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"},
};

class Shaders
{
private:
	GL gl;
	std::vector<u32> shaders;

public:
	u32 add(uint shaderType, const std::string &source);
	auto addVertexShader(const std::string &source) { return add(GL_VERTEX_SHADER, source); }
	auto addFragmentShader(const std::string &source) { return add(GL_FRAGMENT_SHADER, source); }

	std::string compileLog(uint id);
};


u32 Shaders::add(uint type, const std::string &source)
{
	auto id = gl.shader.create(type);
	gl.shader.source(id, source);
	gl.shader.compile(id);

	if (gl.shader.getInt(id, GL_COMPILE_STATUS))
		shaders.push_back(id);
	else
		id = 0;
		
	return id;
}

std::string Shaders::compileLog(uint id)
{
	auto type = gl.shader.getInt(id, GL_SHADER_TYPE);
	auto maxLen = gl.shader.getInt(id, GL_INFO_LOG_LENGTH);

	std::string kind = "???";
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

std::string VERTEX_SHADER = R"(
#version 430 core

void main()
{
}

)";

std::string FRAGMENT_SHADER = R"(
#version 430 core

void main()
{
}

)";

struct GLEW
{
	bool init()
	{
		glewExperimental = GL_TRUE;
		return (glewInit() == GLEW_OK);
	}
};

struct GLFW
{
	bool init() { return glfwInit(); }
	void terminate() { glfwTerminate(); }
	void pollEvents() { glfwPollEvents(); }

	struct Window
	{
		void hintContextVersion(int major, int minor)
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		}

		void hintCoreProfileForwardCompat()
		{
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		}

		void hintResizable(bool resizable)
		{
			glfwWindowHint(GLFW_RESIZABLE, static_cast<int>(resizable));
		}

		bool shouldClose(GLFWwindow* wnd)
		{
			return glfwWindowShouldClose(wnd);
		}

		void shouldClose(GLFWwindow* wnd, bool value)
		{
			glfwSetWindowShouldClose(wnd, static_cast<int>(value));
		}

		void makeContextCurrent(GLFWwindow* wnd)
		{
			glfwMakeContextCurrent(wnd);
		}

		void swapBuffers(GLFWwindow* wnd)
		{
			glfwSwapBuffers(wnd);
		}

		GLFWwindow* create(int w, int h, std::string title)
		{
			return glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
		}

		void destroy(GLFWwindow* wnd)
		{
			glfwDestroyWindow(wnd);
		}

		Dimension framebufferSize(GLFWwindow* wnd)
		{
			int width, height;
			glfwGetFramebufferSize(wnd, &width, &height);
			return { width, height };
		}

		bool keyPress(GLFWwindow* wnd, int key)
		{
			return glfwGetKey(wnd, key) == GLFW_PRESS;
		}

		bool keyRelease(GLFWwindow* wnd, int key)
		{
			return glfwGetKey(wnd, key) == GLFW_RELEASE;
		}

	} window;
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
