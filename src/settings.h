#ifndef SETTINGS_H
#define SETTINGS_H

#include <stddef.h>

typedef struct {
    size_t max_squares;
    float square_min_size;
    float square_max_size;
    float square_min_speed;
    float square_max_speed;
} Settings;

#endif // SETTINGS_H
