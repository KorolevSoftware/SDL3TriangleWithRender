
<img width="818" height="644" alt="Снимок экрана 2025-11-03 в 11 06 15" src="https://github.com/user-attachments/assets/c1e77385-95ad-4547-a7c8-fe49f00df64d" />

# Hello Triangle - SDL3 with OpenGL Coordinates

A modern C++ demonstration project showcasing SDL3 graphics programming with proper coordinate system transformations and callback-based application architecture. USE vibe coding)

## Project Structure

```
hello_triangle/
├── CMakeLists.txt          # Build configuration
├── main.cpp               # Main application source
└── README.md             # This file
```

## Requirements

- **SDL3** (latest development version)
- **GLM** (OpenGL Mathematics library)
- **C++17** compatible compiler
- **CMake** 3.16+

## Coordinate System Transformation

### Transformation Function: `update_transform_matrix`

The core of the coordinate conversion is handled by this function:

```cpp
void update_transform_matrix(AppState* state) {
    state->transform = glm::mat4(1.0f);
    
    // 1. First, shift (0,0) to the center of the window
    state->transform = glm::translate(state->transform, glm::vec3(
        state->window_width / 2.0f,   // X shift by half width
        state->window_height / 2.0f,  // Y shift by half height
        0.0f
    ));
    
    // 2. Then scale to window dimensions
    state->transform = glm::scale(state->transform, glm::vec3(
        state->window_width / 2.0f,   // X scale
        -state->window_height / 2.0f, // Y scale (negative for Y inversion)
        1.0f
    ));
}
```

### Input Data: OpenGL Coordinate Vertices

The triangle vertices are defined in OpenGL normalized device coordinates:

```cpp
glm::vec2 opengl_vertices[3] = {
    glm::vec2(0.0f,  0.5f),   // Top vertex
    glm::vec2(0.5f, -0.5f),   // Bottom-right vertex  
    glm::vec2(-0.5f, -0.5f)   // Bottom-left vertex
};
```

### Vertex Colors

Each vertex has an associated color for gradient rendering:

```cpp
SDL_FColor vertex_colors[3] = {
    { 1.0f, 0.0f, 0.0f, 1.0f },   // Red - Top vertex
    { 0.0f, 1.0f, 0.0f, 1.0f },   // Green - Bottom-right vertex
    { 0.0f, 0.0f, 1.0f, 1.0f }    // Blue - Bottom-left vertex
};
```

## Features

- **SDL3 Integration**: Utilizes the latest SDL3 API with callback-driven application lifecycle
- **OpenGL-style Coordinates**: Implements a coordinate system similar to OpenGL with center at (0,0) and normalized ranges
- **Hardware-Accelerated Rendering**: Uses SDL_RenderGeometry for efficient triangle rendering
- **Delta Time Calculation**: Real-time frame timing for smooth animations and performance monitoring
- **Cross-Platform**: Compatible with Windows, macOS, and Linux
- **Modern C++**: Uses contemporary C++ features and smart memory management

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

## Key Features Demonstrated

1. **Window Management**: Resizable window with automatic coordinate system updates
2. **Coordinate Transformation**: Mathematical conversion between OpenGL and SDL coordinate systems
3. **Renderer Information**: Automatic detection and logging of graphics backend
4. **Input Handling**: Keyboard (ESC to quit) and mouse input with coordinate conversion
5. **Performance Monitoring**: Real-time FPS and delta time calculation
6. **Memory Management**: Proper resource cleanup using RAII principles

## Controls

- **ESC**: Exit application
- **Window Resize**: Automatically adjusts coordinate transformation matrix
- **Mouse Click**: Logs coordinates in both window space and relative to center

## Performance

The application includes built-in performance monitoring:
- Real-time FPS counter
- Delta time calculation for frame-independent animations
- Efficient rendering using SDL's geometry API
- Automatic performance logging every second

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
