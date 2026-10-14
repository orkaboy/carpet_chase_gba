#ifndef WIZARD_H
#define WIZARD_H

#include <tonc.h>

typedef struct wizard_t {
    FIXED x;
    FIXED y;
    int hp;
    int tile_off;
    int hwsprite;
    int frame_time;
    int anim_frame;
} wizard_t;

void init_wizard(wizard_t* wizard, int tile_off, int hwsprite, int hp);
void move_wizard(wizard_t* wizard, FIXED dx, FIXED dy);
void draw_wizard(wizard_t* wizard);
void update_wizard_frame(wizard_t* wizard);
void restrain_wizard(wizard_t* wizard);

INLINE int is_wizard_alive(wizard_t* wizard) {
    return wizard->hp > 0;
}
INLINE int is_wizard_dead(wizard_t* wizard) {
    return wizard->hp == 0;
}

#endif
