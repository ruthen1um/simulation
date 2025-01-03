#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#define TITLE "squares"
#define WIDTH 600
#define HEIGHT 600

#define MAX_SQUARES 10
#define SQUARE_MIN_SIZE 15.0f
#define SQUARE_MAX_SIZE 50.0f
#define SQUARE_MIN_SPEED 1.0f 
#define SQUARE_MAX_SPEED 3.0f 

#define LIMIT(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

typedef struct {
    float x;
    float y;
    float size;
    float speed;
    SDL_Color color;
} Square;

struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
} app;

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
Square* create_square(float x, float y, float size, float speed, SDL_Color color) {
    auto square = (Square*)SDL_malloc(sizeof(Square));
    square->x = x;
    square->y = y;
    square->size = size;
    square->speed = speed;
    square->color = color;

    return square;
}

Square* get_random_square() {
    auto size = get_random_float(SQUARE_MIN_SIZE, SQUARE_MAX_SIZE);

    return create_square(
        get_random_float(0, (float) app.width - size),
        get_random_float(0, (float) app.height - size),
        size,
        get_random_float(SQUARE_MIN_SPEED, SQUARE_MAX_SPEED),
        get_random_color()
    );
}

Square** get_random_squares() {
    auto squares = (Square**)SDL_malloc((sizeof(Square*)) * MAX_SQUARES);
    for (size_t i = 0; i < MAX_SQUARES; ++i) {
        squares[i] = get_random_square();
    }
    return squares;
}

void render_square(const Square* square) {
    SDL_SetRenderDrawColor(app.renderer, square->color.r, square->color.g, square->color.b, square->color.a);
    SDL_FRect rect = { square->x, square->y, square->size, square->size };
    SDL_RenderRect(app.renderer, &rect);
}

void render_squares(const Square** squares) {
    for (size_t i = 0; i < MAX_SQUARES; ++i) {
        render_square(squares[i]);
    }
}

void destroy_square(Square* square) {
    SDL_free(square);
}

void destroy_squares(Square** squares) {
    for (size_t i = 0; i < MAX_SQUARES; ++i) {
        destroy_square(squares[i]);
    }
}

int main(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to init SDL\n");
        return 1;
    };

    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &app.window, &app.renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create window\n");
        SDL_Quit();
        return 2;
    }

    app.width = WIDTH;
    app.height = HEIGHT;

    auto squares = get_random_squares();

    double fps = 60.0;
    double frame_time = 1000.0 / fps;

    auto running = true;
    while (running) {
        double start_time = (double) SDL_GetTicks();

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (ev.type == SDL_EVENT_WINDOW_RESIZED) {
                app.width = ev.window.data1;
                app.height = ev.window.data2;
                destroy_squares(squares);
                squares = get_random_squares();
            } else if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            }
        }

        for (size_t i = 0; i < MAX_SQUARES; ++i) {
            squares[i]->y += squares[i]->speed;

            if (squares[i]->y > (float) app.height) {
                destroy_square(squares[i]);
                squares[i] = get_random_square();
            }
        }

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(app.renderer);

        render_squares((const Square**)squares);

        SDL_RenderPresent(app.renderer);

        double delta_time = (double) SDL_GetTicks() - start_time;

        // if everything was rendered faster than expected, then wait to make fps stable
        if ((double) delta_time < frame_time) {
            SDL_Delay((unsigned int) (frame_time - delta_time));
        }
    }

    destroy_squares(squares);

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    SDL_Quit();

    return 0;
}
