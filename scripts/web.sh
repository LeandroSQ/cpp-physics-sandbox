#!/bin/bash

set -e

cmake --preset=web-release .
cmake --build ./build/web --target all -j 10
if [ $? -eq 0 ]; then
    echo "Build successful"
else
    echo "Build failed"
    exit 1
fi

cp ./build/web/asteroids.* ./src/web