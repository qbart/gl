#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "types.hh"
#include <sstream>
#include <iostream>

struct GL
{
	struct M
	{
		mat4 fov(float degAngle, const Dimension &size, float zNear = 0.001f, float zFar = 1000.0f)
		{
			return glm::perspectiveFov(glm::radians(degAngle), (float)size.w, (float)size.h, zNear, zFar);
		}
	} m;

	string getString(uint name)
	{
		return string(reinterpret_cast<const char*>(glGetString(name)));
	}

	int getInt(uint name)
	{
		int val;
		glGetIntegerv(name, &val);
		return val;
	}

	float getFloat(uint name)
	{
		float val;
		glGetFloatv(name, &val);
		return val;
	}

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

		int getInt(uint shader, uint param) const
		{
			int val;
			glGetShaderiv(shader, param, &val);
			return val;
		}

		string infoLog(uint shader, int bufSize) const
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

		void set(uint loc, const v3 &v)
		{
			glUniform3fv(loc, 1, glm::value_ptr(v));
		}

		void set(uint loc, const mat4 &mat)
		{
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
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

struct Bytes
{
public:
	explicit Bytes(int _bytes) : bytes(_bytes) {}

	int mb() const
	{
		return bytes / 1024 / 1024;
	}
	std::string mb_s() const
	{
		std::stringstream ss;
		ss << mb();
		ss << " MB";
		return ss.str();
	}

private:
	int bytes;
};

static void gl_printInfo()
{
	GL gl;

	std::cout << "gl:            " << gl.getString(GL_VERSION) << "\n";
	std::cout << "glsl.version:  " << gl.getString(GL_SHADING_LANGUAGE_VERSION) << "\n";
	std::cout << "ssbo.max:      " << Bytes(gl.getInt(GL_MAX_SHADER_STORAGE_BLOCK_SIZE)).mb_s() << "\n";
	std::cout << "ubo.max:       " << Bytes(gl.getInt(GL_MAX_UNIFORM_BLOCK_SIZE)).mb_s() << "\n";
	std::cout << "viewports.max: " << gl.getInt(GL_MAX_VIEWPORTS) << "\n";
	std::cout << "\n";
	std::cout << "framebuffer.width.max:   " << gl.getInt(GL_MAX_FRAMEBUFFER_WIDTH) << "\n";
	std::cout << "framebuffer.height.max:  " << gl.getInt(GL_MAX_FRAMEBUFFER_HEIGHT) << "\n";
	std::cout << "framebuffer.layers.max:  " << gl.getInt(GL_MAX_FRAMEBUFFER_LAYERS) << "\n";
	std::cout << "framebuffer.samples.max: " << gl.getInt(GL_MAX_FRAMEBUFFER_SAMPLES) << "\n";
	std::cout << "\n";
	std::cout << "texture.max.size: " << gl.getInt(GL_MAX_TEXTURE_SIZE) << "\n";
	std::cout << "texture_image.max.units: " << gl.getInt(GL_MAX_TEXTURE_IMAGE_UNITS) << "\n";
	std::cout << "texture_buffer.max.size: " << gl.getInt(GL_MAX_TEXTURE_BUFFER_SIZE) << "\n";
	std::cout << "texture.max.rectangle_size: " << gl.getInt(GL_MAX_RECTANGLE_TEXTURE_SIZE) << "\n";
	std::cout << "depth_texture_samples.max: " << gl.getInt(GL_MAX_DEPTH_TEXTURE_SAMPLES) << "\n";
	std::cout << "renderbuffer.max.size: " << gl.getInt(GL_MAX_RENDERBUFFER_SIZE) << "\n";
	std::cout << "cube_map.texture.max.size: " << gl.getInt(GL_MAX_CUBE_MAP_TEXTURE_SIZE) << "\n";
	if (GLEW_EXT_texture_filter_anisotropic)
		std::cout << "texture.anisotropy.max: " << gl.getFloat(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT) << "\n";
	else
		std::cout << "texture.anisotropy.max: not supported\n";
	std::cout << "\n";
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
