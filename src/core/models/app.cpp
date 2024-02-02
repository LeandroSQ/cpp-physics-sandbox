#include "app.hpp"
#include "../precomp.hpp"
#include "../settings.hpp"
#include "../interface/palette.hpp"
#include "../utils.hpp"
#include "raylib.h"

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

    // Zero gravity
    if (IsKeyPressed(KEY_Z)) {
        if (solver.gravity.Length() == 0.0f)
            solver.gravity = raylib::Vector2(0.0f, 9.8f);
        else
            solver.gravity = raylib::Vector2::Zero();
    }

	// Toggle rendering debug quadtree
	if (IsKeyPressed(KEY_APOSTROPHE) || IsKeyPressed(KEY_GRAVE)) {
		TraceLog(LOG_INFO, "app: Toggling quadtree rendering");
		isRenderingQuadtree = !isRenderingQuadtree;
	}
}

void App::setup() {
#ifdef DEBUG
	SetTraceLogLevel(LOG_DEBUG);
#else
	SetTraceLogLevel(LOG_WARNING);
#endif

	TraceLog(LOG_INFO, "app: Starting...");
	SetRandomSeed(GetTime());

	setupGUI();
}

void App::update() {
	auto delta = GetFrameTime();

    spawner.update(getRelativeMousePosition(), solver.substeps, delta, !ImGui::GetIO().WantCaptureMouse);
	solver.solve(delta);

	handleInput(delta);
}

void App::render() {
    // Background
    ClearBackground(PALETTE_GREY);
    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, CENTER_CIRCLE_CURRENT_RADIUS, PALETTE_BLACK);

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
        WIDTH / 2.0f - GetScreenWidth() / 2.0f,
        HEIGHT / 2.0f - GetScreenHeight() / 2.0f
    );
}

void App::onFrameStart() {
    frameStartTime = GetTime();
}

void App::onFrameEnd() {
    if (!ENABLE_AUTO_ADJUST_SUBSTEPS) return;

    const float frameTime = GetTime() - frameStartTime;
    frameCounter++;
    frameTimeSum += frameTime;

    if (frameCounter <= TARGET_FPS) return;

    // Detect slowness and adjust simulation substeps accordingly
    float expectedFrameTime = (1.0f / TARGET_FPS);
    float averageFrameTime = (frameTimeSum / frameCounter);
    if (solver.substeps > 1 && averageFrameTime > expectedFrameTime) {
        TraceLog(LOG_WARNING, "Detected slowness, decreasing simulation substeps. Simulation time: %.2fms (%.2fms expected)", averageFrameTime, expectedFrameTime);
        solver.substeps --;
        if (solver.substeps < 1) solver.substeps = 1;
    } else if (solver.substeps < 10 && expectedFrameTime - averageFrameTime >= 0.5f * expectedFrameTime) {
        TraceLog(LOG_DEBUG, "Detected under utilization, increasing simulation substeps. Simulation time: %.2fms (%.2fms expected)", averageFrameTime, expectedFrameTime);
        solver.substeps ++;
        if (solver.substeps > 10) solver.substeps = 10;
    }

    frameCounter = 0;
    frameTimeSum = 0;
}