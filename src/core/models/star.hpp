#pragma once

#include "../precomp.hpp"
#include "../settings.hpp"
#include "Color.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include "ship.hpp"
#include "../utils.hpp"
#include <array>

struct Star {
    raylib::Vector2 position;
    raylib::Color color;
    float angle;
    float radius;

    Star(raylib::Vector2 position, float radius) : position(position), radius(radius) {
        angle = getRandomValue(0, PI * 2.0f);

        color = convertKelvinToColor(randomKelvin());
    }

    void update(raylib::Vector2 offset) {
        const float parallax = powf(radius / 5.0f, 2.0f) / 25.0f;
        position -= offset * parallax;

        if (position.x + radius > WIDTH) position.x -= WIDTH + radius;
        if (position.x - radius < 0) position.x += WIDTH + radius;
        if (position.y + radius > HEIGHT) position.y -= HEIGHT + radius;
        if (position.y - radius < 0) position.y += HEIGHT + radius;
    }

    void render() {
        angle += 0.1f * GetFrameTime();

        float alpha = (radius / 7.0f) + 0.25f;
        alpha *= sinf((PI * 2.0f * GetTime() + radius * 10.0f) / 10.0f) * 0.15f + 0.85f;

        const float angleIncrement = 360.0f / 5.0f * DEG2RAD;
        float currentAngle = -90 * DEG2RAD + angle;
        float innerRadius = radius * 0.5f;
        std::array<raylib::Vector2, 10> vertices;
        for (int i = 0; i < 5; i++) {
            // Calculate the position of the outer vertex
            float x1 = position.x + cosf(currentAngle) * radius;
            float y1 = position.y + sinf(currentAngle) * radius;
            vertices[i * 2] = raylib::Vector2(x1, y1);
            currentAngle += angleIncrement;

            // Calculate the position of the inner vertex
            float x2 = position.x + cosf(currentAngle + angleIncrement / 2.0f) * innerRadius;
            float y2 = position.y + sinf(currentAngle + angleIncrement / 2.0f) * innerRadius;
            vertices[i * 2 + 1] = raylib::Vector2(x2, y2);
        }

        drawFilledPolygon(position, vertices.data(), vertices.size(), Fade(color, alpha));

        // Draw halo around it
        if (RENDER_STAR_HALO) {
            DrawCircleV(position, radius * 1.5f, Fade(color, alpha * 0.15f));
        }

        /* // Extra effect, draw lines from the center to radius * 2
        // Draw angled line 1
        auto offset = raylib::Vector2(cosf(angle), sinf(angle)) * radius * 1.5f;
        DrawLineV(position - offset, position + offset, Fade(color, alpha * 0.25f));

        // Draw angled line 2
        offset = raylib::Vector2(cosf(angle + PI / 4.0f), sinf(angle + PI / 4.0f)) * radius * 1.5f;
        DrawLineV(position - offset, position + offset, Fade(color, alpha * 0.25f)); */
    }
};