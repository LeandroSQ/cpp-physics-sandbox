#pragma once

// Constants - General
extern unsigned int WIDTH;
extern unsigned int HEIGHT;
extern unsigned int TARGET_FPS;

// Constants - Ship
const float SHIP_ANGULAR_ACCELERATION = 30.0f * DEG2RAD;
const float SHIP_ACCELERATION = 150.0f;
const float SHIP_DAMPING = 0.98f;
const float SHIP_ANGULAR_DAMPING = 0.90f;

// Constants - Asteroid
const float ASTEROID_RADIUS = 25.0f;
const float ASTEROID_JAGGEDNESS = 5.0f;
const float ASTEROID_ANGULAR_VELOCITY = 100.0f * DEG2RAD;
const float ASTEROID_VELOCITY = 100.0f;
const int ASTEROID_MIN_VERTEX_COUNT = 8;
const int ASTEROID_MAX_VERTEX_COUNT = 32;
const int ASTEROID_FRAGMENTS_COUNT = 3;
const float ASTEROID_RADIUS_DIFFERENCE = 10.0f;
const float ASTEROID_MIN_RADIUS_TO_SPLIT = 20.0f;

// Constants - Bullet
const float BULLET_VELOCITY = 500.0f;
const float BULLET_RADIUS = 2.0f;
const float BULLET_SHOOT_INTERVAL = 0.250f;

// Constants - Level
const uint8_t LEVEL_ASTEROIDS_PER_WAVE = 10;
const float LEVEL_ASTEROID_SPAWN_INTERVAL = 5.0f;
const float LEVEL_ASTEROID_SPAWN_INTERVAL_DECREMENT_PER_WAVE = 0.5f;
const float LEVEL_ASTEROID_SPAWN_INTERVAL_MIN = 0.05f;
const float LEVEL_WAVE_INTERVAL = (LEVEL_ASTEROIDS_PER_WAVE + 1) * LEVEL_ASTEROID_SPAWN_INTERVAL;
const float LEVEL_WAVE_INTERVAL_DECREMENT = 0.5f;
const float LEVEL_WAVE_INTERVAL_MIN = 2.0f;