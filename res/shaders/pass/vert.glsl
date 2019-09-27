#version 430 core

layout(location = 0) in vec3 attrPosition;
layout(location = 1) in vec3 attrColor;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(attrPosition, 1);
   vertexColor = attrColor;
}
