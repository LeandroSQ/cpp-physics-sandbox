#pragma once

#include "../precomp.hpp"
#include "quadtree.hpp"
#include "object.hpp"

class InterationHandler {
  private:
    Quadtree &quadtree;
    std::vector<std::shared_ptr<Object>> draggingObjects;

    float angle = 90.0f * DEG2RAD;
    float timer = 0.0f;

    void spawn(raylib::Vector2 mouse, uint8_t substeps);

  public:
    bool isSpawning = true;

    InterationHandler(Quadtree &quadtree);

    void update(raylib::Vector2 mouse, uint8_t substeps, float deltaTime, bool consumeInput);
};
