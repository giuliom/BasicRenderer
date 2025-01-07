#!/bin/bash

echo "Building BasicRenderer"
cmake -S . -B build
cmake --build build --config release --target BasicRenderer