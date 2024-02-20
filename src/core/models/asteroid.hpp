#pragma once

#include "../precomp.hpp"
#include "polygon.hpp"
#include "../data/icontainer.hpp"

class Asteroid {

  public:
    static size_t idCounter;
    static uint32_t collisionCount;

    uint32_t index;
    size_t id;

    Polygon polygon;
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    float angle;
    float angularVelocity;
    raylib::Color color = raylib::Color::White();

    // Animation
    float scale;

    void updatePhysics();
    void wrapAroundScreen();

  public:
    Asteroid();
    Asteroid(Polygon &&polygon, raylib::Vector2 position);
    ~Asteroid() = default;

    void update(IContainer &others);
    void render();

  private:
    void checkForCollisions(IContainer &others);
    void updateAnimations();
};