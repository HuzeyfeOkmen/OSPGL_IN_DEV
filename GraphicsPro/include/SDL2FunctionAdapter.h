#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "GraphicsProFunctions.h"

static inline void GPro_CheckLoop(GPro_Event* event, int* running)
{
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        event->SDLEvent = sdl_event;
        if (sdl_event.type == SDL_QUIT) {
            *running = 0;
        }
    }
}

static inline int GPro_Init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        GPro_Log(GPRO_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }
    GPro_Log(GPRO_OUTPUT, "Program Starting\n");
    return 0;
}

static inline void GPro_Quit(void)
{
    GPro_Log(GPRO_OUTPUT, "Quitting Program\n");
    SDL_Quit();
}

static inline void GPro_Delay(uint32_t ms)
{
    if (ms > 0) {
        SDL_Delay(ms);
    } else {
        GPro_Log(GPRO_WARNING, "⚠️ GPro_Delay: Gereksiz 0ms gecikme çağrısı yapıldı\n");
    }
}
