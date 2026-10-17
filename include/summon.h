#ifndef SUMMON_H
#define SUMMON_H

#include <tonc.h>
#include "gfx.h"

typedef struct summon_t {
    FIXED x;
    FIXED y;
    FIXED velx;    
    FIXED vely;
    int dirx;
    int diry;
    FIXED targetx;
    FIXED targety;
    int anim_frame;
    int frame_timer;
    int shoot_cd;
    int duration;
} summon_t;

extern summon_t summon_pool[NUM_E_SUMMONS];

void init_summons(void);

int spawn_summon(FIXED x, FIXED y, FIXED targetx, FIXED targety, int duration);

void update_summons(void);
void draw_summons(void);

#endif
