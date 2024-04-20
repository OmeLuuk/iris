# Ray Tracer Project

## Overview
This project implements a basic CPU-based ray tracer using C++ and XCB on Ubuntu. The ray tracer supports simple scenes with spheres and incorporates basic ray tracing techniques like shadows and reflections.

## Features

- **CPU-Based Rendering**: Pure CPU implementation for initial simplicity and learning.
- **Sphere Rendering**: Ability to render spheres with simple lighting.
- **Reflections**: Basic reflections demonstrating ray bouncing.
- **2D Debug View**: Top-down debug view showing ray paths.

## Getting Started

### Prerequisites

- A compatible C++ compiler (C++20 or later recommended)
- XCB library installed for window and graphics handling
- Basic knowledge of ray tracing concepts

### Building the Project

Ensure you have `g++` and `libxcb1-dev` installed:
```
sudo apt install g++ libxcb1-dev
sudo apt-get install pkg-config
```