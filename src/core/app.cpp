#include "app.hpp"
#include <imgui.h>
#include <rlImGui.h>

App::App()
    : quadtree(raylib::Rectangle{ 0.0f, 0.0f, (float)WIDTH, (float)HEIGHT }), solver(quadtree), spawner(quadtree) {
    camera.offset = raylib::Vector2{ 0.0f, 0.0f };
    camera.target = raylib::Vector2{ 0.0f, 0.0f };
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
}

App::~App() {
	rlImGuiShutdown();
}

void App::handleInput(float delta) {
	if (ImGui::GetIO().WantCaptureKeyboard) return;

	// Rotate gravity vector
	const float angleVelocity = 1.25f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
		solver.gravity = solver.gravity.Rotate(angleVelocity * delta);
	}
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		solver.gravity = solver.gravity.Rotate(-angleVelocity * delta);
	}

	// Increment/decrement gravity force
	const auto steps = raylib::Vector2::One() * 2.5f * delta;
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
		solver.gravity += steps * solver.gravity.Normalize();
	}
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
		solver.gravity -= steps * solver.gravity.Normalize();
	}

	// Reverse gravity direction
	if (IsKeyPressed(KEY_SPACE)) {
		solver.gravity *= -1.0f;
	}

	// Toggle rendering debug quadtree
	if (IsKeyPressed(KEY_APOSTROPHE) || IsKeyPressed(KEY_GRAVE)) {
		TraceLog(LOG_INFO, "app: Toggling quadtree rendering");
		isRenderingQuadtree = !isRenderingQuadtree;
	}
}

inline ImVec2 convert(raylib::Vector2 a) {
	return ImVec2(a.x, a.y);
}

inline ImU32 convert(raylib::Color a) {
	return IM_COL32(a.r, a.g, a.b, a.a);
}

inline ImU32 convert(ImVec4 a) {
	return IM_COL32((uint8_t)(a.x * 255.0f), (uint8_t)(a.y * 255.0f), (uint8_t)(a.z * 255.0f), (uint8_t)(a.w * 255.0f));
}

void setupStyle() {
	ImGuiStyle &style = ImGui::GetStyle();

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

	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
}

void App::renderGUI() {
	rlImGuiBegin();

	ImGui::Begin("Stats");
	ImGui::Text("FPS: %i", GetFPS());
	ImGui::Text("Frame time: %.2f ms", GetFrameTime() * 1000.0f);
	ImGui::Text("Objects: %i", (int)quadtree.getAll().size());
	ImGui::Text("Subdivisions: %i", quadtree.getSubdivisions());
	ImGui::Text("Collision checks: %i", solver.getAverageIterations());

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	ImGui::Checkbox("Render quadtree", &isRenderingQuadtree);
	ImGui::SliderInt("Substeps", (int *)&solver.substeps, 1, 10);

    if (ImGui::Button("Kill objects")) {
        quadtree.clear();
    }

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	// Apply the size of the circle to the cursor
	ImGui::Text("Gravity: %.2f", solver.gravity.Length());
	ImGui::SameLine();

	// Draw custom circle with arrow
	const float radius = ImGui::GetTextLineHeightWithSpacing();
	constexpr float arrowAngle = 30.0f * DEG2RAD;
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
	ImGui::GetWindowDrawList()->AddLine(
		convert(center + vec), convert(center + vec - solver.gravity.Normalize().Rotate(arrowAngle) * arrowLength), colorArrow, 1.0f
	);
	ImGui::GetWindowDrawList()->AddLine(
		convert(center + vec), convert(center + vec - solver.gravity.Normalize().Rotate(-arrowAngle) * arrowLength), colorArrow, 1.0f
	);

	ImGui::NewLine();
	ImGui::Text("{ %.2f, %.2f }", solver.gravity.x, solver.gravity.y);

	ImGui::End();

	if constexpr (ENABLE_TEMPERATURE) {
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

void App::setup() {
#ifdef DEBUG
	SetTraceLogLevel(LOG_DEBUG);
#else
    SetTraceLogLevel(LOG_ERROR);
#endif

	TraceLog(LOG_INFO, "app: Starting...");
	SetRandomSeed(GetTime());

	rlImGuiSetup(true);
	setupStyle();
}

void App::update() {
	auto delta = GetFrameTime();
	spawner.update(delta, !ImGui::GetIO().WantCaptureMouse);
	solver.solve(delta);

	handleInput(delta);
}

void App::render() {
#ifdef CENTER_CIRCLE
	ClearBackground(PALETTE_GREY);
	DrawCircle(GetRenderWidth() / 2, GetRenderHeight() / 2, 400.0f, PALETTE_BLACK);
#else
	ClearBackground(PALETTE_BLACK);
#endif

    // Use the 2D camera to translate the simulation if it's smaller than the window
    BeginMode2D(camera);

	if (isRenderingQuadtree) {
		quadtree.render();
	} else {
		auto all = quadtree.getAll();
		for (auto object : all) {
			object->render();
		}
	}

    EndMode2D();

	renderGUI();
}

void App::resize() {
    // Ensure the simulation is centered
    auto vec = raylib::Vector2(
        WIDTH / 2.0f - GetRenderWidth() / 2.0f,
        HEIGHT / 2.0f - GetRenderHeight() / 2.0f
    );
    camera.target = vec;
}