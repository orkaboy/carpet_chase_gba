#include "wizard.h"
#include <tonc.h>
#include "gfx.h"

#define WIZ_CARPET_OFFSET -5

void init_wizard(wizard_t* wizard, int hwsprite, int hp) {
    wizard->x = 0;
    wizard->y = 0;
    wizard->hp = hp;
    wizard->hwsprite = hwsprite;
}

void move_wizard(wizard_t* wizard, FIXED dx, FIXED dy) {
    // Move wizard
    wizard->x += dx;
    wizard->y += dy;
}

void draw_wizard(wizard_t* wizard) {
    // Move the OAM objects in the shadow OAM
    // Move wizard sprite
    obj_set_pos(&obj_buffer[wizard->hwsprite], fx2int(wizard->x), WIZ_CARPET_OFFSET + fx2int(wizard->y));
    // Move carpet sprite
    obj_set_pos(&obj_buffer[wizard->hwsprite+2], fx2int(wizard->x), fx2int(wizard->y));
}
