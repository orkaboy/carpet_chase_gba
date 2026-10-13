#include "ui.h"

#include "gamestate.h"
#include "gfx.h"

#define EMPTY_TID           47
#define FILLED_HEART_TID    48
#define EMPTY_HEART_TID     49

#define SKULL_TID           50
#define PART_BAR_TID        51
#define FULL_BAR_TID        59

#define HP_UI_START_X       1

#define BOSS_HP_UI_START_X  27
#define BOSS_HP_SEGMENTS    8

void draw_player_hp(void) {
    static int prev_hp = 0;
    if(player.hp != prev_hp) {
        prev_hp = player.hp;
        for(int i = 0; i < PLAYER_MAX_HP; ++i) {
            int tile = (prev_hp > i) ? FILLED_HEART_TID : EMPTY_HEART_TID;
            se_mem[TEXT_SBB][HP_UI_START_X+i] = SE_PALBANK(PAL_UI) | tile;
        }
    }
}

void draw_boss_hp(void) {
    static int prev_hp = 0;
    if(boss.hp != prev_hp) {
        prev_hp = boss.hp;
        int segments = prev_hp >> 2;
        se_mem[TEXT_SBB][BOSS_HP_UI_START_X+1] = SE_PALBANK(PAL_UI) | SKULL_TID;
        for(int i = 0; i < BOSS_HP_SEGMENTS; ++i) {
            int current_segment = (1+i) * BOSS_HP_SEGMENTS;
            int tile = (segments < current_segment) ? PART_BAR_TID+(segments % BOSS_HP_SEGMENTS) : FULL_BAR_TID;
            tile = ((segments + BOSS_HP_SEGMENTS) < current_segment) ? EMPTY_TID : tile;
            se_mem[TEXT_SBB][BOSS_HP_UI_START_X-i] = SE_PALBANK(PAL_UI) | tile;
        }
    }
}
