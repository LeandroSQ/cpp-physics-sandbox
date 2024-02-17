#!/bin/bash

set -e

# Get current directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SOURCE_DIR=$DIR/assets
TARGET_DIR=$DIR/build/assets

# Check if there is already a symlink to the assets directory
if [ -L $TARGET_DIR ]; then
    echo "Symlink already exists"
else
    echo "Creating symlink"
    ln -s $SOURCE_DIR $TARGET_DIR

    if [ $? -eq 0 ]; then
        echo "Symlink created"
    else
        echo "Symlink creation failed"
    fi
fi