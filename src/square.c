#include <SDL3/SDL.h>
#include "settings.h"
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

Square* get_random_square(const State* state, const Settings* settings) {
    auto size = get_random_float(
        settings->square_min_size,
        settings->square_max_size
    );

    return create_square(
        get_random_float(0, (float) state->width - size),
        get_random_float(0, (float) state->height - size),
        size,
        get_random_float(
            settings->square_min_speed,
            settings->square_max_speed),
        get_random_color()
    );
}

Square** get_random_squares(const State* state, const Settings* settings) {
    auto squares = (Square**)SDL_malloc((sizeof(Square*)) * settings->max_squares);
    for (size_t i = 0; i < settings->max_squares; ++i) {
        squares[i] = get_random_square(state, settings);
    }
    return squares;
}

void destroy_square(Square* square) {
    SDL_free(square);
}

void destroy_squares(const Settings* settings, Square** squares) {
    for (size_t i = 0; i < settings->max_squares; ++i) {
        destroy_square(squares[i]);
    }
}

void render_square(const State* state, const Square* square) {
    SDL_SetRenderDrawColor(state->renderer, square->color.r, square->color.g, square->color.b, square->color.a);
    SDL_FRect rect = { square->x, square->y, square->size, square->size };
    SDL_RenderRect(state->renderer, &rect);
}

void render_squares(const State* state, const Settings* settings, const Square** squares) {
    for (size_t i = 0; i < settings->max_squares; ++i) {
        render_square(state, squares[i]);
    }
}
