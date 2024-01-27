cmake_minimum_required(VERSION 3.15.0)
set(PROJECT_NAME sandbox)

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
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )

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

function(define_target_properties target)
    target_compile_features(${target} PRIVATE cxx_std_23)

    # Precompiled headers
    target_precompile_headers(${target} PRIVATE ${PRECOMPILED_HEADERS})

    # Link dependencies
    target_link_libraries(${target} raylib)
    target_link_libraries(${target} raylib_cpp)
    target_link_libraries(${target} dear_imgui)
    target_link_libraries(${target} rlimgui)

    if(APPLE) # Checks if OSX and links appropriate frameworks (Only required on MacOS)
        target_link_libraries(${target} "-framework IOKit")
        target_link_libraries(${target} "-framework Cocoa")
        target_link_libraries(${target} "-framework OpenGL")
    endif()

    if(PLATFORM STREQUAL "Web") # Web Configurations
        target_include_directories(${PROJECT_NAME} PRIVATE ${EMSDK_INCLUDE})
    endif()
endfunction()

# Define project
define_project_variables()
project(${PROJECT_NAME})

if(PLATFORM STREQUAL "Web")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Source files
file(GLOB_RECURSE SOURCES "src/core/*.cpp" "src/core/*.hpp")
set(PRECOMPILED_HEADERS "src/core/precomp.hpp")

# Dependencies
add_subdirectory(lib)

add_executable(${PROJECT_NAME} ${SOURCES})
define_target_properties(${PROJECT_NAME})

# region Linter
file(GLOB_RECURSE LINT_SOURCES "src/core/*.cpp" "src/core/*.hpp")
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