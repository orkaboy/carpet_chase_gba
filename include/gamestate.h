#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <tonc.h>
#include "wizard.h"

#define PLAYER_MAX_HP 5
#define BOSS_MAX_HP 255

#define P_SPEED_SIDEWAYS float2fx(1.5)
#define P_SPEED_UP float2fx(1.0)
#define P_SPEED_DOWN float2fx(1.75)

extern wizard_t player;
extern wizard_t boss;

void init_game(void);

#endif
