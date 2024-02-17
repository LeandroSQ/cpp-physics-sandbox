#pragma once

#include "../precomp.hpp"
#include "../settings.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include "../data/spatial-hash-grid.hpp"

class App;

class WaveController {
  private:
    std::weak_ptr<App> app;

    float waveInterval = LEVEL_WAVE_INTERVAL;
    float spawnInterval = LEVEL_ASTEROID_SPAWN_INTERVAL;
    uint8_t asteroidsPerWave = LEVEL_ASTEROIDS_PER_WAVE;

    // Current wave
    uint16_t currentWave = 0;
    float timeSinceLastWave = 0.0f;
    float timeSinceLastSpawn = spawnInterval / 2.0f;
    uint16_t asteroidsSpawned = 0;

    // Threading
    std::thread thread;

    void spawn(std::shared_ptr<App> app);
    void threadCallback();

  public:
    WaveController();
    ~WaveController();

    void renderGUI();
    void start(std::shared_ptr<App> app);
};