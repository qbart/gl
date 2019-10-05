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
#include "default_app.hh"
#include <vector>
#include <unordered_map>
#include <sstream>

auto cube(float size)
{
	struct
	{
		vector<v3> verts;
		vector<v3> colors;
		vector<u32> indices;
	} mesh;

	float half = size * 0.5f;
	float top = size;
	float bottom = 0;

	mesh.verts = {
		// Face 1
		// left bottom
		v3(-half, bottom, -half),
		v3(-half, bottom,  half),
		v3(-half, top, half),
		// left top
		v3(-half, bottom, -half),
		v3(-half, top, half),
		v3(-half, top,-half),
		// Face 2
		// front bottom
		v3(-half, bottom,  half),
		v3(half, bottom,  half),
		v3(-half, top, half),
		// front top
		v3(half, bottom,  half),
		v3(half, top, half),
		v3(-half, top, half),
		// Face 3
		// right bottom
		v3(half, bottom,  half),
		v3(half, bottom, -half),
		v3(half, top, half),
		// right top
		v3(half, bottom, -half),
		v3(half, top,-half),
		v3(half, top, half),
		// Face 4
		// back bottom
		v3(half, bottom, -half),
		v3(-half, bottom, -half),
		v3(half, top,-half),
		// back top
		v3(-half, bottom, -half),
		v3(-half, top,-half),
		v3(half, top,-half),
		// Face 5
		// up bottom
		v3(half, top,-half),
		v3(-half, top,-half),
		v3(half, top, half),
		// up top
		v3(-half, top,-half),
		v3(-half, top, half),
		v3(half, top, half),
		// Face 6
		// down bottom
		v3(-half, bottom,  half),
		v3(-half, bottom, -half),
		v3(half, bottom,  half),
		// down top
		v3(-half, bottom, -half),
		v3(half, bottom, -half),
		v3(half, bottom,  half)
	};

	mesh.indices = {
		// Face 1
		// left bottom
		0, 1, 2,
		// left top
		3, 4, 5,
		// Face 2
		// front bottom
		6, 7, 8,
		// front top
		9, 10, 11,
		// Face 3
		// right bottom
		12, 13, 14,
		// right top
		15, 16, 17,
		// Face 4
		// back bottom
		18, 19, 20,
		// back top
		21, 22, 23,
		// Face 5
		// up bottom
		24, 25, 26,
		// up top
		27, 28, 29,
		// Face 6
		// down bottom
		30, 31, 32,
		// down top
		33, 34, 35
	};

	for (auto i = 0; i < 6; ++i)
	{
		mesh.colors.push_back(v3(1, 0, 0));
		mesh.colors.push_back(v3(1, 1, 0));
		mesh.colors.push_back(v3(0, 1, 0));
		mesh.colors.push_back(v3(0, 1, 1));
		mesh.colors.push_back(v3(0, 0, 1));
		mesh.colors.push_back(v3(1, 0, 1));
	}

	return std::move(mesh);
}

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
	DefaultApp app;

	if (!app.init(1280, 1024, "GL"))
		return 1;

	auto ctx = ui.init(app.wnd);

	auto mesh = std::move(cube(1));

	uint vao = gl.vertexArray.create();
	gl.vertexArray.bind(vao);

	uint vbo = gl.buffer.gen();
	gl.buffer.array.bind(vbo);
	gl.buffer.array.data(mesh.verts);

	uint cbo = gl.buffer.gen();
	gl.buffer.array.bind(cbo);
	gl.buffer.array.data(mesh.colors);

	uint ibo = gl.buffer.gen();
	gl.buffer.element.bind(ibo);
	gl.buffer.element.data(mesh.indices);
		
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

	mat4 view = glm::lookAt(v3(-2, 10, 10), v3(0, 0, 0), UP);

	while (app.isOpen())
	{
		ui.beginFrame();
		ui.endFrame();

		auto size = app.framebufferSize();
		auto proj = gl.m.fov(55, size);
		auto model = mat4(1.0f);
		gl.viewport.set(size);
		gl.clear.depthBuffer();
		gl.clear.colorBuffer(v3(0.3f, 0.3f, 0.3f));
		
		//
		gl.vertexArray.bind(vao);
		gl.buffer.element.bind(ibo);
		res.programs.use("pass");
		res.programs.uniform("PROJ", proj);
		res.programs.uniform("VIEW", view);
		res.programs.uniform("MODEL", model);
		gl.draw.triangles.elements(mesh.indices.size());
		//
		ui.draw();
		app.process();

		if (app.keyPress(GLFW_KEY_ESCAPE))
			app.close();

		if (app.keyReleasedOnce(GLFW_KEY_F5))
			res.programs.reloadAll();
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

	res.programs.delAll();

	ui.terminate();

    return 0;
}
