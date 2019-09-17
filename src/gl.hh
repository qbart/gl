#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include "types.hh"

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
		static const unordered_map<int, string> Types;

		uint create(uint type)
		{
			return glCreateShader(type);
		}

		void source(uint shader, const string &src)
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

		string infoLog(uint shader, int bufSize)
		{
			vector<char> log;
			if (bufSize > 0)
			{
				log.resize(bufSize);
				int len;
				glGetShaderInfoLog(shader, bufSize, &len, &log[0]);
			}
			return string(std::begin(log), std::end(log));
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

		string infoLog(uint program, int bufSize)
		{
			vector<char> log;
			if (bufSize > 0)
			{
				int len;
				glGetProgramInfoLog(program, bufSize, &len, &log[0]);
			}
			return string(std::begin(log), std::end(log));
		}
	} program;

	struct Uniform
	{
		int location(uint program, const string& name)
		{
			return glGetUniformLocation(program, name.c_str());
		}

	} uniform;
};

const std::unordered_map<int, string> GL::Shader::Types = {
	{GL_COMPUTE_SHADER, "GL_COMPUTE_SHADER"},
	{GL_VERTEX_SHADER, "GL_VERTEX_SHADER"},
	{GL_TESS_CONTROL_SHADER, "GL_TESS_CONTROL_SHADER"},
	{GL_TESS_EVALUATION_SHADER, "GL_TESS_EVALUATION_SHADER"},
	{GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"},
	{GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"},
};
