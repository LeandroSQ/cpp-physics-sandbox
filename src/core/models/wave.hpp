#pragma once

#include "../precomp.hpp"
#include "../settings.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include "../data/spatial-hash-grid.hpp"

class WaveController {
  public:
    float waveInterval = LEVEL_WAVE_INTERVAL;
    float spawnInterval = LEVEL_ASTEROID_SPAWN_INTERVAL;
    uint8_t asteroidsPerWave = LEVEL_ASTEROIDS_PER_WAVE;

    // Current wave
    uint16_t currentWave = 0;
    float timeSinceLastWave = 0.0f;
    float timeSinceLastSpawn = spawnInterval / 2.0f;
    uint16_t asteroidsSpawned = 0;

  public:
    WaveController() = default;
    ~WaveController() = default;

    void update(IContainer &asteroids, std::list<Bullet> &bullets, Ship &ship);

private:
    void spawn(IContainer &asteroids, std::list<Bullet> &bullets, Ship &ship);
};