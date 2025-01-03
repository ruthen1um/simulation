#include <SDL3/SDL.h>
#include "square.h"
#include "state.h"
#include "misc.h"

Square* create_square(float x, float y, float size, float speed, SDL_Color color) {
    auto square = (Square*)SDL_malloc(sizeof(Square));
    square->x = x;
    square->y = y;
    square->size = size;
    square->speed = speed;
    square->color = color;

    return square;
}

Square* get_random_square(const State* state) {
    auto size = get_random_float(
        state->square_min_size,
        state->square_max_size
    );

    return create_square(
        get_random_float(0, (float) state->width - size),
        get_random_float(0, (float) state->height - size),
        size,
        get_random_float(
            state->square_min_speed,
            state->square_max_speed),
        get_random_color()
    );
}

Square** get_random_squares(const State* state) {
    auto squares = (Square**)SDL_malloc((sizeof(Square*)) * state->max_squares);
    for (size_t i = 0; i < state->max_squares; ++i) {
        squares[i] = get_random_square(state);
    }
    return squares;
}

void destroy_square(Square* square) {
    SDL_free(square);
}

void destroy_squares(const State* state, Square** squares) {
    for (size_t i = 0; i < state->max_squares; ++i) {
        destroy_square(squares[i]);
    }
}

void render_square(const State* state, const Square* square) {
    SDL_SetRenderDrawColor(state->renderer, square->color.r, square->color.g, square->color.b, square->color.a);
    SDL_FRect rect = { square->x, square->y, square->size, square->size };
    SDL_RenderRect(state->renderer, &rect);
}

void render_squares(const State* state, const Square** squares) {
    for (size_t i = 0; i < state->max_squares; ++i) {
        render_square(state, squares[i]);
    }
}
