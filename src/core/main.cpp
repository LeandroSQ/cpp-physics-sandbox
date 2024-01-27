#include "app.hpp"
#include "precomp.hpp"

App app;

#ifndef PLATFORM_WEB
  bool resized = false;
#endif

void render() {
#ifdef PLATFORM_WEB
	if (IsWindowResized()) {
		app.resize();
	}
#else
	if (!resized && IsWindowResized()) {
		// auto size = std::max(GetScreenWidth(), GetScreenHeight());
		// SetWindowSize(size, size);
		app.resize();
		resized = true;
	} else {
		resized = false;
	}
#endif

	app.update();
	BeginDrawing();
	app.render();
	EndDrawing();
}

int main() {
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

	InitWindow(WIDTH, HEIGHT, "Sandbox");
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