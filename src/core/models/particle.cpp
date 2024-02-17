#include "particle.hpp"
#include "../utils.hpp"
#include "Color.hpp"

bool Particle::update() {
    position = position + velocity * GetFrameTime();

    // Deviation
    if (GetRandomValue(0, 100) < 2) {
        velocity = raylib::Vector2(GetRandomValue(-1.0f, 1.0f), GetRandomValue(-1.0f, 1.0f)).Normalize() * velocity.Length();
    }

    timeAlive += GetFrameTime();

    if (timeAlive >= timeToLive) {
        timeAlive = timeToLive;
        return true;
    }

    return false;
}

void Particle::render() {
    const float progress = powf(1.0f - timeAlive / timeToLive, 2.0f);
    const float alpha = this->color.a * progress;
    const float radius = this->radius * progress;
    raylib::Color currentColor;
    if (changeColorBasedOnTime) currentColor = convertKelvinToColor(1000.0f + progress * 9000.0f);
    else raylib::Color(color.r, color.g, color.b, alpha);

    DrawCircleV(position, radius, currentColor);
}
