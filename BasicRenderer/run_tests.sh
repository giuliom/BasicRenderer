#!/bin/bash

# Get the directory of the script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

cmake -S "$SCRIPT_DIR" -B "$PROJECT_ROOT/build"
cmake --build "$PROJECT_ROOT/build" --config release
cd "$PROJECT_ROOT/build"
ctest -C release --output-on-failure