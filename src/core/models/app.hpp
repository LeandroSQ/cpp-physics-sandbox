#pragma once

#include "../precomp.hpp"
#include "Vector2.hpp"
#include "bullet.hpp"
#include "hotreload-shader.hpp"
#include "particle.hpp"
#include "profiler.hpp"
#include "ship.hpp"
#include "asteroid.hpp"
#include "star.hpp"
#include "wave.hpp"
#include "particle.hpp"
#include "../data/spatial-hash-grid.hpp"

class App : public std::enable_shared_from_this<App> {
  private:
    raylib::Camera2D camera;
    raylib::RenderTexture2D frameBuffer;
    raylib::RenderTexture2D tempFrameBuffer;

    // Shaders
    HotReloadShader starsShader;
    HotReloadShader particlesShader;
    HotReloadShader backgroundShader;
    HotReloadShader allShader;

    // Uniforms
    int uniformResolutionLocation = -1;
    raylib::Vector2 uniformResolutionValue;
    int uniformAberrationLocation = -1;
    float uniformAberrationValue = 0.0f;
    
    Profiler profiler;
    WaveController wave;

    float trauma;
    raylib::Vector2 cameraShakeOffset;
    float cameraShakeRotation;

    bool isGUIBuilt;
    bool isViewportDefined;

    void setupWindow();
    void loop();

    void updateAsteroids();
    void updateBullets();

    void setupStars();

    void setupGUI();
    void buildGUIDockSpace();

    void updateStars(raylib::Vector2 &offset);
	void update();
	void render();
    void renderGUI();
    void onResize(uint32_t width, uint32_t height);
    void updateScreenShake();

  public:
    SpatialHashGrid grid;
    std::list<Bullet> bullets;
    std::vector<Star> stars;
    Ship ship;

    App();
    ~App();

    void setup();
    void run();
    void shake(float amount);
};