#include "app.hpp"
#include "../settings.hpp"
#include "Shader.hpp"
#include "Vector2.hpp"
#include "asteroid.hpp"
#include "particle.hpp"
#include "raylib.h"
#include "raymath.h"
#include "soundfx.hpp"
#include "wave.hpp"
#include <memory>
#include <sys/_types/_intptr_t.h>

App::App(): starsShader("assets/shaders/stars.fs"), particlesShader("assets/shaders/particles.fs"), allShader("assets/shaders/all.fs"), backgroundShader("assets/shaders/background.fs") {
    TraceLog(LOG_INFO, "App::App()");

    // Setup camera
    camera.offset = raylib::Vector2::Zero();
    camera.target = raylib::Vector2::Zero();
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
}

App::~App() {
    TraceLog(LOG_INFO, "App::~App()");
    rlImGuiShutdown();
    SoundFX::release();
    CloseAudioDevice();
    CloseWindow();
}

void App::setup() {
#ifdef DEBUG
    SetTraceLogLevel(LOG_DEBUG);
#endif
    TraceLog(LOG_INFO, "App::setup()");

    setupWindow();

    // Shaders
    backgroundShader.load();
    particlesShader.load();
    starsShader.load();
    allShader.load();

    // Setup uniforms
    uniformAberrationLocation = allShader.getUniformLocation("aberration");
    uniformAberrationValue = 0.0f;
    uniformResolutionLocation = backgroundShader.getUniformLocation("resolution");
    uniformResolutionValue = raylib::Vector2((float)WIDTH, (float)HEIGHT);

    // Misc
    setupGUI();
    wave.start(shared_from_this());
    SoundFX::setup();
}

void App::setupWindow() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
#ifdef DEBUG
    SetConfigFlags(FLAG_WINDOW_TOPMOST);
#endif

#ifdef PLATFORM_WEB
	InitWindow(WIDTH, HEIGHT, "Asteroids");
#else
    const int monitor = GetCurrentMonitor();
    const int monitorCount = GetMonitorCount();
    TraceLog(LOG_INFO, "Main::main() - Monitor: %i of %i", monitor, monitorCount);
    if (monitor >= 0 && monitorCount > 0) {
        WIDTH = GetMonitorWidth(monitor) / 2.0f;
        HEIGHT = GetMonitorHeight(monitor) / 2.0f;
        TARGET_FPS = GetMonitorRefreshRate(monitor);

        TraceLog(LOG_INFO, "Main::main() - Monitor id: %i", monitor);
        TraceLog(LOG_INFO, "Main::main() - Monitor viewport: %ix%i", WIDTH, HEIGHT);
        TraceLog(LOG_INFO, "Main::main() - Monitor refresh Rate: %i", TARGET_FPS);
    }

	InitWindow(WIDTH, HEIGHT, "Asteroids");
	SetWindowMinSize(WIDTH, HEIGHT);
#endif

    InitAudioDevice();
}

void App::run() {
#ifdef PLATFORM_WEB
	emscripten_set_main_loop(loop, 0, 1);
#else
	SetTargetFPS(TARGET_FPS);
	while (!WindowShouldClose()) {
		loop();
	}
#endif
}

void App::setupStars() {
    const size_t count = 150;

    stars.clear();
    stars.reserve(count);

    for (size_t i = 0; i < count; i++) {
        stars.emplace_back(
            raylib::Vector2(
                getRandomValue(0, WIDTH),
                getRandomValue(0, HEIGHT)
            ),
            getRandomValue(0.75f, 4.5f) * 1.25f
        );
    }
}

void App::updateAsteroids() {
    Asteroid::collisionCount = 0;
    const int substeps = 1;
    const float dt = GetFrameTime() / substeps;

    for (auto &asteroid : grid.getAll()) asteroid->update(grid, dt);

    grid.update();
}

void App::updateBullets() {
    for (auto it = bullets.begin(); it != bullets.end();) {
        if (it->update(grid)) {
            shake(0.25f);
            it = bullets.erase(it);
        } else {
            it++;
        }
    }
}

void App::updateStars(raylib::Vector2 &offset) {
	if (fabsf(offset.x) < WIDTH / 2.0f && fabsf(offset.y) < HEIGHT / 2.0f) {
		for (auto &star : stars) {
			star.update(offset);
		}
	}
}

void App::updateScreenShake() {

    if (IsKeyDown(KEY_ENTER)) trauma += 0.15f;
    if (trauma >= 1.0f) trauma = 1.0f;

    if (trauma <= EPSILON) return;

    const float traumaPower = powf(trauma, 2.0f);
    const auto center = raylib::Vector2(WIDTH / 2.0f, HEIGHT / 2.0f);
    cameraShakeOffset = raylib::Vector2(GetRandomValue(-1, 1), GetRandomValue(-1, 1)) * traumaPower * 25.5f;
    cameraShakeRotation = GetRandomValue(-1, 1) * traumaPower * 15.5f;

    camera.offset = Vector2Lerp(camera.offset, cameraShakeOffset + center, GetFrameTime() * 25.0f);
    camera.rotation = Lerp(camera.rotation, cameraShakeRotation, GetFrameTime() * 10.0f);
    camera.zoom = Lerp(camera.zoom, /* 1.0f + */ 1.0f - traumaPower * 0.1f, GetFrameTime() * 10.0f);

    uniformAberrationValue = traumaPower * 5.0f;
    allShader.setUniform(uniformAberrationLocation, &uniformAberrationValue, ShaderUniformDataType::SHADER_UNIFORM_FLOAT);

    trauma *= 0.92f;
    if (trauma <= EPSILON) {
        trauma = 0.0f;
        camera.offset = center;
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }
}

