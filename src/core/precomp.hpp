#pragma once

// Raylib
#include <raylib.h>
#include <raylib-cpp.hpp>

// Emscripten
#ifdef PLATFORM_WEB
 #include <emscripten/emscripten.h>
 #include <emscripten/html5.h>
#else
#endif

// GUI
#include <imgui.h>
#include <rlImGui.h>

// STL
#include <algorithm>
#include <iostream>
#include <memory>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
