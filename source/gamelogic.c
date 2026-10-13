#include "gamelogic.h"

#include <tonc.h>

#include "gamestate.h"

void update_game(void) {
    // ============================
    // Input section
    // ============================
    // libtonc: Update keypresses
    key_poll();
    // Fetch +1,0,-1 on X and Y D-pad
    int xdir = key_tri_horz();
    int ydir = key_tri_vert();
    if(key_is_down(KEY_A)) {
        // TODO: Fire button
    }
    // TODO: Scale axis
    move_wizard(&player, xdir * FIX_ONE, ydir * FIX_ONE);

    // ============================
    // Logic section
    // ============================

    // Set the new positions
    move_wizard(&boss, 0, 0);
}
