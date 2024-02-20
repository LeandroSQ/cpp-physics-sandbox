cmake_minimum_required(VERSION 3.18.0)
set(PROJECT_NAME asteroids)

function(dump_info)
    message(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
    message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
    message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
    message(STATUS "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
    message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "CMAKE_C_COMPILER_VERSION: ${CMAKE_C_COMPILER_VERSION}")
    message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

    if((UNIX AND NOT APPLE) OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR CMAKE_SYSTEM MATCHES "Linux")
        set(LINUX TRUE)
    endif()

    if(NOT WIN32 AND (MSVC OR MSYS OR MINGW) OR ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR CMAKE_SYSTEM MATCHES "Windows")
        set(WIN32 TRUE)
    endif()

    if(NOT APPLE AND (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" OR CMAKE_SYSTEM MATCHES "Darwin"))
        set(APPLE TRUE)
    endif()

    if(WIN32)
        message(STATUS "OS: WIN32")
    elseif(APPLE)
        message(STATUS "OS: APPLE")
    elseif(LINUX)
        message(STATUS "OS: LINUX")
    elseif(UNIX)
        message(STATUS "OS: UNIX")
    else()
        message(FATAL_ERROR "OS: UNKNOWN OS")
    endif()

endfunction()

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

    if(NOT CI)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    endif()

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
    if(NOT LINUX OR NOT PLATFORM STREQUAL "Web")
        target_precompile_headers(${target} PRIVATE ${PRECOMPILED_HEADERS})
    endif()

    # Link dependencies
    target_link_libraries(${target} raylib raylib_cpp dear_imgui rlimgui)

    if(APPLE) # Checks if OSX and links appropriate frameworks (Only required on MacOS)
        target_link_libraries(${target} "-framework IOKit")
        target_link_libraries(${target} "-framework Cocoa")
        target_link_libraries(${target} "-framework OpenGL")
    endif()

    if(PLATFORM STREQUAL "Web") # Web Configurations
        target_include_directories(${target} PUBLIC ${EMSDK_INCLUDE})

        # https://github.com/clangd/clangd/issues/1621#issuecomment-1703330511
        # Fix for generating compile_commands.json for clangd
        # Ignore if on github actions runner
        if(NOT CI)
            execute_process(COMMAND em++ --cflags OUTPUT_VARIABLE EM_CFLAGS)
            set_target_properties(${target} PROPERTIES COMPILE_FLAGS "${EM_CFLAGS}")
        endif()
    endif()

    if(LINX AND PLATFORM STREQUAL "Web")
        set_target_properties(${PROJECT_NAME} PROPERTIES PCH_WARN_INVALID OFF)
    endif()
endfunction()

# Define project
define_project_variables()
project(${PROJECT_NAME})

# Print information about the current platform
dump_info()

if(PLATFORM STREQUAL "Web")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # Add ${EMSDK_INCLUDE} to include directories for compile_commands.json
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${EMSDK_INCLUDE} -I${CMAKE_SOURCE_DIR}/src/core")
endif()

# Source files
file(GLOB_RECURSE SOURCES "src/core/*.cpp" "src/core/*.hpp")
set(PRECOMPILED_HEADERS "src/core/precomp.hpp")
if(LINUX AND PLATFORM STREQUAL "Web")
    set(CMAKE_PCH_INSTANTIATE_TEMPLATES OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-pch-instantiate-templates")
else()
    set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
endif()

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
)# endregion
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