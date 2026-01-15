#!/bin/bash

# Get the directory of the script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Default to Release if no argument provided
BUILD_TYPE=${1:-Release}

# Validate build type
if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo "Usage: ./build.sh [Debug|Release]"
    echo "  Default: Release"
    exit 1
fi

echo "Building BasicRenderer ($BUILD_TYPE)"
cmake -S "$SCRIPT_DIR" -B "$PROJECT_ROOT/build" -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build "$PROJECT_ROOT/build" --config $BUILD_TYPE --target BasicRenderer