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
    const float OBJECT_RADIUS = 10.0f;
    const unsigned int SPAWN_COUNT = 800;
#endif
const float SPAWN_INTERVAL = 0.07f;
const float MANUAL_SPAWN_INTERVAL = 0.25f;

// Constants - Temperature
const float AIR_TEMPERATURE = 4000.0f;
const float GROUND_TEMPERATURE = 10000.0f;

// Constants - Interaction
const float CENTER_CIRCLE_RADIUS = 400.0f;
const float DRAGGING_ACCELERATION = 35000.0f;
const float EXPLOSION_FORCE = 1500000.0f;
const float EXPLOSION_RADIUS = 50.0f;

// #define QUADTREE_DUMMY

// Flags - Constraints
extern float CENTER_CIRCLE_CURRENT_RADIUS;
extern bool ENABLE_CIRCLE_CONSTRAINT;

// Flags - Render mode
extern bool ENABLE_HEAT_FROM_CIRCLE_BORDER;
extern bool ENABLE_TEMPERATURE;
extern bool ENABLE_SOLID_COLOR;
extern bool ENABLE_FIXED_RAINBOW;
extern bool ENABLE_RAINBOW_COLORS;
extern bool ENABLE_AUTO_ADJUST_SUBSTEPS;

// Flags - Spawning
extern bool ENABLE_SPAWN_SPREAD;
