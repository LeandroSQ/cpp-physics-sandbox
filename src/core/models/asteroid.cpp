#include "asteroid.hpp"
#include "../utils.hpp"
#include "polygon.hpp"

size_t Asteroid::idCounter = 0;
uint32_t Asteroid::collisionCount = 0;

#pragma region SAT
struct CollisionInfo {
    raylib::Vector2 normal;
    float penetration;
};

struct Projection {
    float min;
    float max;
};

inline std::vector<raylib::Vector2> getGlobalVertices(const Asteroid &a) {
    std::vector<raylib::Vector2> vertices(a.polygon.vertices.size());
    for (size_t i = 0; i < a.polygon.vertices.size(); i++) {
        vertices[i] = rotateAround(a.position + a.polygon.vertices[i], a.position, a.angle);
    }
    return vertices;
}

void getAxes(const std::vector<raylib::Vector2> &vertices, std::vector<raylib::Vector2> &axes) {
    for (size_t i = 0; i < vertices.size(); i++) {
        const auto p1 = vertices[i];
        const auto p2 = vertices[(i + 1) % vertices.size()];
        const auto edge = p1 - p2;
        axes.emplace_back(-edge.y, edge.x);
    }
}

Projection project(const std::vector<raylib::Vector2> &vertices, const raylib::Vector2 &axis) {
    float min = axis.DotProduct(vertices[0]);
    float max = min;
    for (size_t i = 1; i < vertices.size(); i++) {
        const float p = axis.DotProduct(vertices[i]);
        if (p < min) min = p;
        else if (p > max) max = p;
    }
    return { min, max };
}

std::optional<CollisionInfo> getCollisionInfo(const Asteroid &a, const Asteroid &b) {
    auto normal = raylib::Vector2::Zero();
    float overlap = std::numeric_limits<float>::max();

    // Get the global world position of the vertices
    const auto verticesA = getGlobalVertices(a);
    const auto verticesB = getGlobalVertices(b);

    // Get the axes of the polygons
    std::vector<raylib::Vector2> axes;
    getAxes(verticesA, axes);
    getAxes(verticesB, axes);

    // Iterate through the axes
    for (auto it = axes.begin(); it != axes.end(); it++) {
        const auto axis = *it;
        const auto projectionA = project(verticesA, axis);
        const auto projectionB = project(verticesB, axis);

        // Detected a gap, no collision
        if (projectionA.max < projectionB.min || projectionB.max < projectionA.min) {
            return std::nullopt;
        }

        // Calculate the overlap
        const float depth = std::min(projectionA.max, projectionB.max) - std::max(projectionA.min, projectionB.min);
        if (depth < overlap) {
            overlap = depth;
            normal = axis;
        }
    }

    // Normalize the depth and normal
    const auto length = normal.Length();
    normal /= length;
    overlap /= length;

    // If the normal is pointing from A to B, invert it
    const auto center = b.position - a.position;
    if (center.DotProduct(normal) < 0) {
        normal = -normal;
    }

    return CollisionInfo{ normal, overlap };
}
#pragma endregion

void Asteroid::updatePhysics() {
    const auto deltaTime = GetFrameTime();

	position += velocity * deltaTime;
	angle += angularVelocity * deltaTime;
}

void Asteroid::wrapAroundScreen() {
    if (position.x - polygon.outerRadius > WIDTH) {
        position.x = -polygon.outerRadius;
    } else if (position.x < -polygon.outerRadius) {
        position.x = WIDTH + polygon.outerRadius;
    }

    if (position.y - polygon.outerRadius > HEIGHT) {
        position.y = -polygon.outerRadius;
    } else if (position.y < -polygon.outerRadius) {
        position.y = HEIGHT + polygon.outerRadius;
    }
}

void Asteroid::checkForCollisions(IContainer &asteroids) {
    const auto others = asteroids.retrieve(position, polygon.outerRadius * 2);
    for (auto &other : others) {
        if (id == other->id) continue;

        // Check if asteroids are close enough to collide
        // const float distanceLength = (position - other->position).Length();
        // const float minimumDistance = powf(polygon.outerRadius + other->polygon.outerRadius, 2.0f);
        // if (distanceLength > minimumDistance) continue;

        // Time to bring out the big guns
        const auto contact = getCollisionInfo(*this, *other.get());
        if (contact.has_value()) {
            Asteroid::collisionCount++;
            // Move the asteroids apart
            const auto correction = contact->normal * contact->penetration;
            position -= correction / 2.0f;
            other->position += correction / 2.0f;

            // Calculate the new velocities
            const auto relativeVelocity = velocity - other->velocity;
            const auto velocityAlongNormal = relativeVelocity.DotProduct(contact->normal);

            // Use asteroid.radius / ASTEROID_RADIUS for mass
            const float e = 1.0f; // Coefficient of restitution
            const float j = -(1 + e) * velocityAlongNormal;
            const float mass1 = polygon.outerRadius / ASTEROID_RADIUS;
            const float mass2 = other->polygon.outerRadius / ASTEROID_RADIUS;
            const float impulse = j / (mass1 + mass2);
            velocity += contact->normal * impulse * mass1;
            other->velocity -= contact->normal * impulse * mass2;

            // Ensure the asteroids are not stationary
            if (velocity.Length() < 0.1f) velocity = raylib::Vector2::One() * getRandomValue(-1.0f, 1.0f) * ASTEROID_VELOCITY;
            if (other->velocity.Length() < 0.1f) other->velocity = raylib::Vector2::One() * getRandomValue(-1.0f, 1.0f) * ASTEROID_VELOCITY;

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
    // Scale up the asteroid
    scale += GetFrameTime() * 2.0f; // 500ms
    if (scale > 1.0f) scale = 1.0f;
}

Asteroid::Asteroid() {
    id = Asteroid::idCounter++;
    polygon.generateVertices(ASTEROID_RADIUS, GetRandomValue(ASTEROID_MIN_VERTEX_COUNT, ASTEROID_MAX_VERTEX_COUNT));
	position = raylib::Vector2(GetRandomValue(0, WIDTH), GetRandomValue(0, HEIGHT));
    velocity = raylib::Vector2::One() * getRandomValue(-1.0f, 1.0f) * ASTEROID_VELOCITY;
	angle = getRandomValue(0.0f, 360.0f) * DEG2RAD;
	angularVelocity = getRandomValue(-1.0f, 1.0f) * ASTEROID_ANGULAR_VELOCITY;
}

Asteroid::Asteroid(Polygon &&polygon, raylib::Vector2 position) : polygon(polygon), position(position) {
    id = Asteroid::idCounter++;
    angle = getRandomValue(0.0f, 360.0f) * DEG2RAD;
	angularVelocity = getRandomValue(-1.0f, 1.0f) * ASTEROID_ANGULAR_VELOCITY;
    scale = 1.0f;
}

void Asteroid::update(IContainer &others) {
    updatePhysics();
        if (scale >= 1.0f) checkForCollisions(others);
        else updateAnimations();
        wrapAroundScreen();
}

void Asteroid::render() {
	polygon.render(position, easeInOutBack(scale), angle, color);

#ifdef DEBUG
    auto text = TextFormat("%i", index);
    auto size = MeasureTextEx(GetFontDefault(), text, 12, 1);
    DrawText(text, position.x - size.x / 2.0f, position.y - size.y / 2.0f, 12, LIGHTGRAY);
#endif
}
