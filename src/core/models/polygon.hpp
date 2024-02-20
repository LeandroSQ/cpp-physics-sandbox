#pragma once

#include "../precomp.hpp"
#include "Vector2.hpp"
#include <array>
#include <vector>

class Polygon {
  public:
    std::vector<raylib::Vector2> vertices;
    float innerRadius;
    float outerRadius;

    void generateVertices(float radius, uint8_t vertexCount);
    void render(raylib::Vector2 center, float scale, float angle, raylib::Color color);
    std::vector<Polygon> split();
};