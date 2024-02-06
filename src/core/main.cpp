#include "precomp.hpp"
#include "raylib.h"
#include "settings.hpp"
#include "models/app.hpp"

App app;

void render() {
	BeginDrawing();
    ClearBackground(BLACK);
    app.renderGUI();
	EndDrawing();

	app.update();
    app.render();
}

int main() {
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);

#ifdef PLATFORM_WEB
	InitWindow(WIDTH, HEIGHT, "Asteroids");
#else
    const int monitor = GetCurrentMonitor();
    if (monitor > 0) {
        WIDTH = GetMonitorWidth(monitor) / 2.0f;
        HEIGHT = GetMonitorHeight(monitor) / 2.0f;
        TARGET_FPS = GetMonitorRefreshRate(monitor);

        TraceLog(LOG_INFO, "Monitor: %i", monitor);
        TraceLog(LOG_INFO, "Viewport: %i, %i", WIDTH, HEIGHT);
        TraceLog(LOG_INFO, "Refresh Rate: %i", TARGET_FPS);
    }

	InitWindow(WIDTH, HEIGHT, "Asteroids");
	SetWindowMinSize(WIDTH, HEIGHT);
#endif

	app.setup();

#ifdef PLATFORM_WEB
	emscripten_set_main_loop(render, 0, 1);
#else
	SetTargetFPS(TARGET_FPS);

	while (!WindowShouldClose()) {
		render();
	}
#endif

	CloseWindow();

	return 0;
}