#include "../precomp.hpp"
#include "../settings.hpp"
#include "../utils.hpp"
#include "../models/app.hpp"
#include "imgui.h"

void App::setupGUI() {
    rlImGuiSetup(true);
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

void App::renderGUI() {
	rlImGuiBegin();

	ImGui::Begin("Stats");
	ImGui::Text("FPS: %i", GetFPS());
	ImGui::Text("Frame time: %.2f ms", GetFrameTime() * 1000.0f);
    if (ENABLE_AUTO_ADJUST_SUBSTEPS) {
        ImGui::Text("Perceived time: %.2f ms", frameTimeSum / frameCounter * 1000.0f);
        ImGui::Text("Sub-steps: %i", solver.substeps);
    }
	ImGui::Text("Objects: %i", (int)quadtree.getAll().size());
	ImGui::Text("Subdivisions: %i", quadtree.getSubdivisions());
	ImGui::Text("Collision checks: %i", solver.getAverageIterations());

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	ImGui::Checkbox("Render quadtree", &isRenderingQuadtree);
	ImGui::Checkbox("Constraint to circle", &ENABLE_CIRCLE_CONSTRAINT);
    if (ENABLE_CIRCLE_CONSTRAINT && ENABLE_TEMPERATURE) {
        ImGui::Checkbox("Heat from circle border", &ENABLE_HEAT_FROM_CIRCLE_BORDER);
    }

    ImGui::Checkbox("Auto adjust simulation quality", &ENABLE_AUTO_ADJUST_SUBSTEPS);
    if (!ENABLE_AUTO_ADJUST_SUBSTEPS) {
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
	    ImGui::SliderInt("Sub-steps", (int *)&solver.substeps, 1, 10);
    }

    ImGui::Checkbox("Spread spawn", &ENABLE_SPAWN_SPREAD);
	if (ImGui::Button("Kill objects")) {
		quadtree.clear();
	}
    if (quadtree.size() < SPAWN_COUNT) {
        ImGui::SameLine();
        ImGui::Checkbox("Is spawning", &spawner.isSpawning);
    }

    ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();
    ImGui::Text("Render mode");

    if (ImGui::RadioButton("Solid color", ENABLE_SOLID_COLOR)) {
        ENABLE_RAINBOW_COLORS = false;
        ENABLE_FIXED_RAINBOW = false;
        ENABLE_TEMPERATURE = false;
        ENABLE_SOLID_COLOR = true;
    }
    if (ImGui::RadioButton("Rainbow on spawn", ENABLE_RAINBOW_COLORS)) {
        ENABLE_FIXED_RAINBOW = false;
        ENABLE_RAINBOW_COLORS = true;
        ENABLE_SOLID_COLOR = false;
        ENABLE_TEMPERATURE = false;
    }
    if (ImGui::RadioButton("Rainbow gradient", ENABLE_FIXED_RAINBOW)) {
        ENABLE_RAINBOW_COLORS = false;
        ENABLE_FIXED_RAINBOW = true;
        ENABLE_SOLID_COLOR = false;
        ENABLE_TEMPERATURE = false;
    }
    if (ImGui::RadioButton("Temperature", ENABLE_TEMPERATURE)) {
        ENABLE_TEMPERATURE = true;
        ENABLE_SOLID_COLOR = false;
        ENABLE_FIXED_RAINBOW = false;
        ENABLE_RAINBOW_COLORS = false;
    }

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	// Apply the size of the circle to the cursor
	ImGui::Text("Gravity: %.2f", solver.gravity.Length());
	ImGui::SameLine();

	// Draw custom circle with arrow
	const float radius = ImGui::GetTextLineHeightWithSpacing();
	 float arrowAngle = 30.0f * DEG2RAD;
	const float arrowLength = radius / 2.5f;
	const auto vec = solver.gravity.Normalize() * radius * 0.8f;
	const auto center = raylib::Vector2(
        ImGui::GetWindowPos().x + ImGui::GetContentRegionMax().x - radius,
        ImGui::GetCursorScreenPos().y + radius
    );
	const auto color = convert(ImGui::GetStyle().Colors[ImGuiCol_Text]);
	const auto colorArrow = convert(ImGui::GetStyle().Colors[ImGuiCol_CheckMark]);
	ImGui::GetWindowDrawList()->AddCircle(convert(center), radius, color);
	ImGui::GetWindowDrawList()->AddLine(convert(center), convert(center + vec), colorArrow, 1.0f);
	ImGui::GetWindowDrawList()->AddLine(convert(center + vec), convert(center + vec - solver.gravity.Normalize().Rotate(arrowAngle) * arrowLength), colorArrow, 1.0f);
	ImGui::GetWindowDrawList()->AddLine(convert(center + vec), convert(center + vec - solver.gravity.Normalize().Rotate(-arrowAngle) * arrowLength), colorArrow, 1.0f);

	ImGui::NewLine();
	ImGui::Text("{ %.2f, %.2f }", solver.gravity.x, solver.gravity.y);

	ImGui::End();

	if  (ENABLE_TEMPERATURE) {
		ImGui::Begin("Controls");
		ImGui::SliderFloat("Transfer contact", &solver.temperatureTransferContactMultiplier, 0.0f, 1.0f);
		ImGui::SliderFloat("Transfer air", &solver.temperatureTransferAirMultiplier, 0.0f, 0.25f);
		ImGui::SliderFloat("Transfer ground", &solver.temperatureTransferGroundMultiplier, 0.0f, 1.0f);
		ImGui::SliderFloat("Floating force", &solver.temperatureFloatingForce, 0.0f, 100.0f);
		ImGui::End();
	}

	if (ImGui::GetIO().WantCaptureMouse && (ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive())) {
		SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
	} else {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	}

	rlImGuiEnd();
}