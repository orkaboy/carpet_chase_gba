#ifndef SHIELD_H
#define SHIELD_H

#include <tonc.h>

enum ShieldState {
    S_INIT = 0,
    S_COOLDOWN,
    S_SPAWNED,
    S_HELD,
    S_ACTIVE,
};

typedef struct shield_t {
    FIXED x;
    FIXED y;
    int state;
    int cooldown;
    int anim_frame;
    int frame_timer;
} shield_t;

void init_shield(shield_t* s);
void update_shield(shield_t* s);

#endif
