#include "gamelogic.h"

#include <tonc.h>
#include <maxmod.h>
#include "soundbank.h"

#include "gamestate.h"
#include "collision.h"
#include "spell.h"
#include "ui.h"
#include "boss_ai.h"

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

    // Restrain controls to when the player is alive
    if(is_wizard_alive(&player)) {
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
    } else {
        // TODO: Death effect
    }

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
    spell_t* s = spell_pool;
    for(int i = 0; i < NUM_SPELLS; ++i, s++) {
        if(s->alive) {
            update_bb(BB_SPELL, fx2int(s->x) - SPELL_COLL_W/2, fx2int(s->y) - SPELL_COLL_H/2, SPELL_COLL_W, SPELL_COLL_H);
            if(i < NUM_E_SPELLS) { // Enemy attacking player/shield
                if(is_shield_active(&shield) && check_collision(BB_SPELL, BB_SHIELD)) {
                    end_spell(i);
                } else if(check_collision(BB_SPELL, BB_PLAYER)) {
                    end_spell(i);
                    player.hit_timer = HITFX_TIMER;
                    if(is_wizard_alive(&player)) {
                        player.hp--;
                        // Check dead
                        if(is_wizard_alive(&player)) {
                            // Hit SFX player
                            const int lut[] = {SFX_OOF1, SFX_OOF2, SFX_OOF3};
                            int r = qran_range(0, 3);
                            mmEffect(lut[r]);
                        } else {
                            mmEffect(SFX_DEATH);
                            // TODO: Game over
                            // TODO: Fall off carpet
                        }
                    }
                }
            } else { // Player attacking boss
                if(check_collision(BB_SPELL, BB_BOSS)) {
                    end_spell(i);
                    boss.hit_timer = HITFX_TIMER;
                    if(is_wizard_alive(&boss)) {
                        boss.hp--;
                        // Check dead
                        if(is_wizard_dead(&boss)) {
                            mmEffect(SFX_DEATH);
                            // TODO: Game won
                            // TODO: Fall off carpet
                        }
                    }
                }
            }
        }
    }

    // ============================
    // AI section
    // ============================
    if(is_wizard_alive(&boss)) {
        update_boss_wave(&boss_ai);
        update_boss_ai(&boss_ai);
    }

    // Set the new sprite position
    move_wizard(&boss, 0, 0);
}
