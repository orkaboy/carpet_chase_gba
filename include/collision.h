#ifndef COLLISION_H
#define COLLISION_H

#include <tonc.h>

// Keep an array of bounding boxes for collision rather than creating them each frame
#define BB_PLAYER 0
#define BB_BOSS 1
#define BB_SPELL 2
#define BB_SHIELD 3

typedef struct bounding_box_t {
    int left;
    int right;
    int top;
    int bottom;
} bounding_box_t;

int check_collision(int a, int b);
void update_bb(int index, int x, int y, int w, int h);

#endif
