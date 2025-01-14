#ifndef SQUARE_H
#define SQUARE_H

#include <SDL3/SDL.h>

typedef struct {
    float x;
    float y;
    float size;
    float speed;
    SDL_Color color;
} Square;

Square* create_square(float x, float y, float size, float speed, SDL_Color color);
void destroy_square(Square* square);
void destroy_squares(Square** squares, Uint8 n);
void render_square(SDL_Renderer* renderer, const Square* square);
void render_squares(SDL_Renderer* renderer, const Square** squares, Uint8 n);

#endif // SQUARE_H
