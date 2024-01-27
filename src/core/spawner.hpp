#pragma once

#include "precomp.hpp"
#include "quadtree.hpp"
#include "object.hpp"

class Spawner {
  private:
    Quadtree &quadtree;
    std::vector<std::shared_ptr<Object>> draggingObjects;

    float angle = 0.0f;
    float timer = 0.0f;
    bool isSpawning = true;

    void spawn();

  public:
    Spawner(Quadtree &quadtree);

    void update(float deltaTime, bool consumeInput);
};
