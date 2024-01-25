#include "precomp.hpp"
#include "app.hpp"

App app;

void render() {
    app.update();
	BeginDrawing();
	app.render();
	EndDrawing();
}

int main() {
	SetTargetFPS(TARGET_FPS);
    // SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(WIDTH, HEIGHT, "Fire!");

    app.setup();

#ifdef PLATFORM_WEB
	emscripten_set_main_loop(render, 0, 1);
#else

	while (!WindowShouldClose()) {
		render();
	}
#endif

    CloseWindow();

	return 0;
}