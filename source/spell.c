#include "spell.h"

#define PLAYER_SPELL_TID (4*14)
#define BOSS_SPELL_TID (4*15)

// Laid out with all the enemy spells, followed by the player spells
spell_t spell_pool[NUM_SPELLS];

void init_spell_manager(void) {
    spell_t* s = spell_pool;
    for(int i = 0; i < NUM_SPELLS; ++i) {
        s->x = 0;
        s->y = 0;
        s->velx = 0;
        s->vely = 0;
        s->w = 0;
        s->h = 0;
        s->alive = 0;
        s++;
    }
}

int spawn_spell(int player_pool, FIXED x, FIXED y, FIXED velx, FIXED vely) {
    int idx = (player_pool) ? NUM_E_SPELLS : 0;
    const int top = (player_pool) ? NUM_SPELLS : NUM_E_SPELLS;
    spell_t* s = &spell_pool[idx];
    for(; idx < top; ++idx) {
        if(s->alive == 0) {
            // This spell is dead, reuse it
            s->alive = 1;
            s->x = x;
            s->y = y;
            s->velx = velx;
            s->vely = vely;
            s->w = SPELL_COLL_W;
            s->h = SPELL_COLL_H;
            return 1;
        }
        s++;
    }
    return 0;
}

void update_spells(void) {
    spell_t* s = spell_pool;
    for(int i = 0; i < NUM_SPELLS; ++i) {
        if(s->alive) {
            // Move spell
            s->x += s->velx;
            s->y += s->vely;
            // Detect OOB and remove spell
            if(s->x < SCREEN_OOB_L || s->x > SCREEN_OOB_R || s->y < SCREEN_OOB_T || s->y > SCREEN_OOB_B) {
                end_spell(i);
            }
        }
        s++;
    }
}

void draw_spells(void) {
    spell_t* s = spell_pool;
    for(int i = 0; i < NUM_SPELLS; ++i) {
        if(s->alive) {
            int hwsprite = SPRITE_E_SPELL0 + i;
            int tid = (i < NUM_E_SPELLS) ? BOSS_SPELL_TID : PLAYER_SPELL_TID;
            draw_sprite(hwsprite, tid);
            move_sprite(hwsprite, fx2int(s->x), fx2int(s->y));
        }
        s++;
    }
}

void end_spell(int index) {
    spell_pool[index].alive = 0;
    // Hide sprite
    hide_sprite(SPRITE_E_SPELL0 + index);
}
