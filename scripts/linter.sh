#!/bin/bash

set -e

cmake --build ./build -j 10

# Check if the previous command succeeded
if [ $? -eq 0 ]; then
    echo "Build succeeded... running linter!"
    cmake --build ./build --target asteroids_lint
else
    echo "Build failed"
fi