#pragma once

// Constants - General
inline unsigned int WIDTH = 850;
inline unsigned int HEIGHT = 750;
inline unsigned int TARGET_FPS = 60;

// Constants - Ship
const float SHIP_SIZE = 10.0f;
const float SHIP_ANGULAR_ACCELERATION = 0.265f;
const float SHIP_MAX_ANGULAR_VELOCITY = 3.5f;
const float SHIP_ACCELERATION = 500.0f;
const float SHIP_DAMPING = 0.988f;
const float SHIP_ANGULAR_DAMPING = 0.963f;

// Constants - Asteroid
const float ASTEROID_RADIUS = 25.0f;
const float ASTEROID_JAGGEDNESS = 5.0f;
const float ASTEROID_ANGULAR_VELOCITY = 100.0f * DEG2RAD;
const float ASTEROID_VELOCITY = 100.0f;
const float ASTEROID_RESTITUTION = 0.98f;
const int ASTEROID_MIN_VERTEX_COUNT = 8;
const int ASTEROID_MAX_VERTEX_COUNT = 32;
const int ASTEROID_FRAGMENTS_COUNT = 3;
const float ASTEROID_RADIUS_DIFFERENCE = 10.0f;
const float ASTEROID_MIN_RADIUS_TO_SPLIT = 20.0f;
const float ASTEROID_MIN_FRAGMENT_RADIUS = 7.5f;

// Constants - Bullet
const float BULLET_VELOCITY = 500.0f;
const float BULLET_RADIUS = 2.0f;
const float BULLET_SHOOT_INTERVAL = 0.250f;
const float BULLET_TIME_TO_LIVE = 1.0f;

// Constants - Level
const uint8_t LEVEL_ASTEROIDS_PER_WAVE = 10;
const float LEVEL_ASTEROID_SPAWN_INTERVAL = 5.0f;
const float LEVEL_ASTEROID_SPAWN_INTERVAL_DECREMENT_PER_WAVE = 0.15f;
const float LEVEL_ASTEROID_SPAWN_INTERVAL_MIN = 0.5f;
const float LEVEL_WAVE_INTERVAL = (LEVEL_ASTEROIDS_PER_WAVE + 1) * LEVEL_ASTEROID_SPAWN_INTERVAL;
const float LEVEL_WAVE_INTERVAL_DECREMENT = 0.25f;
const float LEVEL_WAVE_INTERVAL_MIN = 2.0f;

// Flags - Grid
inline bool RENDER_GRID = false;

// Flags - Bullets
const bool BULLETS_WRAP_AROUND_SCREEN = true;

// Flags - Stars
inline bool RENDER_STAR_HALO = true;

// Flags - Asteroids
const bool SPAWN_AT_START = true;
const bool ASTEROIDS_SELF_COLLISION = true;