#include "app.hpp"
#include "asteroid.hpp"
#include "imgui.h"
#include "raylib.h"

void App::renderGUI() {
    rlImGuiBegin();

    ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Viewport");
    if (frameBuffer.IsReady()) rlImGuiImageRenderTexture(&frameBuffer);
    auto min = ImGui::GetWindowContentRegionMin();
    auto max = ImGui::GetWindowContentRegionMax();
    auto size = ImVec2(max.x - min.x, max.y - min.y);
    if (frameBuffer.texture.id <= 0 || size.x != frameBuffer.texture.width || size.y != frameBuffer.texture.height) {
        onResize(size.x, size.y);
    }
    ImGui::End();

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %i", GetFPS());
    ImGui::Text("Frame Time: %.2f ms", GetFrameTime() * 1000);
    ImGui::End();

    ImGui::Begin("Ship");
    ImGui::Text("Position: { %.2f, %.2f }", ship.position.x, ship.position.y);
    ImGui::Text("Velocity: { %.2f, %.2f }", ship.velocity.x, ship.velocity.y);
    ImGui::Text("Acceleration: { %.2f, %.2f }", ship.acceleration.x, ship.acceleration.y);
    ImGui::Text("Angle: %.2f", ship.angle * RAD2DEG);
    ImGui::Text("Angular Velocity: %.2f", ship.angularVelocity * RAD2DEG);
    ImGui::End();

    ImGui::Begin("Asteroids");
    ImGui::Text("Alive: %i", asteroids.size());
    ImGui::Text("Collisions: %i", Asteroid::collisionCount);
    if (ImGui::Button("Kill all")) {
        asteroids.clear();
    }
    int i = 0;
    for (auto &asteroid : asteroids.all()) {
        if (ImGui::TreeNode((void *)(intptr_t)i, "Asteroid %i", i)) {
            ImGui::Text("Position: { %.2f, %.2f }", asteroid->position.x, asteroid->position.y);
            ImGui::Text("Velocity: { %.2f, %.2f }", asteroid->velocity.x, asteroid->velocity.y);
            ImGui::Text("Angle: %.2f", asteroid->angle * RAD2DEG);
            ImGui::Text("Angular Velocity: %.2f", asteroid->angularVelocity * RAD2DEG);
            ImGui::Text("Radius: %.2f", asteroid->polygon.outerRadius);
            ImGui::Text("Vertices: %zu", asteroid->polygon.vertices.size());
            for (size_t j = 0; j < asteroid->polygon.vertices.size(); j++) {
                ImGui::Text("Vertex %zu: { %.2f, %.2f }", j, asteroid->polygon.vertices[j].x, asteroid->polygon.vertices[j].y);
            }
            ImGui::TreePop();
        }

        i++;
    }
    ImGui::End();

    ImGui::Begin("Bullets");
    ImGui::Text("Alive: %zu", bullets.size());
    i = 0;
    for (auto &bullet : bullets) {
        if (ImGui::TreeNode((void *)(intptr_t)i, "Bullet %i", i)) {
            ImGui::Text("Position: { %.2f, %.2f }", bullet.position.x, bullet.position.y);
            ImGui::Text("Velocity: { %.2f, %.2f }", bullet.velocity.x, bullet.velocity.y);
            ImGui::TreePop();
        }

        i++;
    }
    ImGui::End();

    ImGui::Begin("Wave");
    ImGui::Text("Current: %i", wave.currentWave);
    ImGui::Text("Time Since Last Wave: %.2f", wave.timeSinceLastWave);
    ImGui::Text("Time Since Last Spawn: %.2f", wave.timeSinceLastSpawn);
    ImGui::Text("Asteroids Spawned: %i", wave.asteroidsSpawned);
    ImGui::End();

    rlImGuiEnd();
}