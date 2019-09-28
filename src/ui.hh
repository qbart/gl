#pragma once
#include "types.hh"
#include "glfw.hh"
#include <sstream>

#ifdef UI_IMGUI

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

struct UI
{
	struct Context
	{
		ImGuiContext* ptr = nullptr;
		unordered_map<uint, uint> fonts;

		void pushFontSize(uint size) { ImGui::PushFont(io().Fonts->Fonts[fonts[size]]); }
		void popFontSize() { ImGui::PopFont(); }

		ImGuiIO& io() { return ImGui::GetIO(); }
	};

	Context init(GLFWwindow *wnd);
	void terminate();
	void beginFrame();
	void endFrame();
	void draw();
};
#endif
