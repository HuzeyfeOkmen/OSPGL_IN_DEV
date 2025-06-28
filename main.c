#include <stdio.h>
#include <stdlib.h>
#include "GraphicsPro/include/GraphicsPro.h"

int main(void)
{
    GPro_Init();
    GPro_Window* window = GPro_CreateWindow(400, 500, 500, 500, "Test Window");
    GPro_Renderer* renderer = GPro_CreateRenderer(window);

    SDL_Event event = {0};
    int running = 1;
    while (running) {
        GPro_CheckLoop(event, &running);
        GPro_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        GPro_RenderClear(renderer);
        GPro_RenderPresent(renderer);

        SDL_Delay(16);
    }

    GPro_DestroyWindow(window);

    GPro_Quit();

    return EXIT_SUCCESS;
}