#include "ui.hh"

#ifdef UI_IMGUI
#include "types.hh"

UI::Context UI::init(GLFWwindow *wnd)
{
	Context ctx;
	ctx.ptr = ImGui::CreateContext();

	ctx.io().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	auto &style = ImGui::GetStyle();
	auto &colors = style.Colors;

	style.ScaleAllSizes(2);

	auto robotopath = string(FONTS_PATH) + "Roboto-Regular.ttf";
	auto io = ctx.io();
	io.Fonts->AddFontFromFileTTF(robotopath.c_str(), 24);
	io.Fonts->AddFontFromFileTTF(robotopath.c_str(), 16);
	ctx.fonts[24] = 0;
	ctx.fonts[16] = 1;

	v4 clTransparent(rgb(0, 0, 0, 0));
	v4 cPrimary(rgb(4, 218, 198));
	v4 cPrimaryHover(rgb(4, 250, 198));
	v4 cPrimaryActive(rgb(187, 134, 252));

	//colors[ImGuiCol_Button] = cPrimary;
	//colors[ImGuiCol_ButtonHovered] = cPrimaryHover;
	//colors[ImGuiCol_ButtonActive] = cPrimaryActive;

	//colors[ImGuiCol_ScrollbarBg] = clTransparent;
	//colors[ImGuiCol_ScrollbarGrab] = cPrimary;
	//colors[ImGuiCol_ScrollbarGrabHovered] = cPrimaryHover;
	//colors[ImGuiCol_ScrollbarGrabActive] = cPrimaryActive;

	//colors[ImGuiCol_Separator] = cPrimary;
	//colors[ImGuiCol_SeparatorHovered] = cPrimaryHover;
	//colors[ImGuiCol_SeparatorActive] = cPrimaryActive;

	//colors[ImGuiCol_ResizeGrip] = cPrimary;
	//colors[ImGuiCol_ResizeGripHovered] = cPrimaryHover;
	//colors[ImGuiCol_ResizeGripActive] = cPrimaryActive;

	//colors[ImGuiCol_FrameBg] = rgb(0, 0, 0);
	//colors[ImGuiCol_FrameBgHovered] = cPrimaryHover;
	//colors[ImGuiCol_FrameBgActive] = cPrimaryActive;


	/*


			colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
			colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.69f, 0.40f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.42f, 0.41f, 0.64f, 0.69f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);

			colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
			colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);

			colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);


			colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
			colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
			colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
			colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
			colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
			colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	*/

	std::stringstream ss;
	ss << "#version ";
	ss << 4;
	ss << 3;
	ss << 0;
	ImGui_ImplGlfw_InitForOpenGL(wnd, true);
	ImGui_ImplOpenGL3_Init(ss.str().c_str());

	return ctx;
}

void UI::terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::beginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UI::endFrame()
{
	ImGui::Render();
}

void UI::draw()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::demo()
{
	ImGui::ShowDemoWindow();
}

#endif