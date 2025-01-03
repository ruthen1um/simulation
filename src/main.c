#include <SDL3/SDL.h>
#include "state.h"
#include "square.h"

#define TITLE "squares"
#define WIDTH 600
#define HEIGHT 600

#define MAX_SQUARES 100
#define SQUARE_MIN_SIZE 15.0f
#define SQUARE_MAX_SIZE 50.0f
#define SQUARE_MIN_SPEED 1.0f
#define SQUARE_MAX_SPEED 3.0f

int main(void) {
    State state;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to init SDL\n");
        return 1;
    };

    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &state.window, &state.renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create window\n");
        SDL_Quit();
        return 2;
    }

    state.width = WIDTH;
    state.height = HEIGHT;

    state.max_squares = MAX_SQUARES,
    state.square_min_size = SQUARE_MIN_SIZE,
    state.square_max_size = SQUARE_MAX_SIZE,
    state.square_min_speed = SQUARE_MIN_SPEED,
    state.square_max_speed = SQUARE_MAX_SPEED;

    auto squares = get_random_squares(&state);

    double fps = 60.0; // frames / second
    double frame_time = (1.0 / fps) * 1000.0; // in seconds multiplied by 1000 - milliseconds

    auto running = true;
    while (running) {
        double start_time = (double) SDL_GetTicks();

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (ev.type == SDL_EVENT_WINDOW_RESIZED) {
                state.width = ev.window.data1;
                state.height = ev.window.data2;
                destroy_squares(&state, squares);
                squares = get_random_squares(&state);
            }
        }

        for (size_t i = 0; i < MAX_SQUARES; ++i) {
            squares[i]->y += squares[i]->speed;

            if (squares[i]->y > (float) state.height) {
                destroy_square(squares[i]);
                squares[i] = get_random_square(&state);
            }
        }

        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(state.renderer);

        render_squares(&state, (const Square**)squares);

        SDL_RenderPresent(state.renderer);

        double delta_time = (double) SDL_GetTicks() - start_time;

        // if everything was rendered faster than expected, then wait to make fps stable
        if (delta_time < frame_time) {
            SDL_Delay((unsigned int) (frame_time - delta_time));
        }
    }

    destroy_squares(&state, squares);

    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);

    SDL_Quit();

    return 0;
}
