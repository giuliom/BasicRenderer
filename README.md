# BasicRenderer

A simple renderer

![Build and Tests](https://github.com/giuliom/BasicRenderer/actions/workflows/CI.yml/badge.svg?branch=main)

![Screenshot](basicrenderer.JPG?raw=true "Screenshot")

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
- Move into the BasicRenderer folder
- Run shell scripts `build.sh` and `run_tests.sh` to build the project and run the tests
- Otherwise CMake can be used directly, these targets are provided:
  - The library `BasicRendererLib`
  - The executable `BasicRenderer`
  - The Google Test executable `BasicRendererTest`

### QtGUI
Due to time constraints this project only supports Windows
- Install Qt 5.15.0
- Open BasicRenderer.sln with Visual Studio
- Build and Run the QtGUI project
