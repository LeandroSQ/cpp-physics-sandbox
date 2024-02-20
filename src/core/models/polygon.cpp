#include "polygon.hpp"
#include "../settings.hpp"
#include "../utils.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include <vector>

void Polygon::generateVertices(float radius, uint8_t vertexCount) {
    outerRadius = 0.0f;
    innerRadius = 0.0f;

    float scale = radius / ASTEROID_RADIUS;

    for (int i = 0; i < vertexCount; i++) {
		float angle = (i / (float)vertexCount) * 2.0f * PI;

        float r = radius + GetRandomValue(-ASTEROID_JAGGEDNESS * scale, ASTEROID_JAGGEDNESS * scale);

        // Keep track of the largest radius
        if (outerRadius == 0.0f || r > outerRadius) outerRadius = r;

        // Keep track of the smallest radius
        if (innerRadius == 0.0f || r < innerRadius) innerRadius = r;

		vertices.emplace_back(cosf(angle) * r, sinf(angle) * r);
	}
}

void Polygon::render(raylib::Vector2 center, float scale, float angle, raylib::Color color) {
    for (size_t i = 0; i < vertices.size(); i++) {
		size_t j = (i + 1) % vertices.size();
		DrawLineEx(
            rotateAround(center + vertices[i] * scale, center, angle),
            rotateAround(center + vertices[j] * scale, center, angle),
            1.5f,
            color
        );
	}

// #ifdef DEBUG
//     DrawCircleLinesV(center, outerRadius, LIGHTGRAY);
//     DrawCircleLinesV(center, innerRadius, GRAY);
// #endif
}

std::vector<Polygon> Polygon::split() {
    std::vector<Polygon> buffer;

    float radius = ceilf(outerRadius / (float)(ASTEROID_FRAGMENTS_COUNT - 1));
    uint8_t vertexCount = GetRandomValue(ASTEROID_MIN_VERTEX_COUNT, ASTEROID_MAX_VERTEX_COUNT);
    for (size_t i = 0; i < ASTEROID_FRAGMENTS_COUNT; i++) {
        buffer.emplace_back();
        buffer.back().generateVertices(radius, vertexCount);
    }

    return buffer;
}