#include "gamelogic.h"

#include <tonc.h>
#include <maxmod.h>
#include "soundbank.h"

#include "gamestate.h"
#include "collision.h"
#include "spell.h"

// Keep an array of bounding boxes for collision rather than creating them each frame
#define BB_PLAYER 0
#define BB_BOSS 1
#define BB_SPELL 2
#define BB_SHIELD 3
#define BB_SHIELD_POWERUP 4
static bounding_box_t _bbs[5];

void _update_bb(int index, int x, int y, int w, int h) {
    bounding_box_t* bb = &_bbs[index];
    bb->right = x + w;
    bb->left = x;
    bb->top = y;
    bb->bottom = y + h;
}

static int player_spell_cd = 0;

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
    if(key_held(KEY_A) && player_spell_cd == 0) {
        spawn_player_spell();
        player_spell_cd = PLAYER_SPELL_CD;
        mmEffect(SFX_LASER);
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
    restrain_wizard(&player);

    // ============================
    // Timers
    // ============================
    player_spell_cd = (player_spell_cd > 0) ? player_spell_cd-1 : 0;

    // ============================
    // Logic section
    // ============================
    update_wizard_frame(&player);
    update_wizard_frame(&boss);

    update_spells();

    // Set the new positions
    move_wizard(&boss, 0, 0);
}
