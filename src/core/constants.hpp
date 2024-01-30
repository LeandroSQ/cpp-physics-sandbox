#pragma once

// Constants - General
const unsigned int WIDTH = 850;
const unsigned int HEIGHT = 850;
const unsigned int TARGET_FPS = 60;

// Constants - Quadtree
const unsigned int MAX_OBJECTS = 3;
const unsigned int MAX_SUBDIVISIONS = 5;

// Constants - Spawn
#ifdef PLATFORM_WEB
    const float OBJECT_RADIUS = 10.0f;
    const unsigned int SPAWN_COUNT = 500;
#else
    const float OBJECT_RADIUS = 6.0f;
    const unsigned int SPAWN_COUNT = 1000;
#endif
const float SPAWN_INTERVAL = 0.05f;
const float MANUAL_SPAWN_INTERVAL = 0.25f;

// Constants - Interaction
const float CENTER_CIRCLE_RADIUS = 400.0f;
const float DRAGGING_ACCELERATION = 35000.0f;
const float EXPLOSION_FORCE = 1500000.0f;
const float EXPLOSION_RADIUS = 50.0f;

// Flags
// #define QUADTREE_DUMMY
#define CENTER_CIRCLE
constexpr bool ENABLE_TEMPERATURE = false;
constexpr bool ENABLE_RAINBOW = true;
constexpr bool ENABLE_SPAWN_SPREAD = false;
