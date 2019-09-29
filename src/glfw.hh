#pragma once;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gl.hh"

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

		bool shouldClose(GLFWwindow *wnd) const
		{
			return glfwWindowShouldClose(wnd);
		}

		void shouldClose(GLFWwindow *wnd, bool value)
		{
			glfwSetWindowShouldClose(wnd, static_cast<int>(value));
		}

		void makeContextCurrent(GLFWwindow *wnd)
		{
			glfwMakeContextCurrent(wnd);
		}

		void swapBuffers(GLFWwindow *wnd)
		{
			glfwSwapBuffers(wnd);
		}

		GLFWwindow* create(int w, int h, string title)
		{
			return glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
		}

		void destroy(GLFWwindow *wnd)
		{
			glfwDestroyWindow(wnd);
		}

		Dimension framebufferSize(GLFWwindow *wnd) const
		{
			int width, height;
			glfwGetFramebufferSize(wnd, &width, &height);
			return { width, height };
		}

		bool keyPress(GLFWwindow *wnd, int key) const
		{
			return glfwGetKey(wnd, key) == GLFW_PRESS;
		}

		bool keyRelease(GLFWwindow *wnd, int key) const
		{
			return glfwGetKey(wnd, key) == GLFW_RELEASE;
		}

		void pos(GLFWwindow *wnd, const v2 &pos)
		{
			glfwSetWindowPos(wnd, pos.x, pos.y);
		}

		//

		void hint430()
		{
			hintContextVersion(4, 3);
			hintCoreProfileForwardCompat();
		}

		void moveToHalfRight(GLFWwindow *wnd)
		{
			int w, h;
			int fh;
			int mx, my, mw, mh;
			int l, t, r, b;

			glfwGetWindowFrameSize(wnd, &l, &t, &r, &b);
			glfwGetWindowSize(wnd, &w, &h);
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			glfwGetMonitorWorkarea(monitor, &mx, &my, &mw, &mh);

			fh = mh - (t + b - 1);

			glfwSetWindowSize(wnd, mw / 2, fh);
			pos(wnd, v2(mx + mw / 2, my + t));
		}
	} window;
};
