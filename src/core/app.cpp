#include "app.hpp"
#include "precomp.hpp"
#include "constants.hpp"
#include "palette.hpp"
#include "gui_style.hpp"
#include "utils.hpp"

App::App() : quadtree(raylib::Rectangle{ 0.0f, 0.0f, (float)WIDTH, (float)HEIGHT }), solver(quadtree), spawner(quadtree) {
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
	ImGui::SliderInt("Sub-steps", (int *)&solver.substeps, 1, 10);

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
	ImGui::GetWindowDrawList()->AddLine(convert(center + vec), convert(center + vec - solver.gravity.Normalize().Rotate(arrowAngle) * arrowLength), colorArrow, 1.0f);
	ImGui::GetWindowDrawList()->AddLine(convert(center + vec), convert(center + vec - solver.gravity.Normalize().Rotate(-arrowAngle) * arrowLength), colorArrow, 1.0f);

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

    spawner.update(getRelativeMousePosition(), delta, !ImGui::GetIO().WantCaptureMouse);
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
	camera.target = raylib::Vector2(
        WIDTH / 2.0f - GetRenderWidth() / 2.0f,
        HEIGHT / 2.0f - GetRenderHeight() / 2.0f
    );
}