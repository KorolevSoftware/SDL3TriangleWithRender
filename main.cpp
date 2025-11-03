#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Application state structure
struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    glm::mat4 transform;
    int window_width;
    int window_height;
    
    // Timing for delta time
    Uint64 previous_ticks;
    Uint64 current_ticks;
    float delta_time;
    
    // Vertices in OpenGL coordinates
    glm::vec2 opengl_vertices[3] = {
        glm::vec2(0.0f,  0.5f),   // Top
        glm::vec2(0.5f, -0.5f),   // Bottom-right
        glm::vec2(-0.5f, -0.5f)   // Bottom-left
    };
    
    // Vertex colors in SDL_FColor format (0.0f - 1.0f)
    SDL_FColor vertex_colors[3] = {
        { 1.0f, 0.0f, 0.0f, 1.0f },   // Red
        { 0.0f, 1.0f, 0.0f, 1.0f },   // Green  
        { 0.0f, 0.0f, 1.0f, 1.0f }    // Blue
    };
};

// Function to update transformation matrix
void update_transform_matrix(AppState* state) {
    // Create transformation matrix from OpenGL to SDL coordinates
    // OpenGL: [-1,1] x [-1,1] with center at (0,0)
    // SDL: [0,width] x [0,height] with center at (width/2, height/2)
    
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

// Application initialization callback
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // Allocate memory for application state
    AppState* state = new AppState();
    *appstate = state;
    
    // Set initial window dimensions
    state->window_width = 800;
    state->window_height = 600;

    // Initialize timing
    state->previous_ticks = SDL_GetTicks();
    state->current_ticks = state->previous_ticks;
    state->delta_time = 0.0f;

    // Initialize SDL (returns bool in SDL3 - true on success)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create window and renderer in one call
    if (!SDL_CreateWindowAndRenderer("Hello Triangle - SDL3 with OpenGL Coordinates", 
                                     state->window_width, state->window_height, 
                                     SDL_WINDOW_RESIZABLE, 
                                     &state->window, &state->renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window and renderer creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Log only renderer name
    const char* renderer_name = SDL_GetRendererName(state->renderer);
    SDL_Log("Renderer: %s", renderer_name ? renderer_name : "Unknown");

    // Initialize transformation matrix
    update_transform_matrix(state);

    SDL_Log("SDL_AppInit completed successfully");
    SDL_Log("Window size: %dx%d", state->window_width, state->window_height);
    SDL_Log("Press ESC to exit");
    SDL_Log("Center of coordinates is now in the middle of the window");
    
    return SDL_APP_CONTINUE;
}

// Event processing callback
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    AppState* state = static_cast<AppState*>(appstate);

    switch (event->type) {
        case SDL_EVENT_QUIT:
            SDL_Log("Quit event received");
            return SDL_APP_SUCCESS;

        case SDL_EVENT_WINDOW_RESIZED:
            state->window_width = event->window.data1;
            state->window_height = event->window.data2;
            update_transform_matrix(state);
            SDL_Log("Window resized to %dx%d", state->window_width, state->window_height);
            break;

        case SDL_EVENT_KEY_DOWN:
            SDL_Log("Key pressed: %s", SDL_GetKeyName(event->key.key));
            if (event->key.key == SDLK_ESCAPE) {
                SDL_Log("Escape key pressed, exiting normally");
                return SDL_APP_SUCCESS;
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            // Show mouse coordinates relative to window center
            float mouse_x = event->button.x - state->window_width / 2.0f;
            float mouse_y = state->window_height / 2.0f - event->button.y; // Invert Y
            SDL_Log("Mouse button pressed at window: (%.0f, %.0f), relative to center: (%.1f, %.1f)", 
                   event->button.x, event->button.y, mouse_x, mouse_y);
            break;
    }

    return SDL_APP_CONTINUE;
}

// Main iteration (rendering) callback
SDL_AppResult SDL_AppIterate(void* appstate) {
    AppState* state = static_cast<AppState*>(appstate);

    // Calculate delta time
    state->current_ticks = SDL_GetTicks();
    state->delta_time = (state->current_ticks - state->previous_ticks) / 1000.0f; // Convert to seconds
    state->previous_ticks = state->current_ticks;

    // Log FPS every second (optional)
    static Uint64 fps_timer = 0;
    static int frame_count = 0;
    frame_count++;
    
    if (state->current_ticks - fps_timer >= 1000) { // Every second
        float fps = frame_count / ((state->current_ticks - fps_timer) / 1000.0f);
        SDL_Log("FPS: %.1f, Delta time: %.3f ms", fps, state->delta_time * 1000.0f);
        fps_timer = state->current_ticks;
        frame_count = 0;
    }

    // Clear screen
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    // Create vertices for SDL with transformation applied
    SDL_Vertex vertices[3];
    for (int i = 0; i < 3; i++) {
        glm::vec4 sdl_pos = state->transform * glm::vec4(state->opengl_vertices[i], 0.0f, 1.0f);
        vertices[i].position = SDL_FPoint{ sdl_pos.x, sdl_pos.y };
        vertices[i].color = state->vertex_colors[i];
    }

    // Render triangle
    int result = SDL_RenderGeometry(state->renderer, nullptr, vertices, 3, nullptr, 0);
    if (result < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to render geometry: %s", SDL_GetError());
        return SDL_APP_CONTINUE; // Continue despite rendering error
    }

    // Update screen
    SDL_RenderPresent(state->renderer);
    
    return SDL_APP_CONTINUE;
}

// Application deinitialization callback
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AppState* state = static_cast<AppState*>(appstate);
    
    // Convert result to readable string
    const char* result_str = "Unknown";
    switch (result) {
        case SDL_APP_SUCCESS:
            result_str = "SDL_APP_SUCCESS";
            break;
        case SDL_APP_FAILURE:
            result_str = "SDL_APP_FAILURE";
            break;
        // SDL_APP_CONTINUE never reaches here
    }
    
    SDL_Log("SDL_AppQuit called with result: %s", result_str);

    // Clean up resources in correct order
    if (state) {
        if (state->renderer) {
            SDL_DestroyRenderer(state->renderer);
            state->renderer = nullptr;
            SDL_Log("Renderer destroyed");
        }
        if (state->window) {
            SDL_DestroyWindow(state->window);
            state->window = nullptr;
            SDL_Log("Window destroyed");
        }
        delete state;
        SDL_Log("AppState memory freed");
    }

    SDL_Log("Application finished successfully");
}