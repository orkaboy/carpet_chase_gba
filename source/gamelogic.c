#include "gamelogic.h"

#include <tonc.h>
#include <maxmod.h>
#include "soundbank.h"

#include "gamestate.h"
#include "collision.h"
#include "spell.h"
#include "ui.h"

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
    // TODO: TEMP
    if(key_held(KEY_B)) {
        //spawn_enemy_spell(boss.x, boss.y + SPELL_SPAWN_OFF, 0, -PLAYER_SPELL_SPEED);
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

    update_ui_glow();

    // ============================
    // Logic section
    // ============================
    update_wizard_frame(&player);
    update_wizard_frame(&boss);

    update_bb(BB_PLAYER, fx2int(player.x)-PLAYER_HITBOX/2, fx2int(player.y)-PLAYER_HITBOX/2, PLAYER_HITBOX, PLAYER_HITBOX);
    update_bb(BB_BOSS, fx2int(boss.x)-BOSS_HITBOX/2, fx2int(boss.y)-BOSS_HITBOX/2, BOSS_HITBOX, BOSS_HITBOX);

    update_shield(&shield);
    update_spells();

    // TODO: Check collisions


    // Set the new positions
    move_wizard(&boss, 0, 0);
}
