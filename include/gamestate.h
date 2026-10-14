#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <tonc.h>
#include "wizard.h"

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define BORDER_LEFT     int2fx(8)
#define BORDER_RIGHT    int2fx(SCREEN_WIDTH - 8)
#define BORDER_BOTTOM   int2fx(SCREEN_HEIGHT - 16)
#define BORDER_TOP      int2fx(72)

#define PLAYER_MAX_HP 5
#define BOSS_MAX_HP 255

#define P_SPEED_SIDEWAYS float2fx(1.5)
#define P_SPEED_UP float2fx(1.0)
#define P_SPEED_DOWN float2fx(1.75)

extern wizard_t player;
extern wizard_t boss;

void init_game(void);

#endif
