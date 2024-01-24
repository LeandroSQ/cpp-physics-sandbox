#!/bin/bash

set -e

cmake --build ./build -j 10

# Check if the previous command succeeded
if [ $? -eq 0 ]; then
    echo "Build succeeded"
    ./build/fire
else
    echo "Build failed"
fi