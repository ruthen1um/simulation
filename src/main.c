#include <SDL3/SDL.h>
#include "square.h"
#include "config.h"
#include "misc.h"

#define TITLE "squares"
#define WIDTH 600
#define HEIGHT 600

#define SQUARES_COUNT 100
#define SQUARE_MIN_SIZE 15.0f
#define SQUARE_MAX_SIZE 50.0f
#define SQUARE_MIN_SPEED 1.0f
#define SQUARE_MAX_SPEED 3.0f

#define FPS 60.0

typedef struct State {
    SDL_Renderer* renderer;
    SDL_Window* window;
    bool running;
} State;

typedef struct WindowParameters {
    const char* title;
    int width;
    int height;
} WindowParameters;

Square* get_random_square(const Config* config, int width, int height) {
    auto size = get_random_float(config->square_min_size, config->square_max_size);
    return create_square(
        get_random_float(0, (float)width - size),
        get_random_float(0, (float)height - size),
        size,
        get_random_float(config->square_min_speed, config->square_max_speed),
        get_random_color()
    );
}

Square** get_random_squares(const Config* config, int width, int height) {
    auto squares = (Square**)SDL_malloc(sizeof(Square*) * config->squares_count);
    for (Uint8 i = 0; i < config->squares_count; ++i) {
        squares[i] = get_random_square(config, width, height);
    }
    return squares;
}

int main(void) {
    State g_state;
    g_state.running = true;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to init SDL\n");
        return 1;
    };

    Config g_config;
    g_config.squares_count = SQUARES_COUNT;
    g_config.square_min_size = SQUARE_MIN_SIZE;
    g_config.square_max_size = SQUARE_MAX_SIZE;
    g_config.square_min_speed = SQUARE_MIN_SPEED;
    g_config.square_max_speed = SQUARE_MAX_SPEED;

    WindowParameters g_params;
    g_params.title = TITLE;
    g_params.width = WIDTH;
    g_params.height = HEIGHT;

    if (!SDL_CreateWindowAndRenderer(g_params.title, g_params.width, g_params.height,
                                     SDL_WINDOW_RESIZABLE, &g_state.window, &g_state.renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create window\n");
        SDL_Quit();
        return 2;
    };

    auto squares = get_random_squares(&g_config, g_params.width, g_params.height);

    double fps = FPS; // frames / second
    double frame_time = (1.0 / fps) * 1000.0; // in seconds multiplied by 1000 - milliseconds

    g_state.running = true;
    while (g_state.running) {
        double start_time = (double)SDL_GetTicks();

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) {
                g_state.running = false;
            } else if (ev.type == SDL_EVENT_WINDOW_RESIZED) {
                g_params.width = ev.window.data1;
                g_params.height = ev.window.data2;
                destroy_squares(squares, g_config.squares_count);
                squares = get_random_squares(&g_config, g_params.width, g_params.height);
            }
        }

        for (Uint8 i = 0; i < SQUARES_COUNT; ++i) {
            squares[i]->y += squares[i]->speed;

            if (squares[i]->y > (float)g_params.height) {
                destroy_square(squares[i]);
                squares[i] = get_random_square(&g_config, g_params.width, g_params.height);
            }
        }

        SDL_SetRenderDrawColor(g_state.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(g_state.renderer);

        render_squares(g_state.renderer, (const Square**)squares, g_config.squares_count);

        SDL_RenderPresent(g_state.renderer);

        double delta_time = (double)SDL_GetTicks() - start_time;

        // if everything was rendered faster than expected, then wait to make fps stable
        if (delta_time < frame_time) {
            SDL_Delay((unsigned int)(frame_time - delta_time));
        }
    }

    destroy_squares(squares, g_config.squares_count);

    SDL_DestroyRenderer(g_state.renderer);
    SDL_DestroyWindow(g_state.window);

    SDL_Quit();

    return 0;
}
