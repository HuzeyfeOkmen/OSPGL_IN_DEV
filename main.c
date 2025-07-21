#include <stdio.h>
#include <stdlib.h>
#include "GraphicsPro.h"

int main(void)
{
    if (GPro_Init() != 0) {
        GPro_Log(GPRO_ERROR, "GPro başlatılamadı!\n");
        return EXIT_FAILURE;
    }

    GPro_Window* window = GPro_CreateWindow(500, 600, 800, 600, "Test Window");
    if (!window) {
        GPro_Log(GPRO_ERROR, "Pencere oluşturulamadı!\n");
        GPro_Quit();
        return EXIT_FAILURE;
    }

    GPro_Renderer* renderer = GPro_CreateRenderer(window);
    if (!renderer) {
        GPro_Log(GPRO_ERROR, "Renderer oluşturulamadı!\n");
        GPro_DestroyWindow(window);
        GPro_Quit();
        return EXIT_FAILURE;
    }

    int running = 1;
    GPro_Event event;

    while (running) {
        GPro_CheckLoop(&event, &running);

        GPro_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        GPro_RenderClear(renderer);

        GPro_RenderPresent(renderer);

        GPro_Delay(1);
    }

    GPro_DestroyRenderer(renderer);
    GPro_DestroyWindow(window);
    GPro_Quit();

    return EXIT_SUCCESS;
}
