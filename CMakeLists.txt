cmake_minimum_required(VERSION 3.20)
project(GraphicsProTest C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_C_STANDARD 17)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GraphicsPro/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GraphicsPro/include/HeaderControllers)

add_compile_options(-Wall -Werror)

add_executable(GraphicsProTest
    main.c
    GraphicsPro/include/HeaderControllers/GraphicsProFunctions.c
    GraphicsPro/include/HeaderControllers/WindowManager.c
)

target_link_libraries(GraphicsProTest SDL2)