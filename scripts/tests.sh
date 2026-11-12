#!/bin/bash
# Builds everything in Release and runs the test suite. Works from any directory.
# Usage: scripts/tests.sh   (Windows: run from Git Bash or WSL)
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
SOURCE_DIR="$PROJECT_ROOT/BasicRenderer"
BUILD_DIR="$PROJECT_ROOT/build"
BUILD_TYPE=Release

# See build.sh for why both CMAKE_BUILD_TYPE and --config/-C are passed.
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build "$BUILD_DIR" --config "$BUILD_TYPE"
cd "$BUILD_DIR"
ctest -C "$BUILD_TYPE" --output-on-failure
