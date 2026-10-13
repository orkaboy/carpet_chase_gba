#include "gamelogic.h"

#include <tonc.h>

#include "gamestate.h"

void update_game(void) {
    // ============================
    // Input section
    // ============================
    // libtonc: Update keypresses
    key_poll();
    // Check for reset (Se+St+L+R)
    if(KEY_EQ(key_held, KEY_RESET)) {
        SoftReset();
    }
    // Fire
    if(key_hit(KEY_A)) {
        // TODO: Fire button
        boss.hp--;
    }
    
    // Player movement logic
    // Fetch +1,0,-1 on X and Y D-pad
    int xdir = key_tri_horz();
    int ydir = key_tri_vert();
    FIXED ydiff = 0;
    if(ydir > 0) { // Move down
        ydiff += P_SPEED_DOWN;
    } else if(ydir < 0) { // Move up
        ydiff -= P_SPEED_UP;
    }
    move_wizard(&player, xdir * P_SPEED_SIDEWAYS, ydiff);

    // ============================
    // Logic section
    // ============================

    // Set the new positions
    move_wizard(&boss, 0, 0);
}
