#include "wizard.h"
#include <tonc.h>
#include "gfx.h"
#include "gamestate.h"

#define WIZARD_ANIM_FRAMES 4
#define WIZARD_ANIM_TIME 10


void init_wizard(wizard_t* wizard, int tile_off, int hwsprite, int affine, int hp) {
    wizard->x = 0;
    wizard->y = 0;
    wizard->hp = hp;
    wizard->tile_off = tile_off;
    wizard->hwsprite = hwsprite;
    wizard->affine = affine;
    wizard->frame_time = 0;
    wizard->anim_frame = 0;
    wizard->hit_timer = 0;
}

void move_wizard(wizard_t* wizard, FIXED dx, FIXED dy) {
    // Move wizard
    wizard->x += dx;
    wizard->y += dy;
}

void restrain_wizard(wizard_t* wizard) {
    if(wizard->x < BORDER_LEFT) wizard->x = BORDER_LEFT;
    else if(wizard->x > BORDER_RIGHT) wizard->x = BORDER_RIGHT;
    if(wizard->y < BORDER_TOP) wizard->y = BORDER_TOP;
    else if(wizard->y > BORDER_BOTTOM) wizard->y = BORDER_BOTTOM;
}

void draw_wizard(wizard_t* wizard) {
    // Move the OAM objects in the shadow OAM
    // Move wizard sprite
    if(is_wizard_alive(wizard)) {
        move_sprite(wizard->hwsprite, fx2int(wizard->x), WIZ_CARPET_OFFSET + fx2int(wizard->y));
    } else {
        obj_set_pos(&obj_buffer[wizard->hwsprite], fx2int(wizard->deadx) - SPRITE_SIZE, fx2int(wizard->deady) - SPRITE_SIZE);
    }
    // Move carpet sprite
    move_sprite(wizard->hwsprite+2, fx2int(wizard->x), fx2int(wizard->y));
}

void update_wizard_frame(wizard_t* wizard) {
    if(wizard->hit_timer) {
        wizard->hit_timer--;
    }
    int pal = (wizard->hit_timer) ? PAL_SPRFX : PAL_SPR;
    // Tick animation (faster if dead)
    wizard->frame_time += is_wizard_alive(wizard) ? 1 : 2;
    if(wizard->frame_time >= WIZARD_ANIM_TIME) {
        wizard->frame_time = 0;
        wizard->anim_frame++;
        if(wizard->anim_frame >= WIZARD_ANIM_FRAMES) {
            wizard->anim_frame = 0;
        }
    }
    // Update shadow OAM
    if(is_wizard_alive(wizard)) {
        draw_sprite(
            wizard->hwsprite,
            (wizard->tile_off+4*wizard->anim_frame) | ATTR2_PALBANK(pal)
        );
    } else {
        obj_set_attr(
            &obj_buffer[wizard->hwsprite],
            ATTR0_AFF_DBL | ATTR0_SQUARE,
            ATTR1_AFF_ID(wizard->affine) | ATTR1_SIZE_16,
            (wizard->tile_off+4*wizard->anim_frame) | ATTR2_PALBANK(pal)
        );
    }
}
