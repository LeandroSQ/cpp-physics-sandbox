#!/bin/bash

set -e

# Check if MacOS
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "MacOS detected"
    # Check if emsdk is installed via homebrew
    if brew ls --versions emscripten > /dev/null; then
        echo "emsdk detected"
        EMSDK_PATH=$(brew --prefix emscripten)/libexec
        echo "emsdk path: $EMSDK_PATH"
    else
        echo "emsdk not detected"
        echo "Installing emsdk"
        brew install emscripten
    fi
else
    echo "This script is only for MacOS"
    exit 1
fi

# Validate toolchain file path
TOOLCHAIN="$EMSDK_PATH/cmake/Modules/Platform/Emscripten.cmake"
echo "Toolchain path: $TOOLCHAIN"
if [ ! -f "$TOOLCHAIN" ]; then
    echo "Emscripten toolchain not found"
    exit 1
fi

# Validate include directory
INCLUDE_DIR="$EMSDK_PATH/cache/sysroot/include"
echo "Include directory: $INCLUDE_DIR"
if [ ! -d "$INCLUDE_DIR" ]; then
    echo "Emscripten include directory not found"
    exit 1
fi

# Clear cmake cache
echo "Clearing cmake cache..."
rm -rf ./build

# Configure
echo "Configuring..."
cmake -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" -DEMSDK_INCLUDE="$INCLUDE_DIR" -DCMAKE_BUILD_TYPE=Release -DPLATFORM=Web -B ./build -S .

# Build
echo "Building..."
cmake --build ./build --config Release
