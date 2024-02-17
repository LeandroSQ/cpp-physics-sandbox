#pragma once

#include "../data/spatial-hash-grid.hpp"
#include "../precomp.hpp"

class Bullet {
  private:
	raylib::Vector2 velocity;
    float timeAlive;

	void updatePhysics();
	bool solveCollisions(IContainer &grid);

  public:
	raylib::Vector2 position;

    Bullet(raylib::Vector2 position, raylib::Vector2 velocity);
    ~Bullet() = default;

	bool update(IContainer &asteroids);
	void render();
};