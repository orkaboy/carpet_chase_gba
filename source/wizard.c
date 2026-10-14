#include "wizard.h"
#include <tonc.h>
#include "gfx.h"

#define WIZ_CARPET_OFFSET -5
#define WIZARD_ANIM_FRAMES 4
#define WIZARD_ANIM_TIME 10


void init_wizard(wizard_t* wizard, int tile_off, int hwsprite, int hp) {
    wizard->x = 0;
    wizard->y = 0;
    wizard->hp = hp;
    wizard->tile_off = tile_off;
    wizard->hwsprite = hwsprite;
    wizard->frame_time = 0;
    wizard->anim_frame = 0;
}

void move_wizard(wizard_t* wizard, FIXED dx, FIXED dy) {
    // Move wizard
    wizard->x += dx;
    wizard->y += dy;
}

void draw_wizard(wizard_t* wizard) {
    // Move the OAM objects in the shadow OAM
    // Move wizard sprite
    move_sprite(wizard->hwsprite, fx2int(wizard->x), WIZ_CARPET_OFFSET + fx2int(wizard->y));
    // Move carpet sprite
    move_sprite(wizard->hwsprite+2, fx2int(wizard->x), fx2int(wizard->y));
}

void update_wizard_frame(wizard_t* wizard) {
    // Tick animation
    wizard->frame_time++;
    if(wizard->frame_time >= WIZARD_ANIM_TIME) {
        wizard->frame_time = 0;
        wizard->anim_frame++;
        if(wizard->anim_frame >= WIZARD_ANIM_FRAMES) {
            wizard->anim_frame = 0;
        }
    }
    // Update shadow OAM
    draw_sprite(
        wizard->hwsprite,
        wizard->tile_off+4*wizard->anim_frame
    );
}
