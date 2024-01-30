#pragma once

#include "Vector2.hpp"
#include "precomp.hpp"
#include "quadtree.hpp"
#include "object.hpp"

class InterationHandler {
  private:
    Quadtree &quadtree;
    std::vector<std::shared_ptr<Object>> draggingObjects;

    float angle = 0.0f;
    float timer = 0.0f;
    bool isSpawning = true;

    void spawn(raylib::Vector2 mouse);

  public:
    InterationHandler(Quadtree &quadtree);

    void update(raylib::Vector2 mouse, float deltaTime, bool consumeInput);
};
