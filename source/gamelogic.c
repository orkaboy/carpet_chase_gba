#include "gamelogic.h"

#include <tonc.h>

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

    // ============================
    // Logic section
    // ============================

}
