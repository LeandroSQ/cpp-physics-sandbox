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

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_C_STANDARD 11)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_STANDARD 23)

    # For clang use -std=c++23
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

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
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PRECOMPILED_HEADERS})

# Link dependencies
target_link_libraries(${PROJECT_NAME} raylib)
target_link_libraries(${PROJECT_NAME} raylib_cpp)
target_link_libraries(${PROJECT_NAME} dear_imgui)
target_link_libraries(${PROJECT_NAME} rlimgui)

if(APPLE) # Checks if OSX and links appropriate frameworks (Only required on MacOS)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()


if(PLATFORM STREQUAL "Web") # Web Configurations
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()
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

# region Clean
add_custom_target(${PROJECT_NAME}_clean
    COMMAND sh ${CMAKE_SOURCE_DIR}/scripts/clean.sh
    COMMENT "Running clean.sh"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
# endregion

# region Testing
# enable_testing()
# add_subdirectory(tests)
# endregion