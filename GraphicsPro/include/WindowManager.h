#pragma once

#include <SDL2/SDL.h>
#include "GraphicsProFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>

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

typedef enum GPro_BlendMode {
    GPRO_BLENDMODE_NONE  = SDL_BLENDMODE_NONE,
    GPRO_BLENDMODE_BLEND = SDL_BLENDMODE_BLEND,
    GPRO_BLENDMODE_ADD   = SDL_BLENDMODE_ADD,
    GPRO_BLENDMODE_MOD   = SDL_BLENDMODE_MOD
} GPro_BlendMode;

size_t windowCount = 0;
GPro_Window** windows = nullptr;

static inline void AddWindow(GPro_Window* window) {
    GPro_Window** tmp = realloc(windows, sizeof(GPro_Window*) * (windowCount + 1));
    if (!tmp) {
        GPro_Log(GPRO_ERROR, "Failed to add window\n");
        return;
    }
    windows = tmp;
    windows[windowCount++] = window;
}

static inline void RemoveWindow(GPro_Window* window) {
    if (!windows) return;

    size_t i = 0;
    for (; i < windowCount; ++i) {
        if (windows[i] == window) break;
    }

    if (i == windowCount) return; // Bulunamadı

    for (size_t j = i; j < windowCount - 1; ++j) {
        windows[j] = windows[j + 1];
    }

    windowCount--;

    if (windowCount == 0) {
        free(windows);
        windows = nullptr;
    } else {
        GPro_Window** tmp = realloc(windows, sizeof(GPro_Window*) * windowCount);
        if (tmp) windows = tmp;
    }
}

static inline GPro_Window* GPro_CreateWindow(int x, int y, int w, int h, const char* title) {
    if (!title) return nullptr;

    GPro_Window* window = calloc(1, sizeof(GPro_Window));
    if (!window) {
        GPro_Log(GPRO_ERROR, "Failed to allocate window\n");
        return nullptr;
    }

    window->SDLWindow = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
    window->w = w;
    window->h = h;
    window->x = x;
    window->y = y;
    window->title = title;

    AddWindow(window);
    return window;
}

static inline void GPro_DestroyWindow(GPro_Window* window) {
    if (!window) {
        GPro_Log(GPRO_ERROR, "Window to destroy is NULL\n");
        return;
    }
    SDL_DestroyWindow(window->SDLWindow);
    RemoveWindow(window);
    free(window);
}

static inline void GPro_DestroyAllWindows(void) {
    while (windowCount > 0) {
        GPro_DestroyWindow(windows[windowCount - 1]);
    }
}

static inline GPro_Renderer* GPro_CreateRenderer(GPro_Window* window) {
    if (!window) {
        GPro_Log(GPRO_ERROR, "Window is NULL\n");
        return nullptr;
    }

    GPro_Renderer* renderer = malloc(sizeof(GPro_Renderer));
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Failed to allocate renderer\n");
        return nullptr;
    }

    renderer->renderer = SDL_CreateRenderer(window->SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer->renderer) {
        GPro_Log(GPRO_ERROR, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        free(renderer);
        return nullptr;
    }
    renderer->window = window;
    return renderer;
}

static inline void GPro_DestroyRenderer(GPro_Renderer* renderer) {
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Renderer to destroy is NULL\n");
        return;
    }
    SDL_DestroyRenderer(renderer->renderer);
    free(renderer);
}

static inline void GPro_SetRenderDrawColor(GPro_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Renderer is NULL\n");
        return;
    }
    SDL_SetRenderDrawColor(renderer->renderer, r, g, b, a);
}

static inline void GPro_RenderClear(GPro_Renderer* renderer) {
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Renderer is NULL\n");
        return;
    }
    SDL_RenderClear(renderer->renderer);
}

static inline void GPro_RenderPresent(GPro_Renderer* renderer) {
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Renderer is NULL\n");
        return;
    }
    SDL_RenderPresent(renderer->renderer);
}

static inline void GPro_SetRenderDrawBlendMode(GPro_Renderer* renderer, GPro_BlendMode blendMode) {
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Renderer is NULL\n");
        return;
    }
    SDL_SetRenderDrawBlendMode(renderer->renderer, (SDL_BlendMode)blendMode);
}
