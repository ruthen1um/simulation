#include <SDL3/SDL.h>
#include "square.h"

Square* create_square(float x, float y, float size, float speed, SDL_Color color) {
    auto square = (Square*)SDL_malloc(sizeof(Square));
    square->x = x;
    square->y = y;
    square->size = size;
    square->speed = speed;
    square->color = color;
    return square;
}

void destroy_square(Square* square) {
    SDL_free(square);
}

void destroy_squares(Square** squares, Uint8 n) {
    for (Uint8 i = 0; i < n; ++i) {
        SDL_free(squares[i]);
    }
    SDL_free(squares);
}

void render_square(SDL_Renderer* renderer, const Square* square) {
    SDL_SetRenderDrawColor(renderer, square->color.r, square->color.g, square->color.b, square->color.a);
    SDL_FRect rect = { square->x, square->y, square->size, square->size };
    SDL_RenderRect(renderer, &rect);
}

void render_squares(SDL_Renderer* renderer, const Square** squares, Uint8 n) {
    for (Uint8 i = 0; i < n; ++i) {
        render_square(renderer, squares[i]);
    }
}
