#pragma once

const unsigned int WIDTH = 850;
const unsigned int HEIGHT = 850;
const unsigned int TARGET_FPS = 100;
const unsigned int MAX_OBJECTS = 3;
const unsigned int MAX_SUBDIVISIONS = 5;
const unsigned int SPAWN_COUNT = 1000;
const float SPAWN_INTERVAL = 0.005f;

// #define QUADTREE_DUMMY
// #define QUADTREE_GRID
#define GRID_SIZE 50
#define CENTER_CIRCLE
// #define MULTI_THREADING