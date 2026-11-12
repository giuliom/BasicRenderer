#!/bin/bash
# Builds the BasicRenderer executable. Works from any directory.
# Usage: scripts/build.sh [Debug|Release]   (Windows: run from Git Bash or WSL)
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
SOURCE_DIR="$PROJECT_ROOT/BasicRenderer"
BUILD_DIR="$PROJECT_ROOT/build"

# Default to Release if no argument provided
BUILD_TYPE=${1:-Release}

# Validate build type
if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo "Usage: scripts/build.sh [Debug|Release]"
    echo "  Default: Release"
    exit 1
fi

echo "Building BasicRenderer ($BUILD_TYPE)"
# No -G: CMake picks the platform default generator (Visual Studio on Windows,
# Unix Makefiles on macOS/Linux). CMAKE_BUILD_TYPE covers single-config
# generators, --config covers multi-config ones like Visual Studio.
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE" --target BasicRenderer
