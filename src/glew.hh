#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct GLEW
{
	bool init()
	{
		glewExperimental = GL_TRUE;
		return (glewInit() == GLEW_OK);
	}
};
