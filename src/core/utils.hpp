#pragma once

#include "Color.hpp"
#include "precomp.hpp"
#include "raylib.h"
#include "settings.hpp"
#include <climits>

inline raylib::Vector2 rotateAround(raylib::Vector2 point, raylib::Vector2 center, float angle) {
    return {
        center.x + (point.x - center.x) * cosf(angle) - (point.y - center.y) * sinf(angle),
        center.y + (point.x - center.x) * sinf(angle) + (point.y - center.y) * cosf(angle)
    };
}

inline float getRandomValue(float min, float max) {
    return GetRandomValue(0, RAND_MAX) / (float)RAND_MAX * (max - min) + min;
}

inline float smoothstep(float x) {
    return x * x * (3 - 2 * x);
}

inline float easeInOutBack(float x) {
    const float tension = 3.0158f;
    const float overshoot = tension * 1.525f;

    return x < 0.5
        ? 0.5f * (x * x * ((overshoot + 1) * x - overshoot))
        : 0.5f * ((2 * x - 2) * (2 * x - 2) * ((overshoot + 1) * (2 * x - 2) + overshoot) + 2);
}

inline raylib::Color randomColor() {
    return raylib::Color(
        GetRandomValue(128, 255),
        GetRandomValue(128, 255),
        GetRandomValue(128, 255),
        255
    );
}