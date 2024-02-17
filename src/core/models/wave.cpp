#include "wave.hpp"
#include "asteroid.hpp"
#include "app.hpp"
#include "raylib.h"
#include "soundfx.hpp"

WaveController::WaveController() {
}

WaveController::~WaveController() {
    TraceLog(LOG_INFO, "WaveController::~WaveController()");
    if (thread.joinable()) thread.join();
}

void WaveController::renderGUI() {
    ImGui::Text("Current: %i", currentWave);
    ImGui::Text("Time Since Last Wave: %.2f", timeSinceLastWave);
    ImGui::Text("Time Since Last Spawn: %.2f", timeSinceLastSpawn);
    ImGui::Text("Asteroids Spawned: %i", asteroidsSpawned);
}

void WaveController::spawn(std::shared_ptr<App> app) {
    const int maxTries = 4;
    const float radius = getRandomValue(0.7f, 1.0f) * ASTEROID_RADIUS;
    auto position = raylib::Vector2(
        GetRandomValue(0, WIDTH),
        GetRandomValue(0, HEIGHT)
    );

    bool hit = false;
    uint8_t tries = 0;
    do {
        // Ensures it did not spawn on top of the ship
        if (position.Distance(app->ship.position) < radius) {
            TraceLog(LOG_DEBUG, "Ship hit on %.2f, %.2f with distance %.3f - %.2f", position.x, position.y, app->ship.position.Distance(position), radius);
            hit = true;
        }

        // Ensures it did not spawn on top of another asteroid
        if (!hit && ASTEROIDS_SELF_COLLISION) {
            for (auto &other : app->grid.getAll()) {
                if (other->position.Distance(position) < other->radius + radius) {
                    TraceLog(LOG_DEBUG, "Asteroid hit on %.2f, %.2f", position.x, position.y);
                    hit = true;
                    break;
                }
            }
        }

        // Ensures it did not spawn on top of bullets
        if (!hit) {
            for (auto &bullet : app->bullets) {
                if (bullet.position.Distance(position) < radius + BULLET_RADIUS) {
                    TraceLog(LOG_DEBUG, "Bullet hit on %.2f, %.2f", position.x, position.y);
                    hit = true;
                    break;
                }
            }
        }

        // Assign new position if it hit something
        if (hit) {
            position = raylib::Vector2(
                GetRandomValue(0, WIDTH),
                GetRandomValue(0, HEIGHT)
            );
            tries++;
        }
    } while(hit && tries < maxTries);

    if (tries >= maxTries) {
        TraceLog(LOG_INFO, "Failed to spawn asteroid after %i tries", maxTries);
    } else {
        app->grid.insert(std::make_shared<Asteroid>(position, radius));
        SoundFX::play(SoundFX::spawn, 0.15f, -1.0f, 1.0f - position.x / (float)WIDTH);
    }
}

void WaveController::threadCallback() {
    bool once = true;
    auto lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    while (!app.expired()) {
        if (auto app = this->app.lock()) {
            // Ignore if grid is not initialized
            if (app->grid.rows <= 0 || app->grid.cols <= 0 || !app->grid.isInitialized()) continue;

            auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto deltaTime = (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;

            // Update timers
            timeSinceLastWave += deltaTime;
            timeSinceLastSpawn += deltaTime;

            // Update wave
            if (timeSinceLastWave >= waveInterval || (app->grid.isEmpty() && currentWave > 0)) {
                TraceLog(LOG_DEBUG, "Entering wave %i", currentWave);

                // Start new wave
                timeSinceLastWave -= waveInterval;
                currentWave++;

                asteroidsPerWave += LEVEL_ASTEROIDS_PER_WAVE;

                waveInterval -= waveInterval * LEVEL_WAVE_INTERVAL_DECREMENT;
                if (waveInterval < LEVEL_WAVE_INTERVAL_MIN) waveInterval = LEVEL_WAVE_INTERVAL_MIN;

                spawnInterval -= spawnInterval * LEVEL_ASTEROID_SPAWN_INTERVAL_DECREMENT_PER_WAVE;
                if (spawnInterval < LEVEL_ASTEROID_SPAWN_INTERVAL_MIN) spawnInterval = LEVEL_ASTEROID_SPAWN_INTERVAL_MIN;
            }

            // Spawn new asteroid
            if (timeSinceLastSpawn >= spawnInterval && asteroidsSpawned < asteroidsPerWave) {
                timeSinceLastSpawn -= spawnInterval;
                asteroidsSpawned++;
                spawn(app);
            }

            if (SPAWN_AT_START && once && app->grid.isEmpty()) {
                once = false;
                TraceLog(LOG_DEBUG, "Spawning initial asteroids");
                for (int i = 0; i < 50; i++) spawn(app);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds((long)(spawnInterval * 1000L)));
    }

    TraceLog(LOG_INFO, "WaveController::threadCallback() exited");
}

void WaveController::start(std::shared_ptr<App> app) {
    this->app = app;
    thread = std::thread(&WaveController::threadCallback, this);
    thread.detach();
}