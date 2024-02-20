#include "bullet.hpp"
#include "../settings.hpp"
#include "asteroid.hpp"

bool Bullet::update(IContainer &asteroids) {
    // Update physics
    position = position + velocity * GetFrameTime();

    // Check if outside screen bounds
    if (position.x < 0 || position.x > WIDTH || position.y < 0 || position.y > HEIGHT) {
        return true;
    }

    // Check for collisions
    const auto list = asteroids.retrieve(position, ASTEROID_RADIUS * 2);
    for (auto &asteroid : list) {
        if (position.Distance(asteroid->position) < asteroid->polygon.outerRadius) {
            if (asteroid->polygon.outerRadius >= ASTEROID_MIN_RADIUS_TO_SPLIT) {
                // Split the asteroid
                auto offset = raylib::Vector2::Zero();
                for (auto &poly : asteroid->polygon.split()) {
                    auto r = raylib::Vector2::One() * poly.outerRadius;

                    const auto newAsteroid = std::make_shared<Asteroid>(std::move(poly), asteroid->position + offset - r);
                    asteroids.insert(newAsteroid);

                    // Calculate the velocity of the new asteroid
                    float angle = atan2f(newAsteroid->position.y - position.y, newAsteroid->position.x - position.x);
                    newAsteroid->velocity = asteroid->velocity + raylib::Vector2(cosf(angle), sinf(angle)) * ASTEROID_VELOCITY;

                    offset += r;
                }
            }

            // Kill the asteroid
            asteroids.remove(asteroid);

            return true;
        }
    }

    return false;
}

void Bullet::render() {
    DrawCircleV(position, BULLET_RADIUS, WHITE);
}
