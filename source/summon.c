#include "summon.h"

#include "spell.h"
#include "gamestate.h"

#define SUMMON_TID0 (9*4)
#define SUMMON_ANIM_FRAMES 2
#define SUMMON_ANIM_TIMER 10

summon_t summon_pool[NUM_E_SUMMONS];

void init_summons(void) {
    summon_t* s = summon_pool;
    for(int i = 0; i < NUM_E_SUMMONS; ++i, s++) {
        s->duration = 0;
        s->anim_frame = 0;
        s->frame_timer = 0;
    }
}

int spawn_summon(FIXED x, FIXED y, FIXED targetx, FIXED targety, int duration) {
    summon_t* s = summon_pool;
    for(int i = 0; i < NUM_E_SUMMONS; ++i, s++) {
        if(s->duration) continue;
        s->x = x;
        s->y = y;
        s->dirx = (targetx > x) ? 1 : -1;
        s->diry = (targety > y) ? 1 : -1;
        s->velx = 0;
        s->vely = 0;
        s->targetx = targetx;
        s->targety = targety;
        s->shoot_cd = SUMMON_SPELL_COOLDOWN;
        s->duration = duration;
        return 1;
    }
    return 0;
}

void update_summons(void) {
    summon_t* s = summon_pool;
    for(int i = 0; i < NUM_E_SUMMONS; ++i, s++) {
        if(s->duration == 0) continue;
        // Handle summon duration
        s->duration--;
        if(s->duration == 0) {
            obj_hide(&obj_buffer[SPRITE_E_SUMMON0 + i]);
            continue;
        }

        // Handle animation
        s->frame_timer++;
        if(s->frame_timer >= SUMMON_ANIM_TIMER) {
            s->frame_timer = 0;
            s->anim_frame++;
            if(s->anim_frame >= SUMMON_ANIM_FRAMES) {
                s->anim_frame = 0;
            }
        }

        // Move to target location
        s->velx += SUMMON_ACC * s->dirx;
        if(s->dirx > 0 && s->x < s->targetx) { // right
            s->x += s->velx;
        } else if(s->dirx < 0 && s->x > s->targetx) { // left
            s->x += s->velx;
        }
        s->vely += SUMMON_ACC * s->diry;
        if(s->diry > 0 && s->y < s->targety) { // down
            s->y += s->vely;
        } else if(s->diry < 0 && s->y > s->targety) { // up
            s->y += s->vely;
        }

        // Fire spells
        if(s->shoot_cd) {
            s->shoot_cd--;
        } else {
            s->shoot_cd = SUMMON_SPELL_COOLDOWN;
            FIXED rxvel = qran_range(0, 2*FIX_SCALE) - FIX_SCALE; // -1 to +1 (fixed)
            spawn_enemy_spell(s->x, s->y, rxvel, BOSS_SPELL_SPEED);
        }

    }
}

#define SUMMON_TID_FRAME(frame) (SUMMON_TID0 + 4*frame)

void draw_summons(void) {
    summon_t* s = summon_pool;
    for(int i = 0; i < NUM_E_SUMMONS; ++i, s++) {
        if(s->duration == 0) continue;
        int hwsprite = SPRITE_E_SUMMON0 + i;
        draw_sprite(hwsprite, SUMMON_TID_FRAME(s->anim_frame));
        move_sprite(hwsprite, fx2int(s->x), fx2int(s->y));
    }
}
