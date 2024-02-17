#pragma once

#include "../precomp.hpp"

class Profiler {
  private:
    std::array<float, 100> frameTimeBuffer;
    size_t frameTimeIndex = 0;

    uint32_t frameCount = 0;
    double frameTimeSum = 0.0;

    std::chrono::time_point<std::chrono::high_resolution_clock> frameStart;

  public:
    Profiler() = default;
    ~Profiler() = default;

    void onFrameStart() {
        frameStart = std::chrono::high_resolution_clock::now();
    }

    void onFrameEnd() {
        const auto frameEnd = std::chrono::high_resolution_clock::now();
        const double frameTime = std::chrono::duration<double>(frameEnd - frameStart).count();

        if (frameTimeIndex >= frameTimeBuffer.size()) frameTimeIndex = 0;
        frameTimeBuffer[frameTimeIndex++] = frameTime;
        frameTimeSum += frameTime;
        frameCount++;
    }

    void renderGUI() {
        const float averageFrameTime = frameTimeSum / frameCount;

        // ImGui::Begin("Profiler");
        ImGui::Text("Average frame time: %.3f ms", averageFrameTime * 1000.0f);
        ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::PlotLines(
            "Frame Time",
            frameTimeBuffer.data(),
            frameTimeBuffer.size(),
            0,
            NULL,
            0.0f,
            0.033f,
            ImVec2(0, 20)
        );
        ImGui::PopStyleColor();
        // ImGui::End();
    }
};