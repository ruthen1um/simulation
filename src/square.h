#ifndef SQUARE_H
#define SQUARE_H

#include <SDL3/SDL.h>
#include "state.h"

typedef struct {
    float x;
    float y;
    float size;
    float speed;
    SDL_Color color;
} Square;

Square* create_square(float x, float y, float size, float speed, SDL_Color color);
Square* get_random_square(const State* state);
Square** get_random_squares(const State* state);

void destroy_square(Square* square);
void destroy_squares(Square** squares);

void render_square(const State* state, const Square* square);
void render_squares(const State* state, const Square** squares);

#endif // SQUARE_H
