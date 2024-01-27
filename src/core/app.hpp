#pragma once

#include "precomp.hpp"
#include "quadtree.hpp"
#include "solver.hpp"
#include "spawner.hpp"

class App {
  private:
    Quadtree quadtree;
    Solver solver;
    Spawner spawner;
    Camera2D camera;
    bool isRenderingQuadtree = true;

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