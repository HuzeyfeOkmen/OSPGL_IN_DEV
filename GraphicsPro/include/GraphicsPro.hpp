#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstdbool>
#include <cstdarg>
#include <SDL2/SDL.h>

#define RED_TEXT    "\x1b[31m"
#define GREEN_TEXT  "\x1b[32m"
#define YELLOW_TEXT "\x1b[33m"
#define RESET_TEXT  "\x1b[0m"

#define GPRO_MODE_BENCHMARK
// #define GPRO_MODE_PRODUCTION
// #define GPRO_MODE_FAKE

#if defined(GPRO_MODE_BENCHMARK)
    #define GPRO_WINDOW_FLAGS SDL_WINDOW_HIDDEN
#elif defined(GPRO_MODE_PRODUCTION)
    #define GPRO_WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#elif defined(GPRO_MODE_FAKE)
    #define GPRO_WINDOW_FLAGS 0
#else
    #error "You Should tell GPro to build mode!"
#endif

typedef struct GPro_Window {
    int w, h, x, y;
    const char* title;
    SDL_Window* SDLWindow;
} GPro_Window;

typedef struct GPro_Renderer {
    GPro_Window* window;
    SDL_Renderer* renderer;
} GPro_Renderer;

typedef enum {
    GPRO_OUTPUT,
    GPRO_ERROR,
    GPRO_WARNING
} GPro_StreamType;

typedef struct GPro_Event {
    SDL_Event SDLEvent;
} GPro_Event;

typedef enum {
    GPRO_BLENDMODE_NONE  = SDL_BLENDMODE_NONE,
    GPRO_BLENDMODE_BLEND = SDL_BLENDMODE_BLEND,
    GPRO_BLENDMODE_ADD   = SDL_BLENDMODE_ADD,
    GPRO_BLENDMODE_MOD   = SDL_BLENDMODE_MOD
} GPro_BlendMode;

class GProAPI {
public:
    GProAPI() = default;
    ~GProAPI() = default;

    GPro_Window* CreateWindow(int x, int y, int width, int height, const char* title) {
        if (!title) return nullptr;
        return GPro_CreateWindow(x, y, width, height, title);
    }

    void DestroyWindow(GPro_Window* window) {
        GPro_DestroyWindow(window);
    }

    void DestroyAllWindows() {
        GPro_DestroyAllWindows();
    }

    GPro_Renderer* CreateRenderer(GPro_Window* window) {
        return GPro_CreateRenderer(window);
    }

    void DestroyRenderer(GPro_Renderer* renderer) {
        GPro_DestroyRenderer(renderer);
    }

    void SetRenderDrawColor(GPro_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        GPro_SetRenderDrawColor(renderer, r, g, b, a);
    }

    void RenderClear(GPro_Renderer* renderer) {
        GPro_RenderClear(renderer);
    }

    void RenderPresent(GPro_Renderer* renderer) {
        GPro_RenderPresent(renderer);
    }

    void SetRenderDrawBlendMode(GPro_Renderer* renderer, GPro_BlendMode mode) {
        GPro_SetRenderDrawBlendMode(renderer, mode);
    }

    int Init(void) {
        return GPro_Init();
    }

    void Quit(void) {
        GPro_Quit();
    }

    void Log(GPro_StreamType stream, const char* format, ...) {
        va_list args;
        va_start(args, format);
        GPro_Log(stream, format, args);
        va_end(args);
    }

    void CheckLoop(GPro_Event* event, int* running) {
        while (SDL_PollEvent(&event->SDLEvent)) {
            if (event->SDLEvent.type == SDL_QUIT)
                *running = 0;
        }
    }

    void Delay(uint32_t ms)
    {
        GPro_Delay(ms);
    }

private:
    size_t windowCount = 0;
    GPro_Window** windows = nullptr;

    FILE* GPro_GetStream(GPro_StreamType type) {
        switch (type) {
            case GPRO_OUTPUT:  return stdout;
            case GPRO_WARNING: return stdout;
            case GPRO_ERROR:   return stderr;
            default:           return stdout;
        }
    }

