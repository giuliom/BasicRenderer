#!/bin/bash

# Default to Release if no argument provided
BUILD_TYPE=${1:-Release}

# Validate build type
if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo "Usage: ./build.sh [Debug|Release]"
    echo "  Default: Release"
    exit 1
fi

echo "Building BasicRenderer ($BUILD_TYPE)"
cmake -S . -B build -DCMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build build --config $BUILD_TYPE --target BasicRenderer