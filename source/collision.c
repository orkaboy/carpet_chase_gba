#include "collision.h"

static bounding_box_t bb_pool[4];

int check_collision(int a_idx, int b_idx) {
    bounding_box_t* a = &bb_pool[a_idx];
    bounding_box_t* b = &bb_pool[b_idx];
    // Check if we are outside of the bounding boxes (no overlap possible)
    if(a->right < b->left) {
        return 0;
    }
    if(a->left >= b->right) {
        return 0;
    }
    if(a->top >= b->bottom) {
        return 0;
    }
    if(a->bottom < b->top) {
        return 0;
    }
    // If all these fail, there is an overlap/collision
    return 1;
}

void update_bb(int index, int x, int y, int w, int h) {
    bounding_box_t* bb = &bb_pool[index];
    bb->right = x + w;
    bb->left = x;
    bb->top = y;
    bb->bottom = y + h;
}
