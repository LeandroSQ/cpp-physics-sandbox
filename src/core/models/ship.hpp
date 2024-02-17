#pragma once

#include "../precomp.hpp"
#include "bullet.hpp"

class Ship {
  public:
    std::array<raylib::Vector2, 3> vertices;
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    raylib::Vector2 acceleration;
    float angle;
    float angularVelocity;

  private:
    float thrust;
    float shotCooldown;

    void updateVertices();
    void updatePhysics();
    void updateInput(std::list<Bullet> &bullets);
    void wrapAroundScreen();
    void updateShootingLogic(std::list<Bullet> &bullets);
    void shoot(std::list<Bullet> &bullets);

    void updateTrail();

    void updateThrustSound();

  public:
    Ship() = default;
    ~Ship() = default;

    void setup();
    void update(std::list<Bullet> &bullets);
    void render();
};