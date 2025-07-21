#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define RED_TEXT    "\x1b[31m"
#define GREEN_TEXT  "\x1b[32m"
#define YELLOW_TEXT "\x1b[33m"
#define RESET_TEXT  "\x1b[0m"

typedef enum {
    GPRO_OUTPUT,
    GPRO_ERROR,
    GPRO_WARNING
} GPro_StreamType;
typedef struct GPro_Event {
    SDL_Event SDLEvent;
} GPro_Event;

static inline FILE* GPro_GetStream(GPro_StreamType streamType)
{
    switch (streamType) {
        case GPRO_OUTPUT:  return stdout;
        case GPRO_WARNING: return stdout;
        case GPRO_ERROR:   return stderr;
        default:           return stdout;
    }
}

static inline const char* GPro_GetColor(GPro_StreamType streamType)
{
    switch (streamType) {
        case GPRO_OUTPUT:  return GREEN_TEXT;
        case GPRO_WARNING: return YELLOW_TEXT;
        case GPRO_ERROR:   return RED_TEXT;
        default:           return RESET_TEXT;
    }
}

static inline void GPro_Log(GPro_StreamType stream, const char* format, ...)
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
