#pragma once

#include "../precomp.hpp"
#include "../utils.hpp"
#include "Color.hpp"
#include "Vector2.hpp"

struct Particle {
    raylib::Vector2 position;
    raylib::Vector2 velocity;
    raylib::Color color;
    float radius;
    float timeToLive = 1.0f;
    float timeAlive = 0.0f;
    bool changeColorBasedOnTime = false;

    Particle(raylib::Vector2 position, raylib::Vector2 velocity, raylib::Color color, float radius, float timeToLive, bool changeColorBasedOnTime = false) : position(position), velocity(velocity), color(color), radius(radius), timeToLive(timeToLive), changeColorBasedOnTime(changeColorBasedOnTime) {  }

    bool update();
    void render();

};

namespace ParticleSystem {
    inline std::list<Particle> particles;

    static void update() {
        for (auto it = particles.begin(); it != particles.end();) {
            if (it->update()) {
                it = particles.erase(it);
            } else {
                it++;
            }
        }
    }

    static void render() {
        for (auto &particle : particles) {
            particle.render();
        }
    }

    static void addExplosion(raylib::Vector2 position, unsigned int count, float startAngle = -PI, float endAngle = PI, raylib::Vector2 baseVelocity = raylib::Vector2::Zero(), float minVelocity = 50.0f, float maxVelocity = 100.0f, float radius = 2.0f, float timeToLive = 1.0f, raylib::Color color = LIGHTGRAY) {
        for (int i = 0; i < count; i++) {
            const float angle = i / (float)count * (endAngle - startAngle) + startAngle;
            const auto velocity = raylib::Vector2(cosf(angle), sinf(angle)) * getRandomValue(minVelocity, maxVelocity) + baseVelocity;
            particles.emplace_back(position, velocity, color, radius, timeToLive, true);
        }
    }

    static void addTrail(raylib::Vector2 position, unsigned int count, float baseAngle, raylib::Vector2 velocity, raylib::Color color = RED, float radius = 2.0f, float timeToLive = 0.250f) {
        for (int i = 0; i < count; i++) {
            const float angle = getRandomValue(-PI, PI) + baseAngle;
            const auto vel = raylib::Vector2(cosf(angle), sinf(angle)) * getRandomValue(-8.5f, 40.0f) + velocity;
            particles.emplace_back(position + vel / SHIP_SIZE, vel, color, radius, timeToLive, true);
        }
    }
}