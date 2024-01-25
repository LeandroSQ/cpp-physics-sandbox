#include "app.hpp"
#include <rlImGui.h>

App::App() : quadtree(raylib::Rectangle{ 0.0f, 0.0f, (float)WIDTH, (float)HEIGHT }), solver(quadtree), spawner(quadtree) { }

App::~App() {
    rlImGuiShutdown();
}

void App::handleInput(float delta) {
	// Rotate gravity vector
	const float angleVelocity = 1.25f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
		solver.gravity = solver.gravity.Rotate(angleVelocity * delta);
	}
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		solver.gravity = solver.gravity.Rotate(-angleVelocity * delta);
	}

	// Increment/decrement gravity force
	const auto steps = raylib::Vector2::One() * 1.0f * delta;
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



    rlImGuiEnd();

    /* // Stats
	DrawText(TextFormat("%i FPS", GetFPS()), 10, 10, 20, PALETTE_WHITE);
	DrawText(TextFormat("%.2f Frame time", GetFrameTime() * 1000.0f), 10, 30, 20, PALETTE_WHITE);
	DrawText(TextFormat("%i Objects", quadtree.getAll().size()), 10, 50, 20, PALETTE_WHITE);
	DrawText(TextFormat("%i Subdivisions", quadtree.getSubdivisions()), 10, 70, 20, PALETTE_WHITE);
	DrawText(TextFormat("%i Collision checks", solver.getAverageIterations()), 10, 90, 20, PALETTE_WHITE);

    // Draw gravity indicator
    constexpr float radius = 15.0f;
    constexpr float arrowAngle = 30.0f * DEG2RAD;
    constexpr float arrowLength = radius / 2.0f;
    const auto vec = solver.gravity.Normalize() * radius * 0.8f;
    const auto center = raylib::Vector2(10.0f + radius, 115.0f + radius);
    DrawCircleV(center, radius, PALETTE_WHITE);
    DrawLineEx(center, center + vec, 2.0f, PALETTE_RED);
    DrawLineEx(center + vec, center + vec - solver.gravity.Normalize().Rotate(arrowAngle) * arrowLength, 1.0f, PALETTE_RED);
    DrawLineEx(center + vec, center + vec - solver.gravity.Normalize().Rotate(-arrowAngle) * arrowLength, 1.0f, PALETTE_RED);
    DrawText(TextFormat("Gravity { %.2f, %.2f }", solver.gravity.x, solver.gravity.y), center.x + radius + 10, 110.0f + radius - 5, 20, PALETTE_WHITE); */
}

void App::setup() {
#ifdef DEBUG
	SetTraceLogLevel(LOG_DEBUG);
#endif

	TraceLog(LOG_INFO, "app: Starting...");
	SetRandomSeed(GetTime());

    rlImGuiSetup(true);
}

void App::update() {
	auto delta = GetFrameTime();
	spawner.update(delta);
	solver.solve(delta);

    handleInput(delta);
}

void App::render() {
#ifdef CENTER_CIRCLE
	ClearBackground(PALETTE_GREY);
	DrawCircle(WIDTH / 2, HEIGHT / 2, 400.0f, PALETTE_BLACK);
#else
	ClearBackground(PALETTE_BLACK);
#endif

	if (isRenderingQuadtree) {
		quadtree.render();
	} else {
		auto all = quadtree.getAll();
		for (auto object : all) {
			object->render();
		}
	}

	renderGUI();
}
