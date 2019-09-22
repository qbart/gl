#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include "types.hh"

using Dimension = struct { int w, h; };

struct GL
{
	struct Get
	{
		string str(uint name)
		{
			return string(reinterpret_cast<const char*>(glGetString(name)));
		}

		int integer(uint name)
		{
			int val;
			glGetIntegerv(name, &val);
			return val;
		}
	} get;

	struct Clear
	{
		void colorBuffer(const v3 &floats, int drawBuffer = 0)
		{
			v4 f(floats, 1);
			glClearBufferfv(GL_COLOR, drawBuffer, &f[0]);
		}

		void depthBuffer(float val = 1)
		{
			glClearBufferfv(GL_DEPTH, 0, &val);
		}
	} clear;

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

	struct VertexArray
	{
		uint create()
		{
			uint va;
			glCreateVertexArrays(1, &va);
			return va;
		}

		void bind(uint va)
		{
			glBindVertexArray(va);
		}

		void del(uint va)
		{
			glDeleteVertexArrays(1, &va);
		}
	} vertexArray;

	struct VertexAttrib
	{
		void enableArray(uint index)
		{
			glEnableVertexAttribArray(index);
		}

		void disableArray(uint index)
		{
			glDisableVertexAttribArray(index);
		}

		void pointer(uint index, int size, uint type)
		{
			glVertexAttribPointer(index, size, type, GL_FALSE, 0, (void *)0);
		}

		void pointerFloat(uint index, int size)
		{
			glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, (void *)0);
		}
	} vertexAttrib;

	struct Buffer
	{
		uint gen()
		{
			uint buf;
			glGenBuffers(1, &buf);
			return buf;
		}

		void del(uint buf)
		{
			glDeleteBuffers(1, &buf);
		}

		void bind(uint target, uint buf)
		{
			glBindBuffer(target, buf);
		}

		void data(uint target, const vector<v3> &vv, uint usage = GL_STATIC_DRAW)
		{
			glBufferData(target, sizeof(v3) * vv.size(), &vv[0], usage);
		}

		void data(uint target, const vector<u32> &vv, uint usage = GL_STATIC_DRAW)
		{
			glBufferData(target, sizeof(u32) * vv.size(), &vv[0], usage);
		}

		// array
		struct Array
		{
			void bind(uint buf)
			{
				glBindBuffer(GL_ARRAY_BUFFER, buf);
			}

			void data(const vector<v3> &vv, uint usage = GL_STATIC_DRAW)
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(v3) * vv.size(), &vv[0], usage);
			}
		} array;

		// element
		struct Element
		{
			void bind(uint buf)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf);
			}

			void data(const vector<u32> &ii, uint usage = GL_STATIC_DRAW)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * ii.size(), &ii[0], usage);
			}
		} element;
	} buffer;

	struct Draw
	{
		void arrays(uint mode, int offset, int count)
		{
			glDrawArrays(mode, offset, count);
		}

		void elements(uint mode, int count)
		{
			glDrawElements(mode, sizeof(u32) * count, GL_UNSIGNED_INT, nullptr);
		}

		void elementsBaseVertex(uint mode, int count, int offset = 0)
		{
			glDrawElementsBaseVertex(mode, sizeof(u32) * count, GL_UNSIGNED_INT, nullptr, offset);
		}

		// custom
		struct Triangles
		{
			void arrays(int offset, int count)
			{
				glDrawArrays(GL_TRIANGLES, offset, count);
			}

			void elements(int count)
			{
				glDrawElements(GL_TRIANGLES, sizeof(u32) * count, GL_UNSIGNED_INT, nullptr);
			}

			void elementsBaseVertex(int count, int offset = 0)
			{
				glDrawElementsBaseVertex(GL_TRIANGLES, sizeof(u32) * count, GL_UNSIGNED_INT, nullptr, offset);
			}
		} triangles;
	} draw;

	struct CullFace
	{
		void enable(bool val = true)
		{
			if (val)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);
		}

		void back()
		{
			glCullFace(GL_BACK);
		}

		void front()
		{
			glCullFace(GL_FRONT);
		}

		void ccwFront()
		{
			glFrontFace(GL_CCW);
		}

		void cwFront()
		{
			glFrontFace(GL_CW);
		}
	} cullFace;
};

const std::unordered_map<int, string> GL::Shader::Types = {
	{GL_COMPUTE_SHADER, "GL_COMPUTE_SHADER"},
	{GL_VERTEX_SHADER, "GL_VERTEX_SHADER"},
	{GL_TESS_CONTROL_SHADER, "GL_TESS_CONTROL_SHADER"},
	{GL_TESS_EVALUATION_SHADER, "GL_TESS_EVALUATION_SHADER"},
	{GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"},
	{GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"},
};
