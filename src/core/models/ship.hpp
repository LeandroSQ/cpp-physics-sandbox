#pragma once

#include "../precomp.hpp"
#include "Vector2.hpp"

class Ship {
  public:
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    raylib::Vector2 acceleration;
    float angle;
    float angularVelocity;

  private:
    void updatePhysics();
    void updateInput();
    void wrapAroundScreen();

  public:
    Ship() = default;
    ~Ship() = default;

    void setup();
    void update();
    void render();
};