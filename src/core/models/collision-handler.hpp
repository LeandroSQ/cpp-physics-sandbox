#pragma once

#include "../precomp.hpp"

namespace CollisionHandler {
    struct CollisionInfo {
        raylib::Vector2 normal;
        float penetration;
    };

    struct Shape {
        std::vector<raylib::Vector2> vertices;
        raylib::Vector2 position;
        float radius;
    };

    /**
    * @brief Implements the Separating Axis Theorem (SAT) to detect collisions between two polygons
    *
    * @param a The first polygon
    * @param b The second polygon
    * @return std::optional<CollisionInfo> The collision info if a collision is detected, otherwise std::nullopt
    */
    std::optional<CollisionInfo> getCollisionInfo(const Shape &a, const Shape &b);
}