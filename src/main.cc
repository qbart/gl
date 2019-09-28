#include <iostream>
#include "glm.hh"
#include "gl.hh"
#include "glew.hh"
#include "glfw.hh"
#include "types.hh"
#include "files.hh"
#include "programs.hh"
#include "log.hh"
#include "ui.hh"
#include <vector>
#include <unordered_map>
#include <sstream>


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
	Resources res;

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

	res.programs.load("pass");

	bool reloading = false;

	while (!glfw.window.shouldClose(window))
	{
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
			res.programs.load("pass");
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
