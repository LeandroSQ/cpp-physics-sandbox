#pragma once

#include "../precomp.hpp"
#include "../data/spatial-hash-grid.hpp"

struct Bullet {
    raylib::Vector2 position;
    raylib::Vector2 velocity;

    bool update(IContainer &asteroids);
    void render();
};