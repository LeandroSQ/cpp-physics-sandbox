#pragma once

#include "precomp.hpp"
#include "palette.hpp"

inline void setupStyle() {
	ImGuiStyle &style = ImGui::GetStyle();

    auto primary = ImVec4(0.13f, 0.61f, 0.93f, 1.00f);
    auto background = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    auto text = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);

	style.WindowMinSize = ImVec2(160, 20);
	style.FramePadding = ImVec2(4, 2);
	style.ItemSpacing = ImVec2(6, 2);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.Alpha = 0.95f;
	style.WindowRounding = 4.0f;
	style.FrameRounding = 2.0f;
	style.IndentSpacing = 6.0f;
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 14.0f;
	style.GrabRounding = 16.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.SeparatorTextPadding = ImVec2(10, 10);

	style.Colors[ImGuiCol_Text] = text;
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(text.x, text.y, text.z, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = background;
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.13f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(primary.x, primary.y, primary.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = primary;
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = primary;
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(primary.x, primary.y, primary.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = primary;
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = primary;
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(primary.x, primary.y, primary.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = primary;
	style.Colors[ImGuiCol_Header] = ImVec4(primary.x, primary.y, primary.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(primary.x, primary.y, primary.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = primary;
	style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(primary.x, primary.y, primary.z, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = primary;
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(primary.x, primary.y, primary.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = primary;
	style.Colors[ImGuiCol_PlotLines] = ImVec4(text.x, text.y, text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = primary;
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(primary.x, primary.y, primary.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = primary;
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(primary.x, primary.y, primary.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
}