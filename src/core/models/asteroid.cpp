#include "asteroid.hpp"
#include "../utils.hpp"
#include "Vector2.hpp"
#include "collision-handler.hpp"
#include "raylib.h"
#include "raymath.h"
#include <array>
#include <vector>

// Static
size_t Asteroid::idCounter = 0;
uint32_t Asteroid::collisionCount = 0;

// Utility
void Asteroid::updatePhysics(float deltaTime) {
	position += velocity * deltaTime;
	angle += angularVelocity * deltaTime;
}

void Asteroid::wrapAroundScreen() {
    const float padding = radius * 3;

	if (position.x - padding > WIDTH) {
		position.x = -padding;
	} else if (position.x < -padding) {
		position.x = WIDTH + padding;
	}

	if (position.y - padding > HEIGHT) {
		position.y = -padding;
	} else if (position.y < -padding) {
		position.y = HEIGHT + padding;
	}
}

void Asteroid::solveCollisions(IContainer &grid) {
    if (!ASTEROIDS_SELF_COLLISION) return;

	const auto others = grid.retrieve(position, radius * 2);
	for (auto &other : others) {
		if (id == other->id) continue;
        if (fabsf(radius - other->radius) > 7.5f) continue;

        Asteroid::collisionCount++;

		// Time to bring out the big guns
		const auto contact = CollisionHandler::getCollisionInfo(*this, *other.get());
		if (contact.has_value()) {
			// Move the asteroids apart
			const auto correction = contact->normal * contact->penetration;
			position -= correction / 2.0f;
			other->position += correction / 2.0f;

			// Calculate the new velocities
			const auto relativeVelocity = velocity - other->velocity;
			const auto velocityAlongNormal = relativeVelocity.DotProduct(contact->normal);

			// Use asteroid.radius / ASTEROID_RADIUS for mass
			const float j = -(1 + ASTEROID_RESTITUTION) * velocityAlongNormal;
			const float mass1 = 1.0f + radius / ASTEROID_RADIUS;
			const float mass2 = 1.0f + other->radius / ASTEROID_RADIUS;
			const float impulse = j / (mass1 + mass2);
			velocity += contact->normal * impulse * mass1;
			other->velocity -= contact->normal * impulse * mass2;

			// Ensure the asteroids are not stationary
			if (velocity.Length() < EPSILON) velocity = raylib::Vector2::One() * getRandomValue(-1.0f, 1.0f) * ASTEROID_VELOCITY;
			if (other->velocity.Length() < EPSILON) other->velocity = raylib::Vector2::One() * getRandomValue(-1.0f, 1.0f) * ASTEROID_VELOCITY;

			// Ensure the asteroids are not too fast
			if (velocity.Length() > ASTEROID_VELOCITY) velocity = velocity.Normalize() * ASTEROID_VELOCITY;
			if (other->velocity.Length() > ASTEROID_VELOCITY) other->velocity = other->velocity.Normalize() * ASTEROID_VELOCITY;

			// Calculate the new angular velocities
			const float angularVelocityAlongNormal = angularVelocity - other->angularVelocity;
			const float torque = angularVelocityAlongNormal * mass1;
			const float angularImpulse = torque / (mass1 + mass2);
			angularVelocity -= angularImpulse * mass1;
			other->angularVelocity += angularImpulse * mass2;

			// Ensure the asteroids are not stationary
			if (fabsf(angularVelocity) < 0.1f) angularVelocity = getRandomValue(-1.0f, 1.0f) * ASTEROID_ANGULAR_VELOCITY;
			if (fabsf(other->angularVelocity) < 0.1f) other->angularVelocity = getRandomValue(-1.0f, 1.0f) * ASTEROID_ANGULAR_VELOCITY;

			// Ensure the asteroids are not too fast
			if (fabsf(angularVelocity) > ASTEROID_ANGULAR_VELOCITY) angularVelocity = ASTEROID_ANGULAR_VELOCITY * (angularVelocity < 0 ? -1 : 1);
			if (fabsf(other->angularVelocity) > ASTEROID_ANGULAR_VELOCITY) other->angularVelocity = ASTEROID_ANGULAR_VELOCITY * (other->angularVelocity < 0 ? -1 : 1);
		}
	}
}

void Asteroid::updateAnimations() {
    float animationSpeed = 2.0f; // 500ms

    if (radius < ASTEROID_RADIUS) {
        animationSpeed = 4.0f; // 250ms
    }

	// Scale up the asteroid
	scale += GetFrameTime() * radius; // 500ms
	if (scale > 1.0f) scale = 1.0f;
}

