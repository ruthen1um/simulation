#ifndef CONFIG_H
#define CONFIG_H

#include <SDL3/SDL.h>

typedef struct Config {
    Uint8 squares_count;
    float square_min_size;
    float square_max_size;
    float square_min_speed;
    float square_max_speed;
} Config;

#endif // CONFIG_H
