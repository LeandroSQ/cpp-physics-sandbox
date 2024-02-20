#pragma once

#include "precomp.hpp"
#include "settings.hpp"

inline raylib::Vector2 getRelativeMousePosition() {
    const auto dpi = GetWindowScaleDPI();
    return raylib::Vector2(
        std::clamp(GetMouseX() - GetScreenWidth() / 2.0f + WIDTH / 2.0f, 0.0f, (float)WIDTH),
        std::clamp(GetMouseY() - GetScreenHeight() / 2.0f + HEIGHT / 2.0f, 0.0f, (float)HEIGHT)
    );
}

inline ImVec2 convert(raylib::Vector2 a) {
	return { a.x, a.y };
}

inline ImU32 convert(raylib::Color a) {
	return IM_COL32(a.r, a.g, a.b, a.a);
}

inline ImU32 convert(ImVec4 a) {
	return IM_COL32((uint8_t)(a.x * 255.0f), (uint8_t)(a.y * 255.0f), (uint8_t)(a.z * 255.0f), (uint8_t)(a.w * 255.0f));
}