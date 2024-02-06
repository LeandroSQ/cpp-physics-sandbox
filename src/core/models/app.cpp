#include "app.hpp"
#include "../settings.hpp"
#include "RenderTexture.hpp"
#include "asteroid.hpp"
#include "imgui.h"
#include "polygon.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

App::~App() {
    TraceLog(LOG_INFO, "App::~App()");
    rlImGuiShutdown();
}

void App::shoot() {
    Bullet bullet {
        ship.position,
        raylib::Vector2(cosf(ship.angle), sinf(ship.angle)) * BULLET_VELOCITY
    };
    bullets.push_back(bullet);
}

void App::setup() {
#ifdef DEBUG
    SetTraceLogLevel(LOG_DEBUG);
#endif

    TraceLog(LOG_INFO, "App::setup()");

    rlImGuiSetup(true);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ship.setup();
}

void App::updateAsteroids() {
    Asteroid::collisionCount = 0;
    for (auto &asteroid : asteroids.all()) asteroid->update(asteroids);

    asteroids.update();
}

void App::updateBullets() {
    if (shootTimer > BULLET_SHOOT_INTERVAL) shootTimer = BULLET_SHOOT_INTERVAL;
    else shootTimer += GetFrameTime();
    if (shootTimer >= BULLET_SHOOT_INTERVAL && IsKeyDown(KEY_SPACE)) {
        shoot();
        shootTimer -= BULLET_SHOOT_INTERVAL;
    }

    for (auto it = bullets.begin(); it != bullets.end();) {
        if (it->update(asteroids)) {
            it = bullets.erase(it);
        } else {
            it++;
        }
    }
}

void App::update() {
    updateAsteroids();
    updateBullets();
    ship.update();
    wave.update(asteroids, bullets, ship);
}

void App::render() {
    frameBuffer.BeginMode();
    ClearBackground(BLACK);

    for (auto &asteroid : asteroids.all()) asteroid->render();

    for (auto &bullet : bullets) bullet.render();

    ship.render();

    // Render grid
    asteroids.render();

    frameBuffer.EndMode();
}

void App::onResize(uint32_t width, uint32_t height) {
    TraceLog(LOG_INFO, "App::onResize(%i, %i)", width, height);
    frameBuffer = raylib::RenderTexture2D(width, height);

    // Update global settings
    WIDTH = width;
    HEIGHT = height;

    // Update the grid
    const auto size = (float)ASTEROID_RADIUS * 4;
    asteroids.resize(ceilf(HEIGHT / size), ceilf(WIDTH / size));
}
