#include "object.hpp"
#include "precomp.hpp"
#include "quadtree.hpp"
#include "solver.hpp"

Quadtree quadtree(raylib::Rectangle{ 0.0f, 0.0f, (float)WIDTH, (float)HEIGHT });
Solver solver;
float spawnTimer = 0.0f;
float angle = 90.0f * DEG2RAD;

float random(float min, float max) {
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void spawn() {
	auto obj = std::make_shared<Object>(
		raylib::Vector2{ GetScreenWidth() / 2.0f, 75.0f },
		// raylib::Vector2 { random(-10.0f, 10.0f), 0.0f },
		raylib::Vector2{ cos(angle) * 1000000.0f, sin(angle) * 1000000.0f },
		0.0f,
        10.0f,
		// random(2.0f, 15.0f),
		0.0f,
		WHITE
	);
	quadtree.add(obj);
}

void update() {
	uint8_t substeps = 8;
	float dt = GetFrameTime() / float(substeps);

	angle = sin(GetTime()) * 0.25f * PI + 0.5f * PI;

	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		spawn();
	else if (true || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		if (spawnTimer >= SPAWN_INTERVAL && quadtree.size() < SPAWN_COUNT) {
			spawnTimer -= SPAWN_INTERVAL;
			spawn();
		} else {
			spawnTimer += GetFrameTime();
		}
	} else {
		spawnTimer = SPAWN_INTERVAL;
	}

	for (uint8_t i = 0; i < substeps; i++) {
		solver.solve(quadtree, dt);
	}
}

void render(raylib::Window &window) {
	update();

	BeginDrawing();
	window.ClearBackground(GRAY);

	// Draw bigger circle on the center with 200f radius
	DrawCircle(WIDTH / 2, HEIGHT / 2, 400.0f, BLACK);

	// Render
	// for (auto object : quadtree.getAll()) object->render();

	// Debug quadtree
	quadtree.render();

	// Stats
	DrawText(TextFormat("%i FPS", GetFPS()), 10, 10, 20, WHITE);
	DrawText(TextFormat("%.2f Frame time", GetFrameTime() * 1000.0f), 10, 30, 20, WHITE);
	DrawText(TextFormat("%i Objects", quadtree.getAll().size()), 10, 50, 20, WHITE);
	DrawText(TextFormat("%i Subdivisions", quadtree.getSubdivisions()), 10, 70, 20, WHITE);
	DrawText(TextFormat("%i Collision checks", (solver.iterationSum / solver.iterationCount)), 10, 90, 20, WHITE);

	EndDrawing();
}

void setup() {
#ifdef DEBUG
	SetTraceLogLevel(LOG_DEBUG);
#endif

#ifdef MULTI_THREADING
	omp_set_num_threads(omp_get_max_threads());
#endif

	TraceLog(LOG_INFO, "main: Starting...");
	SetRandomSeed(GetTime());
	SetConfigFlags(FLAG_MSAA_4X_HINT);
}

int main() {
	setup();
    raylib::Window window(WIDTH, HEIGHT, "Fire!");

#ifdef PLATFORM_WEB
	emscripten_set_main_loop(render, 0, 1);
#else
	SetTargetFPS(TARGET_FPS);

	while (!window.ShouldClose()) {
		render(window);
	}
#endif

	return 0;
}