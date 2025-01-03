#ifndef SQUARE_H
#define SQUARE_H

#include <SDL3/SDL.h>
#include "settings.h"
#include "state.h"

typedef struct {
    float x;
    float y;
    float size;
    float speed;
    SDL_Color color;
} Square;

Square* create_square(float x, float y, float size, float speed, SDL_Color color);
Square* get_random_square(const State* state, const Settings* settings);
Square** get_random_squares(const State* state, const Settings* settings);

void destroy_square(Square* square);
void destroy_squares(const Settings* settings, Square** squares);

void render_square(const State* state, const Square* square);
void render_squares(const State* state, const Settings* settings, const Square** squares);

#endif // SQUARE_H
