set(PROJECT_NAME fire)

function(define_project_variables)
    # https://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
    # Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
    if(POLICY CMP0025)
        cmake_policy(SET CMP0025 NEW)
    endif()

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        message("Debug build.")
        add_definitions(-DDEBUG)
    elseif(CMAKE_BUILD_TYPE MATCHES Release)
        message("Release build.")
        add_definitions(-DRELEASE)
    endif()

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

    # For OpenMP on MacOS M1, include the llvm toolchain from homebrew
    if (APPLE)
        set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
        set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
    endif()

    # set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
    # set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
    # set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_C_STANDARD 11)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_STANDARD 23)

    cmake_minimum_required(VERSION 3.15.0)
endfunction()

# Define project
define_project_variables()
project(${PROJECT_NAME})

# region Main target\
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
set(PRECOMPILED_HEADERS "src/precomp.hpp")

# Dependencies
add_subdirectory(lib)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PRECOMPILED_HEADERS})

# Link dependencies
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} magic_enum)
target_link_libraries(${PROJECT_NAME} raylib_cpp)

if(APPLE) # Checks if OSX and links appropriate frameworks (Only required on MacOS)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

    # Include libomp for OpenMP on MacOS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xpreprocessor -fopenmp -lomp")
    target_include_directories(${PROJECT_NAME} PRIVATE "/opt/homebrew/opt/libomp/include")
    target_link_directories(${PROJECT_NAME} PRIVATE "/opt/homebrew/opt/libomp/lib")
endif()

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
# find_package(OpenMP REQUIRED)
# target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)


if(PLATFORM STREQUAL "Web") # Web Configurations
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# endregion

# region Linter
file(GLOB_RECURSE LINT_SOURCES "src/*.cpp" "src/*.hpp")

add_custom_target(${PROJECT_NAME}_lint
    COMMAND clang-tidy ${LINT_SOURCES}
    COMMENT "Running clang-tidy"
    DEPENDS ${LINT_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
# endregion

# region Testing
# enable_testing()
# add_subdirectory(tests)
# endregion