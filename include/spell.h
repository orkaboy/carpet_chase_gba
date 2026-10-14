#ifndef SPELL_H
#define SPELL_H

#include <tonc.h>

#include "gfx.h"
#include "gamestate.h"

#define NUM_SPELLS (NUM_E_SPELLS + NUM_P_SPELLS)

#define PLAYER_SPELL_SPEED float2fx(-4.0)

#define SPELL_COLL_W 4
#define SPELL_COLL_H 8
#define SPELL_SPAWN_OFF 8

typedef struct spell_t {
    FIXED x;
    FIXED y;
    FIXED velx;
    FIXED vely;
    int w;
    int h;
    int alive;
} spell_t;

// Laid out with all the enemy spells, followed by the player spells
extern spell_t spell_pool[NUM_SPELLS];

void init_spell_manager(void);

int spawn_spell(int player_pool, FIXED x, FIXED y, FIXED velx, FIXED vely);

INLINE int spawn_player_spell(void) {
    // Spawn a spell above the player, moving upwards
    return spawn_spell(1, player.x, player.y - SPELL_SPAWN_OFF, 0, PLAYER_SPELL_SPEED);
}

INLINE int spawn_enemy_spell(FIXED x, FIXED y, FIXED velx, FIXED vely) {
    return spawn_spell(0, x, y, velx, vely);
}

void update_spells(void);
void draw_spells(void);
void end_spell(int index);

#endif
