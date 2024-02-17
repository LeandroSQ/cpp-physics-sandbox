#pragma once

#include "Color.hpp"
#include "Image.hpp"
#include "Vector2.hpp"
#include "precomp.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "settings.hpp"
#include <climits>
#include <vector>

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
    return x < 0.5f ? 2 * x * x : 1 - powf(-2 * x + 2, 2) / 2;
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

/**
 * @brief Given a center and a list of vertices, draw a filled polygon
 * Raylib does provide a few functions for drawing polygons, but I don't really like the way they work and in some scenarios the results are incomplete polygons
 *
 * @param center
 * @param vertices
 * @param color
 */
static void drawFilledPolygon(raylib::Vector2 center, raylib::Vector2 *vertices, size_t size, raylib::Color color) {
    if (size < 3) return;
    if (size == 3) {
        DrawTriangle(vertices[0], vertices[1], vertices[2], color);
    }

#if defined(SUPPORT_QUADS_DRAW_MODE)
    constexpr Texture2D texShapes = { 1, 1, 1, 1, 7 };
    constexpr Rectangle texShapesRec = { 0.0f, 0.0f, 1.0f, 1.0f };
    rlSetTexture(texShapes.id);

    rlBegin(RL_QUADS);
	rlColor4ub(color.r, color.g, color.b, color.a);
    for (int i = 0; i < size; i++)
    {
        int j = (i + 1) % size;

		rlTexCoord2f(texShapesRec.x / texShapes.width, texShapesRec.y / texShapes.height);
		rlVertex2f(vertices[i].x, vertices[i].y);

		rlTexCoord2f(texShapesRec.x / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
		rlVertex2f(center.x, center.y);

		rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, (texShapesRec.y + texShapesRec.height) / texShapes.height);
		rlVertex2f(center.x, center.y);

		rlTexCoord2f((texShapesRec.x + texShapesRec.width) / texShapes.width, texShapesRec.y / texShapes.height);
		rlVertex2f(vertices[j].x, vertices[j].y);
	}

	rlEnd();

    rlSetTexture(0);
 #else
        rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a);
        for (int i = 0; i < size; i++)
        {
            int j = i + 1;
            if (j == size) j = 0;

			rlVertex2f(vertices[i].x, vertices[i].y);
			rlVertex2f(center.x, center.y);
			rlVertex2f(vertices[j].x, vertices[j].y);
        }

		rlEnd();
#endif
}

static void drawPolygonOutline(raylib::Vector2 *vertices, size_t size, raylib::Color color) {
    if (size < 3) return;

    rlBegin(RL_LINES);
    rlColor4ub(color.r, color.g, color.b, color.a);

    for (int i = 0; i < size - 1; i++) {
        rlVertex2f(vertices[i].x, vertices[i].y);
        rlVertex2f(vertices[i + 1].x, vertices[i + 1].y);
    }

    rlVertex2f(vertices[size - 1].x, vertices[size - 1].y);
    rlVertex2f(vertices[0].x, vertices[0].y);

    rlEnd();
}

static float noise(raylib::Vector2 position) {
    static std::optional<raylib::Image> image;

    if (!image.has_value()) {
        image = GenImagePerlinNoise(200, 200, 0, 0, 0.2f);
    }

    const int x = (int)position.x % 200;
    const int y = (int)position.y % 200;

    const auto color = image->GetColor(x, y);
    return color.r / 255.0f;
}

static float randomKelvin() {
    return powf(getRandomValue(0.0f, 1.0f), 1.0f / 2.0f) * 9000.0f + 1000.0f;
}

static raylib::Color convertKelvinToColor(float temperature) {
    // Using Tanner Helland's formula and Neil Bartlett's implementation

    float red, green, blue;

    if (temperature <= 6600.0f) {
        red = 255.0f;
        green = temperature / 100.0f - 2.0f;
        green = 99.4708025861f * logf(green) - 161.1195681661f;
        if (temperature <= 1900.0f) {
            blue = 0.0f;
        } else {
            blue = temperature / 100.0f - 10.0f;
            blue = 138.5177312231f * logf(blue) - 305.0447927307f;
        }
    } else {
        red = temperature / 100.0f - 55.0f;
        red = 329.698727446f * powf(red, -0.1332047592f);
        green = temperature / 100.0f - 50.0f;
        green = 288.1221695283f * powf(green, -0.0755148492f);
        blue = 255.0f;
    }

    return raylib::Color((unsigned char)red, (unsigned char)green, (unsigned char)blue, 255);
}

inline float oscillate(float perSecond, float amplitude, float offset = 0.0f) {
    return sinf(PI * 2.0f * GetTime() * perSecond + offset) * amplitude;
}