#pragma once

#include "../precomp.hpp"
#include "bullet.hpp"
#include "ship.hpp"
#include "asteroid.hpp"
#include "wave.hpp"
#include "../data/spatial-hash-grid.hpp"

class App {
  private:
    raylib::RenderTexture2D frameBuffer;

    SpatialHashGrid asteroids;
    std::list<Bullet> bullets;
    float shootTimer;
    Ship ship;

    WaveController wave;

    void shoot();
    void updateAsteroids();
    void updateBullets();

  public:
    App() = default;
    ~App();

    void setup();
    void update();
    void render();
    void onResize(uint32_t width, uint32_t height);
    void renderGUI();
};