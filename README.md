
<img width="818" height="644" alt="Снимок экрана 2025-11-03 в 11 06 15" src="https://github.com/user-attachments/assets/c1e77385-95ad-4547-a7c8-fe49f00df64d" />

# Hello Triangle - SDL3 with OpenGL Coordinates

A modern C++ demonstration project showcasing SDL3 graphics programming with proper coordinate system transformations and callback-based application architecture. I use vibe coding)

## Features

- **SDL3 Integration**: Utilizes the latest SDL3 API with callback-driven application lifecycle
- **OpenGL-style Coordinates**: Implements a coordinate system similar to OpenGL with center at (0,0) and normalized ranges
- **Hardware-Accelerated Rendering**: Uses SDL_RenderGeometry for efficient triangle rendering
- **Delta Time Calculation**: Real-time frame timing for smooth animations and performance monitoring
- **Cross-Platform**: Compatible with Windows, macOS, and Linux
- **Modern C++**: Uses contemporary C++ features and smart memory management

## Project Structure

```
hello_triangle/
├── CMakeLists.txt        # Build configuration
├── main.cpp              # Main application source
└── README.md             # This file
```

## Requirements

- **SDL3** (latest development version)
- **GLM** (OpenGL Mathematics library)
- **C++17** compatible compiler
- **CMake** 3.16+

## Building

### Linux/macOS
```bash
mkdir build && cd build
cmake ..
make
./HelloTriangleSDL3
```

### Windows
```bash
mkdir build && cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```

## Dependencies Installation

### Ubuntu/Debian
```bash
sudo apt-get install libsdl3-dev libglm-dev cmake build-essential
```

### macOS
```bash
brew install sdl3 glm cmake
```

### Windows (vcpkg)
```bash
vcpkg install sdl3 glm
```

## Architecture

The project demonstrates SDL3's callback-based architecture:

- **SDL_AppInit**: Initializes application state, window, and renderer
- **SDL_AppEvent**: Handles user input and window events
- **SDL_AppIterate**: Main rendering loop with delta time calculation
- **SDL_AppQuit**: Cleanup and resource management

## Coordinate System

The application transforms from OpenGL-style coordinates to SDL screen coordinates:
- **OpenGL**: [-1, 1] × [-1, 1] with center at (0, 0)
- **SDL**: [0, width] × [0, height] with center at (width/2, height/2)

## Key Features Demonstrated

1. **Window Management**: Resizable window with proper coordinate updates
2. **Renderer Information**: Automatic detection and logging of graphics backend
3. **Input Handling**: Keyboard (ESC to quit) and mouse input
4. **Performance Monitoring**: Real-time FPS and delta time calculation
5. **Memory Management**: Proper resource cleanup using RAII principles

## Controls

- **ESC**: Exit application
- **Window Resize**: Automatically adjusts coordinate system
- **Mouse Click**: Logs coordinates relative to center

## Performance

The application includes built-in performance monitoring:
- Real-time FPS counter
- Delta time calculation for frame-independent animations
- Efficient rendering using SDL's geometry API

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
