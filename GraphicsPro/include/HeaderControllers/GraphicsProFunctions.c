#include "GraphicsProFunctions.h"

FILE* GPro_GetStream(GPro_StreamType streamType)
{
    switch (streamType) {
        case GPRO_OUTPUT:  return stdout;
        case GPRO_WARNING: return stdout;
        case GPRO_ERROR:   return stderr;
        default:           return stdout;
    }
}

const char* GPro_GetColor(GPro_StreamType streamType)
{
    switch (streamType) {
        case GPRO_OUTPUT:  return GREEN_TEXT;
        case GPRO_WARNING: return YELLOW_TEXT;
        case GPRO_ERROR:   return RED_TEXT;
        default:           return RESET_TEXT;
    }
}