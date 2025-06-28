#include "WindowManager.h"

size_t windowCount = 0;
struct GPro_Window** windows = NULL;

void AddWindow(GPro_Window* window)
{
    GPro_Window** tmp = (GPro_Window**) realloc(windows, sizeof(GPro_Window*) * (windowCount + 1));
    if (tmp) {
        windows = tmp;
        windows[windowCount] = window;
        windowCount++;
    } else {
        GPro_Log(GPRO_ERROR, "Can't add window\n");
        return;
    }
}

GPro_Window* GPro_CreateWindow(int PositionX, int PositionY, int Height, int Width, const char* title)
{
    GPro_Window* window = (GPro_Window*) calloc(windowCount, sizeof(GPro_Window));
    if(!window) {
        GPro_Log(GPRO_ERROR, "Can't create window memory area\n");
        return NULL;
    }
    window->w = Width;
    window->h = Height;
    window->x = PositionX;
    window->y = PositionY;
    window->title = title;
    window->SDLWindow = SDL_CreateWindow(window->title, window->x, window->y, window->w, window->h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!window->SDLWindow) {
        GPro_Log(GPRO_ERROR, "Can't create SDLWindow\n");
        return NULL;
    }

    return window;
}