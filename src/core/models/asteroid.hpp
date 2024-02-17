#pragma once

#include "../data/icontainer.hpp"
#include "../precomp.hpp"
#include "Vector2.hpp"
#include "collision-handler.hpp"

class Asteroid : public CollisionHandler::Shape {
  public:
	static size_t idCounter;
	static uint32_t collisionCount;

	// Identifiers
	size_t id;
	uint16_t gridCellIndex = 0;
	raylib::Color color = raylib::Color::White();

  private:
	float innerRadius;

	// Animation
	float scale;

	// Physics
	raylib::Vector2 velocity;
	raylib::Vector2 lastPosition;
	float lastAngle;
	float angle;
	float angularVelocity;

	// Utility
	void updatePhysics(float deltaTime);
	void solveCollisions(IContainer &grid);
	void wrapAroundScreen();
	void updateAnimations();
    void updateVertices();
    void generateVertices(float radius, uint8_t vertexCount);

  public:
	Asteroid(raylib::Vector2 position, float radius);
	~Asteroid() = default;

	std::vector<std::shared_ptr<Asteroid>> split();
	void update(IContainer &others, float deltaTime);
	void render();
};