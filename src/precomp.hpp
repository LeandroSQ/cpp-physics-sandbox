#pragma once

// #include <raylib.h>
// #include <raymath.h>

#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
 #include <emscripten/emscripten.h>
#endif

// #include <magic_enum_all.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "constants.hpp"

#ifdef MULTI_THREADING
 #include <omp.h>
 #include <execution>
 #include <future>
#endif
