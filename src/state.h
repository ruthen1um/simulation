#ifndef STATE_H
#define STATE_H

#include <SDL3/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
} State;

#endif // STATE_H
