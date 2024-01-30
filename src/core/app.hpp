#pragma once

#include "precomp.hpp"
#include "quadtree.hpp"
#include "solver.hpp"
#include "interation_handler.hpp"

class App {
  private:
    Quadtree quadtree;
    Solver solver;
    InterationHandler spawner;
    Camera2D camera;
    bool isRenderingQuadtree;

    void handleInput(float deltaTime);
    void renderGUI();

  public:
    App();
    ~App();

    void setup();
    void update();
    void render();
    void resize();
};