#!/bin/bash
# Runs BasicRenderer, passing all arguments through to it. If the executable
# hasn't been built yet, builds it first (Release) with build.sh.
# Works from any directory: relative paths in the arguments are resolved from
# where you call it. Assets are located relative to the executable.
# Usage: scripts/run.sh [BasicRenderer args...]   e.g. scripts/run.sh --help
# (Windows: run from Git Bash or WSL)
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
# CMakeLists.txt outputs the executable to build/bin for every configuration
BIN_DIR="$PROJECT_ROOT/build/bin"

find_executable() {
    for candidate in "$BIN_DIR/BasicRenderer" "$BIN_DIR/BasicRenderer.exe"; do
        if [[ -f "$candidate" ]]; then
            echo "$candidate"
            return 0
        fi
    done
    return 1
}

if ! EXECUTABLE="$(find_executable)"; then
    echo "BasicRenderer is not built yet, building it first..." >&2
    # Build log goes to stderr so the renderer's stdout stays clean for piping
    bash "$SCRIPT_DIR/build.sh" >&2
    EXECUTABLE="$(find_executable)" || {
        echo "Error: build finished but no executable was found in $BIN_DIR" >&2
        exit 1
    }
fi

exec "$EXECUTABLE" "$@"
