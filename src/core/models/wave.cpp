#include "wave.hpp"
#include "asteroid.hpp"

void WaveController::update(IContainer &asteroids, std::list<Bullet> &bullets, Ship &ship) {
    timeSinceLastWave += GetFrameTime();
    timeSinceLastSpawn += GetFrameTime();

    // if (asteroids.isEmpty()) {
    //     for (int i = 0; i < 50; i++) {
    //         spawn(asteroids, bullets, ship);
    //     }
    // }

    if (timeSinceLastWave >= waveInterval || (asteroids.isEmpty() && currentWave > 0)) {
        TraceLog(LOG_DEBUG, "Entering wave %i", currentWave);

        // Start new wave
        timeSinceLastWave -= waveInterval;
        currentWave++;

        asteroidsPerWave += LEVEL_ASTEROIDS_PER_WAVE;

        waveInterval *= LEVEL_WAVE_INTERVAL_DECREMENT;
        if (waveInterval < LEVEL_WAVE_INTERVAL_MIN) waveInterval = LEVEL_WAVE_INTERVAL_MIN;

        spawnInterval *= LEVEL_ASTEROID_SPAWN_INTERVAL_DECREMENT_PER_WAVE;
        if (spawnInterval < LEVEL_ASTEROID_SPAWN_INTERVAL_MIN) spawnInterval = LEVEL_ASTEROID_SPAWN_INTERVAL_MIN;
    }

    if (timeSinceLastSpawn >= spawnInterval && asteroidsSpawned < asteroidsPerWave) {
        // Spawn new asteroid
        timeSinceLastSpawn -= spawnInterval;
        asteroidsSpawned++;
        spawn(asteroids, bullets, ship);
    }

    if (IsKeyReleased(KEY_ENTER)) {
        spawn(asteroids, bullets, ship);
    }
}

void WaveController::spawn(IContainer &asteroids, std::list<Bullet> &bullets, Ship &ship) {
    const auto asteroid = std::make_shared<Asteroid>();

    bool hit = false;
    uint8_t tries = 0;
    do {
        // Ensures it did not spawn on top of the ship
        if (asteroid->position.Distance(ship.position) < asteroid->polygon.outerRadius) {
            TraceLog(LOG_DEBUG, "Ship hit on %.2f, %.2f with distance %.3f - %.2f", asteroid->position.x, asteroid->position.y, ship.position.Distance(asteroid->position), asteroid->polygon.outerRadius);
            hit = true;
        }

        // Ensures it did not spawn on top of another asteroid
        if (!hit) {
            for (auto &other : asteroids.all()) {
                if (&other == &asteroid) continue;
                if (other->position.Distance(asteroid->position) < other->polygon.outerRadius + asteroid->polygon.outerRadius) {
                    TraceLog(LOG_DEBUG, "Asteroid hit on %.2f, %.2f", asteroid->position.x, asteroid->position.y);
                    hit = true;
                    break;
                }
            }
        }

        // Ensures it did not spawn on top of bullets
        if (!hit) {
            for (auto &bullet : bullets) {
                if (bullet.position.Distance(asteroid->position) < asteroid->polygon.outerRadius + BULLET_RADIUS) {
                    TraceLog(LOG_DEBUG, "Bullet hit on %.2f, %.2f", asteroid->position.x, asteroid->position.y);
                    hit = true;
                    break;
                }
            }
        }

        // Assign new position if it hit something
        if (hit) {
            asteroid->position = raylib::Vector2(
                GetRandomValue(0, WIDTH),
                GetRandomValue(0, HEIGHT)
            );
            tries++;
        }
    } while(hit && tries < 10);

    if (tries >= 10) {
        TraceLog(LOG_INFO, "Failed to spawn asteroid after 10 tries");
    } else {
        asteroids.insert(asteroid);
    }
}