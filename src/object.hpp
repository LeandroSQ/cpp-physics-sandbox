#pragma once

#include "precomp.hpp"
#include "quadtree.hpp"

class Quadtree;

class Object {
  private:
	raylib::Vector2 previousPosition;

  public:
	raylib::Vector2 position;
	raylib::Vector2 acceleration;

    Quadtree *quadrant = nullptr;

    float mass;
    float radius;
    float temperature;

    raylib::Color color = WHITE;

    Object(raylib::Vector2 position, raylib::Vector2 acceleration, float mass, float radius, float temperature, raylib::Color color);
    Object();

    void update(float deltaTime);
    void render();

    raylib::Rectangle getBounds();
};