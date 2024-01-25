#pragma once

const unsigned int WIDTH = 850;
const unsigned int HEIGHT = 850;
const unsigned int TARGET_FPS = 100;
const unsigned int MAX_OBJECTS = 3;
const unsigned int MAX_SUBDIVISIONS = 5;
const unsigned int SPAWN_COUNT = 500;
const float SPAWN_INTERVAL = 0.005f;
const float MANUAL_SPAWN_INTERVAL = 0.25f;
const float CENTER_CIRCLE_RADIUS = 400.0f;
const unsigned int SUBSTEPS = 4;

// #define QUADTREE_DUMMY
// #define CENTER_CIRCLE
constexpr bool ENABLE_TEMPERATURE = true;

#pragma region Colors
const raylib::Color PALETTE_RED = raylib::Color(231, 76, 60);
const raylib::Color PALETTE_BLUE = raylib::Color(15, 188, 249);
const raylib::Color PALETTE_WHITE = raylib::Color(200, 214, 229);
const raylib::Color PALETTE_BLACK = raylib::Color(21, 21, 21);
const raylib::Color PALETTE_GREY = raylib::Color(44, 44, 44);
#pragma endregion