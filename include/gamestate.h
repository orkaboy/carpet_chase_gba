#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <tonc.h>
#include "wizard.h"

#define PLAYER_MAX_HP 5

extern wizard_t player;
extern wizard_t boss;

void init_game(void);

#endif
