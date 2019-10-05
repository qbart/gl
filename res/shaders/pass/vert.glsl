#version 430 core

layout(location = 0) in vec3 attrPosition;
layout(location = 1) in vec3 attrColor;

out vec3 vertexColor;

uniform mat4 PROJ;
uniform mat4 VIEW;
uniform mat4 MODEL;

void main()
{
	gl_Position = PROJ * VIEW * MODEL * vec4(attrPosition, 1);
   vertexColor = attrColor;
}
