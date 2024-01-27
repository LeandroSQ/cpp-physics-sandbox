#pragma once

// #include <raylib.h>
// #include <raymath.h>

#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
 #include <emscripten/emscripten.h>
#endif

#include <algorithm>
#include <iostream>
#include <memory>
#include <cstdio>
#include <string>
#include <vector>

#include "constants.hpp"
#include "palette.hpp"

#ifdef MULTI_THREADING
 #include <omp.h>
 #include <execution>
 #include <future>
#endif
