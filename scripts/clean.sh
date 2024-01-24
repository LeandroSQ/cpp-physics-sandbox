#!/bin/bash

set -e

# cmake -build ./build --target clean
rm -rf CMakeFiles
rm -rf build

# Remove everything under ./lib except for CMakelists.txt, files and folders
find ./lib -type f ! -name 'CMakeLists.txt' -delete
find ./lib -mindepth 1 -type d ! -name 'CMakeLists.txt' -exec rm -rf {} +