    const char* GPro_GetColor(GPro_StreamType type) {
        switch (type) {
            case GPRO_OUTPUT:  return GREEN_TEXT;
            case GPRO_WARNING: return YELLOW_TEXT;
            case GPRO_ERROR:   return RED_TEXT;
            default:           return RESET_TEXT;
        }
    }

    void GPro_Log(GPro_StreamType stream, const char* format, ...)
{
    FILE* output = GPro_GetStream(stream);
    const char* color = GPro_GetColor(stream);

    va_list args;
    va_start(args, format);
    
    fprintf(output, "%s", color);
    vfprintf(output, format, args);
    fprintf(output, "%s", RESET_TEXT);

    va_end(args);
}

    int GPro_Init(void) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
            return -1;
        }
        GPro_Log(GPRO_OUTPUT, "Program Starting\n");
        return 0;
    }

    void GPro_Quit(void) {
        GPro_Log(GPRO_OUTPUT, "Quitting Program\n");
        SDL_Quit();
    }

    void AddWindow(GPro_Window* window) {
        GPro_Window** tmp = (GPro_Window**) realloc(windows, sizeof(GPro_Window*) * (windowCount + 1));
        if (tmp) {
            windows = tmp;
            windows[windowCount++] = window;
        } else {
            GPro_Log(GPRO_ERROR, "Failed to realloc window list\n");
        }
    }

    void GPro_DestroyAllWindows(void) {
        for (size_t i = 0; i < windowCount; ++i)
            GPro_DestroyWindow(windows[i]);

        free(windows);
        windows = nullptr;
        windowCount = 0;
    }

    GPro_Window* GPro_CreateWindow(int x, int y, int w, int h, const char* title) {
        GPro_Window* win = (GPro_Window*) calloc(1, sizeof(GPro_Window));
        if (!win) {
            GPro_Log(GPRO_ERROR, "Window allocation failed\n");
            return nullptr;
        }

        win->x = x;
        win->y = y;
        win->w = w;
        win->h = h;
        win->title = title;
        win->SDLWindow = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!win->SDLWindow) {
            GPro_Log(GPRO_ERROR, "SDL window creation failed\n");
            free(win);
            return nullptr;
        }

        AddWindow(win);
        return win;
    }

    void GPro_DestroyWindow(GPro_Window* window) {
        if (!window) return;
        SDL_DestroyWindow(window->SDLWindow);
        free(window);
    }

    GPro_Renderer* GPro_CreateRenderer(GPro_Window* window) {
        if (!window) return nullptr;

        GPro_Renderer* r = (GPro_Renderer*) malloc(sizeof(GPro_Renderer));
        if (!r) return nullptr;

        r->window = window;
        r->renderer = SDL_CreateRenderer(window->SDLWindow, -1, SDL_RENDERER_ACCELERATED);
        if (!r->renderer) {
            free(r);
            return nullptr;
        }

        return r;
    }

    void GPro_DestroyRenderer(GPro_Renderer* renderer) {
        if (!renderer) return;
        SDL_DestroyRenderer(renderer->renderer);
        free(renderer);
    }

    void GPro_SetRenderDrawColor(GPro_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        if (!renderer) return;
        SDL_SetRenderDrawColor(renderer->renderer, r, g, b, a);
    }

    void GPro_RenderClear(GPro_Renderer* renderer) {
        if (!renderer) return;
        SDL_RenderClear(renderer->renderer);
    }

    void GPro_RenderPresent(GPro_Renderer* renderer) {
        if (!renderer) return;
        SDL_RenderPresent(renderer->renderer);
    }

    void GPro_SetRenderDrawBlendMode(GPro_Renderer* renderer, GPro_BlendMode mode) {
        if (!renderer) return;
        SDL_SetRenderDrawBlendMode(renderer->renderer, (SDL_BlendMode)mode);
    }

    void GPro_Delay(uint32_t ms)
{
    if (ms > 0) {
        SDL_Delay(ms);
    } else {
        GPro_Log(GPRO_WARNING, "⚠️ GPro_Delay: Gereksiz 0ms gecikme çağrısı yapıldı\n");
    }
}
};

inline GProAPI GPro;
