#pragma once

#include "../precomp.hpp"
#include "quadtree.hpp"
#include "solver.hpp"
#include "interation_handler.hpp"

class App {
  private:
    Solver solver;
    Camera2D camera;
    Quadtree quadtree;
    InterationHandler spawner;

    bool isRenderingQuadtree;

    float frameStartTime;
    uint8_t frameCounter;
    float frameTimeSum;

    void handleInput(float deltaTime);
    void renderGUI();
    void setupGUI();

  public:

    App();
    ~App();

    void onFrameStart();
    void onFrameEnd();

    void setup();
    void update();
    void render();
    void resize();
};