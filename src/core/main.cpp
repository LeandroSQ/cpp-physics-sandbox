#include "precomp.hpp"
#include "constants.hpp"
#include "app.hpp"

App app;


void render() {
    if (IsWindowResized()) app.resize();

	app.update();
	BeginDrawing();
	app.render();
	EndDrawing();
}

int main() {
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);

#ifdef PLATFORM_WEB
	InitWindow(WIDTH, HEIGHT, "Sandbox");
#else
	InitWindow(WIDTH, HEIGHT, "Sandbox");
#endif
	SetWindowMinSize(WIDTH, HEIGHT);

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