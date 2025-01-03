#include <SDL3/SDL.h>
#include "constants.h"
#include "state.h"
#include "square.h"

#define TITLE "squares"
#define WIDTH 600
#define HEIGHT 600

int main(void) {
    State gs;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to init SDL\n");
        return 1;
    };

    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &gs.window, &gs.renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create window\n");
        SDL_Quit();
        return 2;
    }

    gs.width = WIDTH;
    gs.height = HEIGHT;

    auto squares = get_random_squares(&gs);

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
                gs.width = ev.window.data1;
                gs.height = ev.window.data2;
                destroy_squares(squares);
                squares = get_random_squares(&gs);
            }
        }

        for (size_t i = 0; i < MAX_SQUARES; ++i) {
            squares[i]->y += squares[i]->speed;

            if (squares[i]->y > (float) gs.height) {
                destroy_square(squares[i]);
                squares[i] = get_random_square(&gs);
            }
        }

        SDL_SetRenderDrawColor(gs.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(gs.renderer);

        render_squares(&gs, (const Square**)squares);

        SDL_RenderPresent(gs.renderer);

        double delta_time = (double) SDL_GetTicks() - start_time;

        // if everything was rendered faster than expected, then wait to make fps stable
        if ((double) delta_time < frame_time) {
            SDL_Delay((unsigned int) (frame_time - delta_time));
        }
    }

    destroy_squares(squares);

    SDL_DestroyRenderer(gs.renderer);
    SDL_DestroyWindow(gs.window);

    SDL_Quit();

    return 0;
}
