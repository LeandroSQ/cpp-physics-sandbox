#include "bullet.hpp"
#include "../settings.hpp"
#include "asteroid.hpp"
#include "particle.hpp"
#include "soundfx.hpp"

void Bullet::updatePhysics() {
    position = position + velocity * GetFrameTime();
}

bool Bullet::solveCollisions(IContainer &grid) {
    // Check for collisions
    const auto list = grid.retrieve(position, ASTEROID_RADIUS * 2);
    for (auto &asteroid : list) {
        if (position.Distance(asteroid->position) < asteroid->radius) {
            if (asteroid->radius >= ASTEROID_MIN_RADIUS_TO_SPLIT) {
                const auto newAsteroids = asteroid->split();
                for (auto &newAsteroid : newAsteroids) grid.insert(newAsteroid);
            }

            // Particles
            ParticleSystem::addExplosion(position, 30, 0, 360);

            // Play sound
            SoundFX::play(SoundFX::explosion, 0.25f, -1.0f, 1.0f - asteroid->position.x / (float)WIDTH);

            // Kill the asteroid
            grid.remove(asteroid);

            return true;
        }
    }

    return false;
}

Bullet::Bullet(raylib::Vector2 position, raylib::Vector2 velocity) : position(position), velocity(velocity) {  }


bool Bullet::update(IContainer &grid) {
    timeAlive += GetFrameTime();
    if (timeAlive > BULLET_TIME_TO_LIVE) return true;

    updatePhysics();

    // Check if outside screen bounds
    if (BULLETS_WRAP_AROUND_SCREEN) {
        if (position.x < 0) position.x = WIDTH;
        if (position.x > WIDTH) position.x = 0;
        if (position.y < 0) position.y = HEIGHT;
        if (position.y > HEIGHT) position.y = 0;
    } else if (position.x < 0 || position.x > WIDTH || position.y < 0 || position.y > HEIGHT) {
        return true;
    }

    if (solveCollisions(grid)) {
        // TODO: Spawn particles
        // TODO: Play sound
        return true;
    }

    // TODO: Emit particle tail
    // TODO: Emit light

    return false;
}

void Bullet::render() {
    DrawCircleV(position, BULLET_RADIUS, WHITE);
}
