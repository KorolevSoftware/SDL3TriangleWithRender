cmake_minimum_required(VERSION 3.16)
project(HelloTriangleSDL3)

set(CMAKE_CXX_STANDARD 17)

# Поиск зависимостей
find_package(SDL3 REQUIRED)
find_package(glm QUIET)

if(NOT glm_FOUND)
    find_path(GLM_INCLUDE_DIR glm/glm.hpp 
        PATHS 
            /usr/include
            /usr/local/include
            ${CMAKE_SOURCE_DIR}/third_party/glm
    )
    if(GLM_INCLUDE_DIR)
        include_directories(${GLM_INCLUDE_DIR})
    else()
        message(FATAL_ERROR "GLM not found. Please install libglm-dev")
    endif()
endif()

# Исполняемый файл с именем проекта
add_executable(${PROJECT_NAME} main.cpp)

# Линковка
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

# Callback mode
target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_USE_CALLBACKS)