void Asteroid::updateVertices() {
    // After updating the position and angle, update the vertices of the asteroid
    // Translating and rotating the vertices but keeping the same shape
    const float diffAngle = angle - lastAngle;
    for (size_t i = 0; i < vertices.size(); i++) {
        // Translate the vertex to the new position
        vertices[i] -= lastPosition;

        // Rotate the vertex around the center of the asteroid
        vertices[i] = vertices[i].Rotate(diffAngle);
        vertices[i] += position;
    }


    lastPosition = position;
    lastAngle = angle;
}

void Asteroid::generateVertices(float radius, uint8_t vertexCount) {
	this->radius = 0.0f;
    innerRadius = 0.0f;

    float scale = radius / ASTEROID_RADIUS;
    for (uint8_t i = 0; i < vertexCount; i++) {
        float angle = (i / (float)vertexCount) * 2.0f * PI;

        float r = radius + GetRandomValue(-ASTEROID_JAGGEDNESS * scale, ASTEROID_JAGGEDNESS * scale);

        // Keep track of the largest radius
        if (this->radius == 0.0f || r > this->radius) this->radius = r;

        // Keep track of the smallest radius
        if (innerRadius == 0.0f || r < innerRadius) innerRadius = r;

        vertices.emplace_back(position.x + cosf(angle) * r, position.y + sinf(angle) * r);
    }
}

// Public API
Asteroid::Asteroid(raylib::Vector2 position, float radius) {
	id = Asteroid::idCounter++;
	this->position = position;
	generateVertices(radius, GetRandomValue(ASTEROID_MIN_VERTEX_COUNT, ASTEROID_MAX_VERTEX_COUNT));

	// Generate random values
	velocity = raylib::Vector2::One() * getRandomValue(-1.0f, 1.0f) * ASTEROID_VELOCITY;
	angularVelocity = getRandomValue(-1.0f, 1.0f) * ASTEROID_ANGULAR_VELOCITY;

    // Store the initial position and angle
    lastPosition = position;
    lastAngle = angle;
}

std::vector<std::shared_ptr<Asteroid>> Asteroid::split() {
	std::vector<std::shared_ptr<Asteroid>> newAsteroids;

    // Split the asteroid into two
    if (radius >= ASTEROID_MIN_RADIUS_TO_SPLIT) {
        const uint8_t fragmentCount = GetRandomValue(2, 4);
        float newRadius = radius / fragmentCount;
        if (newRadius < ASTEROID_MIN_FRAGMENT_RADIUS) newRadius = ASTEROID_MIN_FRAGMENT_RADIUS;

        for (int i = 0; i < fragmentCount; i++) {
            const float angle = 360.0f * (i / (float)fragmentCount) * DEG2RAD;
            const auto newPosition = position + raylib::Vector2(cosf(angle), sinf(angle)) * newRadius;
            auto newAsteroid = std::make_shared<Asteroid>(newPosition, newRadius);
            newAsteroid->velocity = raylib::Vector2(cosf(angle), sinf(angle)) * getRandomValue(-1.0f, 1.0f) * ASTEROID_VELOCITY * 2.0f;
            // newAsteroid->scale = 1.0f;
            newAsteroids.push_back(newAsteroid);
        }
    }

    return newAsteroids;
}

void Asteroid::update(IContainer &others, float deltaTime) {
	updatePhysics(deltaTime);
    updateVertices();

    if (scale >= 1.0f)
		solveCollisions(others);
    else
		updateAnimations();

	wrapAroundScreen();
}

void Asteroid::render() {
    // Check if the asteroid is inside the screen
    if (position.x + radius < 0 || position.x - radius > WIDTH || position.y + radius < 0 || position.y - radius > HEIGHT) return;

    drawFilledPolygon(position, vertices.data(), vertices.size(), BLACK);// Only to occlude other asteroids
    drawPolygonOutline(vertices.data(), vertices.size(), WHITE);

    if (RENDER_GRID) {
        auto text = TextFormat("%i", gridCellIndex);
        auto size = MeasureTextEx(GetFontDefault(), text, 12, 1);
        DrawText(text, position.x - size.x / 2.0f, position.y - size.y / 2.0f, 12, LIGHTGRAY);
    }

#ifdef DEBUG
	/* DrawCircleLinesV(position, radius, LIGHTGRAY);
	DrawCircleLinesV(position, innerRadius, GRAY); */
#endif
}
