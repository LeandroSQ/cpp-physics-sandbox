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

// #define QUADTREE_DUMMY
#define CENTER_CIRCLE
constexpr bool ENABLE_TEMPERATURE = false;
