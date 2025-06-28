#pragma once

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "GraphicsProFunctions.h"

typedef struct GPro_Window {
    int w;
    int h;
    int x;
    int y;
    const char* title;
    SDL_Window* SDLWindow;
} GPro_Window;

typedef struct GPro_Renderer {
    GPro_Window* window;
    SDL_Renderer* renderer;
} GPro_Renderer;

extern size_t windowCount;
extern struct GPro_Window** windows;

void AddWindow(GPro_Window* window);

GPro_Window* GPro_CreateWindow(int PositionX, int PositionY, int Height, int Width, const char* title);

static inline void GPro_DestroyWindow(GPro_Window* window)
{
    if(!window) {
        GPro_Log(GPRO_ERROR, "Can't find window to Destroy\n");
        return;
    }
    SDL_DestroyWindow(window->SDLWindow);
    free(window);
    if (windowCount > 0) {
        windowCount--;
    }
}

static inline void GPro_DestroyAllWindows(GPro_Window*** windows_ptr)
{
    if (!windows_ptr || !*windows_ptr) {
        GPro_Log(GPRO_ERROR, "Can't close windows\n");
        return;
    }

    for (size_t i = 0; i < windowCount; i++) {
        GPro_DestroyWindow((*windows_ptr)[i]);
    }

    free(*windows_ptr);
    *windows_ptr = NULL;
    windowCount = 0;
}

static inline GPro_Renderer* GPro_CreateRenderer(GPro_Window* window)
{
    if (!window) {
        GPro_Log(GPRO_ERROR, "Window not found\n");
        return NULL;
    }

    GPro_Renderer* gproRenderer = (GPro_Renderer*) malloc(sizeof(GPro_Renderer));
    if (!gproRenderer) {
        GPro_Log(GPRO_ERROR, "Can't allocate memory for GPro Renderer\n");
        return NULL;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window->SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Can't create SDL Renderer\n");
        free(gproRenderer);
        return NULL;
    }

    gproRenderer->window = window;
    gproRenderer->renderer = renderer;

    return gproRenderer;
}

static inline void GPro_SetRenderDrawColor(GPro_Renderer* renderer, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    if(!renderer) {
        GPro_Log(GPRO_ERROR, "Can't find renderer\n");

        return;
    }

    SDL_SetRenderDrawColor(renderer->renderer, red, green, blue, alpha);
}

static inline void GPro_RenderClear(GPro_Renderer* renderer)
{
    if(!renderer) {
        GPro_Log(GPRO_ERROR, "Can't find renderer\n");

        return;
    }
    
    SDL_RenderClear(renderer->renderer);
}

static inline void GPro_RenderPresent(GPro_Renderer* renderer)
{
    if(!renderer) {
        GPro_Log(GPRO_ERROR, "Can't find renderer\n");

        return;
    }

    SDL_RenderPresent(renderer->renderer);
}