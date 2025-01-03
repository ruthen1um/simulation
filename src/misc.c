#include <SDL3/SDL.h>

float get_random_float(float a, float b) {
    return a + SDL_randf() * b;
}

SDL_Color get_random_color() {
    return (SDL_Color) {
        .r = (unsigned char) SDL_rand(256),
        .g = (unsigned char) SDL_rand(256),
        .b = (unsigned char) SDL_rand(256),
        .a = (unsigned char) SDL_rand(256)
    };
}
