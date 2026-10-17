#include "gamelogic.h"

#include <tonc.h>
#include <maxmod.h>
#include "soundbank.h"

#include "gamestate.h"
#include "collision.h"
#include "spell.h"
#include "ui.h"
#include "boss_ai.h"
#include "summon.h"

static int player_spell_cd = 0;

void _death_effect(wizard_t* wizard) {
    mmEffect(SFX_DEATH);
    // Fall off carpet
    wizard->deadx = wizard->x;
    wizard->deady = wizard->y + WIZ_CARPET_OFFSET;
    int r = qran_range(0, 2*FIX_SCALE) - FIX_SCALE; // -1 to +1 (fixed)
    wizard->deadvelx = r;
    wizard->deadvely = float2fx(-1.5);
    wizard->deadtheta = 0;
    // Update obj_attr to prevent glitches 
    update_wizard_frame(wizard);
}

void _update_death_effect(wizard_t* wizard) {
    wizard->deadvely += float2fx(0.05);
    wizard->deadx += wizard->deadvelx;
    wizard->deady += wizard->deadvely;
    wizard->deadtheta += (wizard->deadvelx < 0) ? DEATH_ROT_SPEED : -DEATH_ROT_SPEED;

    OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE*)obj_buffer;
    obj_aff_rotate(&obj_aff_buffer[wizard->affine], wizard->deadtheta);

    if(wizard->deady > SCREEN_OOB_B) {
        wizard->deadvelx = wizard->deadvely = 0;
        obj_hide(&obj_buffer[wizard->hwsprite]);
        // TODO: Trigger end of game pause
    }
}

void _handle_input(void) {
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
        _update_death_effect(&player);
    }
}

void _handle_collisions(void) {
    // Check collisions
    spell_t* s = spell_pool;
    for(int i = 0; i < NUM_SPELLS; ++i, s++) {
        if(s->alive) {
            update_bb(BB_SPELL, fx2int(s->x) - SPELL_COLL_W/2, fx2int(s->y) - SPELL_COLL_H/2, SPELL_COLL_W, SPELL_COLL_H);
            if(i < NUM_E_SPELLS) { // Enemy attacking player/shield
                if(is_shield_active(&shield) && check_collision(BB_SPELL, BB_SHIELD)) {
                    end_spell(i);
                } else if(is_wizard_alive(&player) && check_collision(BB_SPELL, BB_PLAYER)) {
                    end_spell(i);
                    player.hit_timer = HITFX_TIMER;
                    player.hp--;
                    // Check dead
                    if(is_wizard_alive(&player)) {
                        // Hit SFX player
                        const int lut[] = {SFX_OOF1, SFX_OOF2, SFX_OOF3};
                        int r = qran_range(0, 3);
                        mmEffect(lut[r]);
                    } else {
                        _death_effect(&player);
                        // Game over
                        render_text(10, "You lose!", RGB8(255, 0, 0));
                    }
                }
            } else { // Player attacking boss
                if(is_wizard_alive(&boss) && check_collision(BB_SPELL, BB_BOSS)) {
                    end_spell(i);
                    boss.hit_timer = HITFX_TIMER;
                    boss.hp--;
                    // Check dead
                    if(is_wizard_dead(&boss)) {
                        _death_effect(&boss);
                        // Game won
                        render_text(10, "You win!", RGB8(0, 255, 0));
                    }
                }
            }
        }
    }
}

void update_game(void) {
    // ============================
    // Input section
    // ============================
    _handle_input();

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

    update_summons();

    update_bb(BB_PLAYER, fx2int(player.x)-PLAYER_HITBOX/2, fx2int(player.y)-PLAYER_HITBOX/2, PLAYER_HITBOX, PLAYER_HITBOX);
    update_bb(BB_BOSS, fx2int(boss.x)-BOSS_HITBOX/2, fx2int(boss.y)-BOSS_HITBOX/2, BOSS_HITBOX, BOSS_HITBOX);
    // Note, uses BB_PLAYER for powerup pickup check
    update_shield(&shield);
    // Moves spells, but doesn't check collision
    update_spells();

    _handle_collisions();

    // ============================
    // AI section
    // ============================
    if(is_wizard_alive(&boss)) {
        update_boss_wave(&boss_ai);
        update_boss_ai(&boss_ai);
    } else {
        _update_death_effect(&boss);
        // Ragebait check
        update_bb(
            BB_BOSS,
            fx2int(boss.deadx)-BOSS_HITBOX/2,
            fx2int(boss.deady)-BOSS_HITBOX/2,
            BOSS_HITBOX,
            BOSS_HITBOX
        );
        if(is_wizard_alive(&player) && check_collision(BB_BOSS, BB_PLAYER)) {
            player.hp = 0;
            _death_effect(&player);
            // Game over
            render_text(10, "You wose?", RGB8(255, 0, 0));
        }
    }
}
