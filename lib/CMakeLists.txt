cmake_minimum_required(VERSION 3.15.0)

# Definitions
set(TEMP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/temp)
set(INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Function to download and include a library, given name, version/revision and source
function(include_library name version source)
    # Check if the provided version is valid 0.0.0 format, useful for branches or commits
    string(REGEX MATCH "^[0-9]+\\.[0-9]+(\\.[0-9]+)?$" version_match ${version})
    if (version_match)
        find_package(${name} ${version} QUIET)
    else()
        find_package(${name} QUIET)
    endif()

    if (NOT ${name}_FOUND)
        message(STATUS "Library ${name} ${version} not defined before, downloading and installing it")

        # Check if already downloaded
        if(EXISTS ${INSTALL_PATH}/${name}-${version})
            message(STATUS "Library ${name} ${version} already downloaded")
        else()
            # Replace "VERSION" in source with version
            string(REPLACE "VERSION" ${version} source ${source})

            # Replace "-" in name with "_"
            string(REPLACE "-" "_" name ${name})

            # Check if temp folder exists and if it has the library
            if (EXISTS ${TEMP_PATH}/${name}-${version}.tar.gz)
                message(STATUS "Library ${name} ${version} already downloaded")
            else()
                # Download
                message(STATUS "Downloading library ${name} ${version}")
                file(DOWNLOAD ${source} ${TEMP_PATH}/${name}-${version}.tar.gz)
            endif()

            # Create empty folder under TEMP_PATH to unzip everything diregarding the top level root folder
            file(MAKE_DIRECTORY ${TEMP_PATH}/${name}-${version})

            # Unzip
            message(STATUS "Unzipping library ${name} ${version} to ${INSTALL_PATH}/${name}-${version}")
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E tar xzf ${TEMP_PATH}/${name}-${version}.tar.gz
                WORKING_DIRECTORY ${TEMP_PATH}/${name}-${version}
            )

            # Check if there is a top level folder
            message(STATUS "Moving library ${name} ${version} to ${INSTALL_PATH}/${name}-${version}")
            file(GLOB children RELATIVE ${TEMP_PATH}/${name}-${version} ${TEMP_PATH}/${name}-${version}/*)
            list(LENGTH children children_length)
            # Remove if exists
            if (EXISTS ${INSTALL_PATH}/${name}-${version})
                file(REMOVE_RECURSE ${INSTALL_PATH}/${name}-${version})
            endif()
            file(MAKE_DIRECTORY ${INSTALL_PATH}/${name}-${version})
            message(STATUS "Children: ${children}")
            if (children_length EQUAL 1)
                # Move everything inside the top level folder to the install path
                message(STATUS "Moving everything inside the top level folder to the install path. From ${TEMP_PATH}/${name}-${version}/${children} to ${INSTALL_PATH}/${name}-${version}")
                if(WIN32)
                    # On Windows, RENAME yields in a "Access is denied" error, so we use INSTALL and then REMOVE_RECURSE
                    file(COPY ${TEMP_PATH}/${name}-${version}/${children}/ DESTINATION ${INSTALL_PATH}/${name}-${version})
                    file(REMOVE_RECURSE ${TEMP_PATH}/${name}-${version})
                else()
                    file(RENAME ${TEMP_PATH}/${name}-${version}/${children} ${INSTALL_PATH}/${name}-${version})
                endif()
            else()
                # Move everything to the install path
                message(STATUS "Moving everything to the install path. From ${TEMP_PATH}/${name}-${version} to ${INSTALL_PATH}/${name}-${version}")
                file(RENAME ${TEMP_PATH}/${name}-${version} ${INSTALL_PATH}/${name}-${version})
            endif()
            file(REMOVE_RECURSE ${TEMP_PATH}/${name}-${version})
        endif()

        # Include
        if(EXISTS ${INSTALL_PATH}/${name}-${version}/CMakeLists.txt)
            message(STATUS "Including library's CMakeLists.txt ${name} ${version}")
            add_subdirectory(${INSTALL_PATH}/${name}-${version} ${CMAKE_CURRENT_BINARY_DIR}/${name}-${version})
        elseif(EXISTS ${INSTALL_PATH}/${name}-${version}/projects/CMake/CMakeLists.txt)
            message(STATUS "Including library's projects/CMake/CMakeLists.txt ${name} ${version}")
            add_subdirectory(${INSTALL_PATH}/${name}-${version}/projects/CMake ${CMAKE_CURRENT_BINARY_DIR}/${name}-${version})
        elseif(EXISTS ${INSTALL_PATH}/${name}-${version}/include)
            message(STATUS "Including library 'include' folder ${name} ${version}")
            add_library(${name} INTERFACE)
        else()
            # Check if on the root there is .h, .hpp, .c or .cpp files
            set(extensions .h .hpp .c .cpp)
            file(GLOB children ${INSTALL_PATH}/${name}-${version}/*.h ${INSTALL_PATH}/${name}-${version}/*.hpp ${INSTALL_PATH}/${name}-${version}/*.c ${INSTALL_PATH}/${name}-${version}/*.cpp)
            list(LENGTH children children_length)
            if (children_length EQUAL 0 AND NOT EXISTS ${INSTALL_PATH}/${name}-${version}/include)
                message(FATAL_ERROR "Library ${name} ${version} not found")
            endif()

            # Include the folder
            message(STATUS "Including library ${name} ${version} (no CMakeLists.txt found)")
            file(GLOB LIBRARY_SOURCES ${INSTALL_PATH}/${name}-${version}/*.c ${INSTALL_PATH}/${name}-${version}/*.cpp)
            file(GLOB LIBRARY_HEADERS ${INSTALL_PATH}/${name}-${version}/*.h ${INSTALL_PATH}/${name}-${version}/*.hpp)
            add_library(${name} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
            set_target_properties(${name} PROPERTIES
                CXX_STANDARD 11
                CXX_STANDARD_REQUIRED YES
                CXX_EXTENSIONS NO
            )
            target_include_directories(${name} PUBLIC ${INSTALL_PATH}/${name}-${version})
        endif()

        message(STATUS "Done installing library ${name} ${version}")
    else()
        message(STATUS "Library ${name} ${version} found, skipping")
    endif()
endfunction()

# Dependencies
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SUPPORT_RPRAND_GENERATOR ON)
include_library(raylib 5.0 https://github.com/raysan5/raylib/archive/refs/tags/VERSION.tar.gz)

set(BUILD_RAYLIB_CPP_EXAMPLES OFF CACHE BOOL "" FORCE)
include_library(raylib_cpp 5.0.1 https://github.com/RobLoach/raylib-cpp/archive/refs/tags/vVERSION.tar.gz)

# include_library(raygui 4.0 https://github.com/raysan5/raygui/archive/refs/tags/VERSION.tar.gz)

include_library(dear_imgui docking https://github.com/ocornut/imgui/archive/refs/heads/VERSION.tar.gz)
include_library(rlimgui main https://github.com/raylib-extras/rlimgui/archive/refs/heads/VERSION.tar.gz)
target_compile_definitions(rlimgui PRIVATE NO_FONT_AWESOME)
target_link_libraries(rlimgui dear_imgui raylib)

# Clean
# message(STATUS "Removing temporary files")
# file(REMOVE_RECURSE ${TEMP_PATH})