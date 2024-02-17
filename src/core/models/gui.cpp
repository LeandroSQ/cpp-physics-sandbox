#include "app.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "../utils.hpp"
#include "raylib.h"
#include "rlImGui.h"

void App::setupGUI() {
    TraceLog(LOG_INFO, "App::setupGUI()");

    rlImGuiSetup(true);
    auto &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Ensure the ini file is in the same directory as the executable
    std::string cwd = GetApplicationDirectory();
    cwd += "imgui.ini";
    io.IniFilename = cwd.c_str();
#ifdef DEBUG
    if (raylib::FileExists(cwd)) {
        raylib::SaveFileText(cwd, "");
    }
#endif
}

void App::buildGUIDockSpace() {
    const auto workPosition = ImGui::GetMainViewport()->WorkPos;
    const auto workSize = ImGui::GetMainViewport()->WorkSize;
    const auto nodeSize = ImVec2 { (float)WIDTH,  (float)HEIGHT };
    const auto center = ImVec2 { workPosition.x + workSize.x / 2.0f, workPosition.y + workSize.y / 2.0f };
    const auto nodePos = ImVec2 { center.x - nodeSize.x / 2.0f, center.y - nodeSize.y / 2.0f };

    auto id = ImGui::GetID("MainDockSpace");
    ImGui::DockBuilderRemoveNode(id);
    ImGui::DockBuilderAddNode(id);

    ImGui::DockBuilderSetNodeSize(id, nodeSize);
    ImGui::DockBuilderSetNodePos(id, nodePos);

    auto dockTop = ImGui::DockBuilderSplitNode(id, ImGuiDir_Up, 0.20f, nullptr, &id);
    const auto dockBottom = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.80f, nullptr, &id);

    ImGui::DockBuilderDockWindow("Viewport", dockBottom);

    auto dockLeft = ImGui::DockBuilderSplitNode(dockTop, ImGuiDir_Left, 0.5f, nullptr, &dockTop);
    ImGui::DockBuilderDockWindow("Stats", dockLeft);

    dockLeft = ImGui::DockBuilderSplitNode(dockLeft, ImGuiDir_Right, 0.5f, nullptr, &dockLeft);
    ImGui::DockBuilderDockWindow("Ship", dockLeft);

    ImGui::DockBuilderDockWindow("Wave", dockTop);

    auto dockRight = ImGui::DockBuilderSplitNode(dockTop, ImGuiDir_Right, 0.5f, nullptr, &dockTop);
    ImGui::DockBuilderDockWindow("Bullets", dockRight);
    ImGui::DockBuilderDockWindow("Asteroids", dockRight);

    ImGui::DockBuilderFinish(id);

    isGUIBuilt = true;
}

void App::renderGUI() {
    rlImGuiBegin();

    if (!isGUIBuilt) buildGUIDockSpace();

    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    auto min = ImGui::GetWindowContentRegionMin();
    auto max = ImGui::GetWindowContentRegionMax();
    auto size = ImVec2(max.x - min.x, max.y - min.y);
    if ((frameBuffer.texture.id <= 0 || size.x != frameBuffer.texture.width || size.y != frameBuffer.texture.height) && size.x > 0 && size.y > 0) {
        onResize(size.x, size.y);
    } else if (tempFrameBuffer.IsReady()) rlImGuiImageRenderTexture(&tempFrameBuffer);
    ImGui::End();

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %i", GetFPS());
    ImGui::Text("Frame Time: %.2f ms", GetFrameTime() * 1000);

    ImGui::SliderFloat("Tension:", &trauma, 0.0f, 1.0f);

    /* int targetFps = TARGET_FPS;
    if (ImGui::SliderInt("Target FPS", &targetFps, 0, 100)) {
        TARGET_FPS = targetFps;
        SetTargetFPS(targetFps);
    } */
    ImGui::Checkbox("Render grid overlay", &RENDER_GRID);
    ImGui::Checkbox("Render star halo", &RENDER_STAR_HALO);

    profiler.renderGUI();

    ImGui::End();

    // TODO: Move this to Ship::renderGUI()
    ImGui::Begin("Ship");
    /* ImGui::SliderFloat("SHIP_ANGULAR_ACCELERATION", &SHIP_ANGULAR_ACCELERATION, 0.0f, PI * 2.0f);
    ImGui::SliderFloat("SHIP_MAX_ANGULAR_VELOCITY", &SHIP_MAX_ANGULAR_VELOCITY, 0.0f, PI * 2.0f);
    ImGui::SliderFloat("SHIP_ACCELERATION", &SHIP_ACCELERATION, 10.0f, 300.0f);
    ImGui::SliderFloat("SHIP_DAMPING", &SHIP_DAMPING, 0.0f, 1.0f);
    ImGui::SliderFloat("SHIP_ANGULAR_DAMPING", &SHIP_ANGULAR_DAMPING, 0.0f, 1.0f); */

    ImGui::Text("Position: { %.2f, %.2f }", ship.position.x, ship.position.y);
    ImGui::Text("Velocity: { %.2f, %.2f }", ship.velocity.x, ship.velocity.y);
    ImGui::Text("Acceleration: { %.2f, %.2f }", ship.acceleration.x, ship.acceleration.y);
    ImGui::Text("Angle: %.2f", ship.angle * RAD2DEG);
    ImGui::Text("Angular Velocity: %.2f", ship.angularVelocity * RAD2DEG);
    ImGui::End();

    ImGui::Begin("Asteroids");
    ImGui::Text("Alive: %i", grid.size());
    ImGui::Text("Collisions: %i", Asteroid::collisionCount);
    if (ImGui::Button("Kill all")) grid.clear();

    // int i = 0;
    // for (auto &asteroid : asteroids.all()) {
    // TODO: Move this to Asteroid::renderGUI()
    //     if (ImGui::TreeNode((void *)(intptr_t)i, "Asteroid %i", i)) {
    //         ImGui::Text("Position: { %.2f, %.2f }", asteroid->position.x, asteroid->position.y);
    //         ImGui::Text("Velocity: { %.2f, %.2f }", asteroid->velocity.x, asteroid->velocity.y);
    //         ImGui::Text("Angle: %.2f", asteroid->angle * RAD2DEG);
    //         ImGui::Text("Angular Velocity: %.2f", asteroid->angularVelocity * RAD2DEG);
    //         ImGui::Text("Radius: %.2f", asteroid->polygon.outerRadius);
    //         ImGui::Text("Vertices: %zu", asteroid->polygon.vertices.size());
    //         for (size_t j = 0; j < asteroid->polygon.vertices.size(); j++) {
    //             ImGui::Text("Vertex %zu: { %.2f, %.2f }", j, asteroid->polygon.vertices[j].x, asteroid->polygon.vertices[j].y);
    //         }
    //         ImGui::TreePop();
    //     }

    //     i++;
    // }
    ImGui::End();

    ImGui::Begin("Bullets");
    ImGui::Text("Alive: %zu", bullets.size());
    // i = 0;
    // for (auto &bullet : bullets) {
    // TODO: Move this to Bullet::renderGUI()
    //     if (ImGui::TreeNode((void *)(intptr_t)i, "Bullet %i", i)) {
    //         ImGui::Text("Position: { %.2f, %.2f }", bullet.position.x, bullet.position.y);
    //         ImGui::Text("Velocity: { %.2f, %.2f }", bullet.velocity.x, bullet.velocity.y);
    //         ImGui::TreePop();
    //     }

    //     i++;
    // }
    ImGui::End();

    ImGui::Begin("Wave");
    wave.renderGUI();
    ImGui::End();

    rlImGuiEnd();
}