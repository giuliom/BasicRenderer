# BasicRenderer

A simple renderer

![Build and Tests](https://github.com/giuliom/BasicRenderer/actions/workflows/CI.yml/badge.svg?branch=main)

![Screenshot](basicrenderer.jpeg?raw=true "Screenshot")

## Description

Basic Renderer is a simple renderer made with fun and learning in mind.

Development is currently on hold.

Features planned:
* Rasterization
* Classic Ray-Tracing
* Path-Tracing

The solution includes two projects:
- BasicRenderer aka the actual renderer itself that can be built as standalone executable
- QtGUI a graphical interface for BasicRenderer made with Qt 5.15.0


## Getting Started

### BasicRenderer
The project root folder contains a BasicRenderer.sln Visual Studio file but this project is made to use CMake
- Install CMake
- Use the scripts in the `scripts` folder (on Windows run them from Git Bash or WSL):
  - `scripts/build.sh [Debug|Release]` builds the executable (default Release)
  - `scripts/run.sh [args...]` runs the executable, building it first if needed, e.g. `scripts/run.sh --help`
  - `scripts/tests.sh` builds and runs the tests
- Otherwise CMake can be used directly on the BasicRenderer folder, these targets are provided:
  - The library `BasicRendererLib`
  - The executable `BasicRenderer`
  - The Google Test executable `BasicRendererTest`

### Scene File Format
Support for a very simple json base scene file format is provided, see the scene included in the assets folder for an example

### QtGUI
Due to time constraints this project only supports Windows
- Install Qt 5.15.0
- Open BasicRenderer.sln with Visual Studio
- Build and Run the QtGUI project
