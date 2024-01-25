#pragma once

#include "precomp.hpp"
#include "quadtree.hpp"
#include "object.hpp"

class Spawner {
  private:
    Quadtree &quadtree;

    float angle = 0.0f;
    float timer = 0.0f;
    bool isSpawning = true;

    void spawn();

  public:
    Spawner(Quadtree &quadtree);

    void update(float deltaTime);
};
