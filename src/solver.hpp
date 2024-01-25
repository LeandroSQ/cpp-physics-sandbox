#pragma once

#include "object.hpp"
#include "precomp.hpp"
#include "quadtree.hpp"

class Solver {
  private:
    Quadtree &quadtree;
    raylib::Vector2 centerCirclePosition;
	uint64_t iterationSum = 0;
	uint32_t iterationCount = 0;

    void applyGravity(std::shared_ptr<Object> object, float deltaTime);
    void applyConstraints(std::shared_ptr<Object> object, float deltaTime);
    void solveCollisions(std::shared_ptr<Object> object);

  public:
    float temperatureTransferContactMultiplier = 0.025f;
    float temperatureTransferAirMultiplier = 0.005f;
    float temperatureTransferGroundMultiplier = 0.25f;
    float temperatureFloatingForce = 3.0f;
    unsigned int substeps = 4;
    raylib::Vector2 gravity = { 0.0f, 9.8f };

    Solver(Quadtree &quadtree);

	void solve(float deltaTime);
    uint32_t getAverageIterations();
};