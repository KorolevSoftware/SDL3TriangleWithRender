#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Структура для хранения состояния приложения
struct AppState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    glm::mat4 transform;
    int window_width;
    int window_height;
    
    // Вершины в координатах OpenGL
    glm::vec2 opengl_vertices[3] = {
        glm::vec2(0.0f,  0.5f),   // Верх
        glm::vec2(0.5f, -0.5f),   // Право-низ
        glm::vec2(-0.5f, -0.5f)   // Лево-низ
    };
    
    // Цвета вершин в формате SDL_FColor (0.0f - 1.0f)
    SDL_FColor vertex_colors[3] = {
        { 1.0f, 0.0f, 0.0f, 1.0f },   // Красный
        { 0.0f, 1.0f, 0.0f, 1.0f },   // Зеленый  
        { 0.0f, 0.0f, 1.0f, 1.0f }    // Синий
    };
};

// Функция для обновления матрицы преобразования
void update_transform_matrix(AppState* state) {
    // Создаем матрицу преобразования из OpenGL в SDL координаты
    // OpenGL: [-1,1] x [-1,1] с центром в (0,0)
    // SDL: [0,width] x [0,height] с центром в (width/2, height/2)
    
    state->transform = glm::mat4(1.0f);
    
    // 1. Сначала сдвигаем (0,0) в центр окна
    state->transform = glm::translate(state->transform, glm::vec3(
        state->window_width / 2.0f,   // Сдвиг по X на половину ширины
        state->window_height / 2.0f,  // Сдвиг по Y на половину высоты
        0.0f
    ));
    
    // 2. Потом масштабируем к размерам окна
    state->transform = glm::scale(state->transform, glm::vec3(
        state->window_width / 2.0f,   // Масштаб по X
        -state->window_height / 2.0f, // Масштаб по Y (отрицательный для инверсии)
        1.0f
    ));
}

// Callback инициализации приложения
extern "C" SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // Выделяем память для состояния приложения
    AppState* state = new AppState();
    *appstate = state;
    
    // Устанавливаем начальные размеры окна
    state->window_width = 800;
    state->window_height = 600;

    // Инициализация SDL (возвращает bool в SDL3 - true при успехе)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Создание окна
    state->window = SDL_CreateWindow("Hello Triangle - SDL3 with OpenGL Coordinates", 
                                     state->window_width, state->window_height, 
                                     SDL_WINDOW_RESIZABLE);
    if (!state->window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Создание рендерера
    state->renderer = SDL_CreateRenderer(state->window, NULL);
    if (!state->renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(state->window);
        return SDL_APP_FAILURE;
    }

    // Логируем только имя рендерера
    const char* renderer_name = SDL_GetRendererName(state->renderer);
    SDL_Log("Renderer: %s", renderer_name ? renderer_name : "Unknown");

    // Инициализация матрицы преобразования
    update_transform_matrix(state);

    SDL_Log("SDL_AppInit completed successfully");
    SDL_Log("Window size: %dx%d", state->window_width, state->window_height);
    SDL_Log("Press ESC to exit");
    SDL_Log("Center of coordinates is now in the middle of the window");
    
    return SDL_APP_CONTINUE;
}

// Callback обработки событий
extern "C" SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
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
            // Показываем координаты мыши относительно центра окна
            float mouse_x = event->button.x - state->window_width / 2.0f;
            float mouse_y = state->window_height / 2.0f - event->button.y; // Инвертируем Y
            SDL_Log("Mouse button pressed at window: (%.0f, %.0f), relative to center: (%.1f, %.1f)", 
                   event->button.x, event->button.y, mouse_x, mouse_y);
            break;
    }

    return SDL_APP_CONTINUE;
}

// Callback основной итерации (рендеринг)
extern "C" SDL_AppResult SDL_AppIterate(void* appstate) {
    AppState* state = static_cast<AppState*>(appstate);

    // Очистка экрана
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    // Создаем вершины для SDL с применением преобразования
    SDL_Vertex vertices[3];
    for (int i = 0; i < 3; i++) {
        glm::vec4 sdl_pos = state->transform * glm::vec4(state->opengl_vertices[i], 0.0f, 1.0f);
        vertices[i].position = SDL_FPoint{ sdl_pos.x, sdl_pos.y };
        vertices[i].color = state->vertex_colors[i];
    }

    // Отрисовка треугольника
    int result = SDL_RenderGeometry(state->renderer, NULL, vertices, 3, NULL, 0);
    if (result < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to render geometry: %s", SDL_GetError());
        return SDL_APP_CONTINUE; // Продолжаем несмотря на ошибку рендеринга
    }

    // Обновление экрана
    SDL_RenderPresent(state->renderer);

    // Небольшая задержка для снижения нагрузки на CPU (~60 FPS)
    SDL_Delay(16);
    
    return SDL_APP_CONTINUE;
}

// Callback деинициализации приложения
extern "C" void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    AppState* state = static_cast<AppState*>(appstate);
    
    // Преобразуем результат в читаемую строку
    const char* result_str = "Unknown";
    switch (result) {
        case SDL_APP_SUCCESS:
            result_str = "SDL_APP_SUCCESS";
            break;
        case SDL_APP_FAILURE:
            result_str = "SDL_APP_FAILURE";
            break;
        case SDL_APP_CONTINUE:
            result_str = "SDL_APP_CONTINUE";
            break;
    }
    
    SDL_Log("SDL_AppQuit called with result: %s", result_str);

    // Очистка ресурсов в правильном порядке
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

    SDL_Quit();
    SDL_Log("SDL quit completed");
    SDL_Log("Application finished successfully");
}