void App::shake(float amount) {
    trauma += amount;
    if (trauma >= 1.0f) trauma = 1.0f;
}

void App::loop() {
    BeginDrawing();
    ClearBackground(BLACK);
    renderGUI();
    EndDrawing();

    update();
    render();
}

static long lastShaderUpdateTime = 0;

void App::update() {
	profiler.onFrameStart();
    updateScreenShake();
    updateAsteroids();
    updateBullets();
    ParticleSystem::update();

    auto offset = ship.position;
    ship.update(bullets);
    offset = ship.position - offset;
	updateStars(offset);

#ifdef DEBUG
    backgroundShader.autoReloadIfNeeded();
    particlesShader.autoReloadIfNeeded();
    starsShader.autoReloadIfNeeded();
    allShader.autoReloadIfNeeded();

    backgroundShader.setUniform(uniformResolutionLocation, &uniformResolutionValue, ShaderUniformDataType::SHADER_UNIFORM_VEC2);
#endif

	// TODO: Research bout the spaceship that appears sometimes, is it an enemy?

    // TODO: Move to function, and maybe ship
    // TODO: Improve particles
    // const auto reflectedAngle = ship.angle + PI;
    // const auto shipDirection = raylib::Vector2(cosf(reflectedAngle), sinf(reflectedAngle));
    // const auto shipBottom = ship.position + shipDirection * 10.0f;
    // const int count = 5;
    // for (int i = 0; i < count; i++) {
    //     const float angle = (180.0f / count * i) * DEG2RAD + ship.angle + PI / 2.0f + getRandomValue(-0.1f, 0.1f);
    //     const auto direction = raylib::Vector2(cosf(angle), sinf(angle));
    //     const auto position = shipBottom;
    //     const auto velocity = direction * ship.velocity;
    //     particles.emplace_back(
    //         position,
    //         velocity,
    //         LIGHTGRAY,
    //         2.0f,
    //         0.25f
    //     );
    // }
}

void App::render() {
    if (!frameBuffer.IsReady() || !tempFrameBuffer.IsReady()) {
        TraceLog(LOG_WARNING, "App::render() - Frame buffer not ready");
        return;
    }

    // First pass is drawing the stars to a temporary frame buffer
    tempFrameBuffer.BeginMode();
    ClearBackground(raylib::Color(0, 0, 0, 0));

    tempFrameBuffer.EndMode();

    // Second pass is drawing the game to the main frame buffer
    frameBuffer.BeginMode();

    // Render background
    backgroundShader.beginMode();
    DrawRectangle(0, 0, WIDTH, HEIGHT, WHITE);
    backgroundShader.endMode();

    camera.BeginMode();

    // Render stars
    starsShader.beginMode();
    tempFrameBuffer.GetTexture().Draw(0, 0, WHITE);
    starsShader.endMode();

    for (auto &star : stars) star.render();

    // DEBUG: Render debug grid overlay
    if (RENDER_GRID) grid.render();

    // Render asteroids
    for (auto &asteroid : grid.getAll()) asteroid->render();

    // Render bullets
    for (auto &bullet : bullets) bullet.render();

    // Render particles
    particlesShader.beginMode();
    ParticleSystem::render();
    particlesShader.endMode();

    camera.EndMode();
    frameBuffer.EndMode();

    // Third pass is drawing the frame buffer to the screen
    tempFrameBuffer.BeginMode();
    allShader.beginMode();
    frameBuffer.GetTexture().Draw(0, 0, WHITE);
    allShader.endMode();
    // Render space ship
    ship.render();
    tempFrameBuffer.EndMode();

    profiler.onFrameEnd();
}

void App::onResize(uint32_t width, uint32_t height) {
    TraceLog(LOG_INFO, "App::onResize(%i, %i)", width, height);
    frameBuffer = raylib::RenderTexture2D(width, height);
    tempFrameBuffer = raylib::RenderTexture2D(width, height);

    // Update global settings
    WIDTH = width;
    HEIGHT = height;

    // Update the grid
    const auto size = (float)ASTEROID_RADIUS * 4;
    grid.resize(ceilf(height / size), ceilf(width / size));

    // Update the camera
    camera.offset = raylib::Vector2(width / 2.0f, height / 2.0f);
    camera.target = raylib::Vector2(width / 2.0f, height / 2.0f);

    // Update the uniforms
    uniformResolutionValue = raylib::Vector2((float)width, (float)height);
    backgroundShader.setUniform(uniformResolutionLocation, &uniformResolutionValue, ShaderUniformDataType::SHADER_UNIFORM_VEC2);

    // Update the ship
    if (!isViewportDefined) {
        isViewportDefined = true;
        ship.setup();
    }

    setupStars();
}
