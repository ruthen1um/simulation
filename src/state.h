#ifndef STATE_H
#define STATE_H

#include <SDL3/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;

    size_t max_squares;
    float square_min_size;
    float square_max_size;
    float square_min_speed;
    float square_max_speed;
} State;

#endif // STATE_H
