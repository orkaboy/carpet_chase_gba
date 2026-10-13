#ifndef COLLISION_H
#define COLLISION_H

#include <tonc.h>

typedef struct bounding_box_t {
    int left;
    int right;
    int top;
    int bottom;
} bounding_box_t;

int check_collision(bounding_box_t* a, bounding_box_t* b);

#endif
