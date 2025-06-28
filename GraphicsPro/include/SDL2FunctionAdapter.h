#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "GraphicsProFunctions.h"

static inline void GPro_CheckLoop(SDL_Event event, int* running)
{
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = 0;
        }
    }
}

static inline int GPro_Init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
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