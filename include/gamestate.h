#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <tonc.h>
#include "wizard.h"
#include "shield.h"
#include "boss_ai.h"

#define SECONDS(x) (60*x)

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define SCREEN_OOB_L    int2fx(-16)
#define SCREEN_OOB_R    int2fx(SCREEN_WIDTH+16)
#define SCREEN_OOB_T    int2fx(-16)
#define SCREEN_OOB_B    int2fx(SCREEN_HEIGHT+16)

#define BORDER_LEFT     int2fx(8)
#define BORDER_RIGHT    int2fx(SCREEN_WIDTH - 8)
#define BORDER_BOTTOM   int2fx(SCREEN_HEIGHT - 16)
#define BORDER_TOP      int2fx(72)

#define PLAYER_MAX_HP 5
#define BOSS_MAX_HP 255

#define PLAYER_HITBOX 8
#define BOSS_HITBOX 12

#define P_SPEED_SIDEWAYS float2fx(1.5)
#define P_SPEED_UP float2fx(1.0)
#define P_SPEED_DOWN float2fx(1.75)

#define PLAYER_SPELL_CD 10
#define HITFX_TIMER 5

// In frames
#define SHIELD_INIT_COOLDOWN SECONDS(10)
#define SHIELD_COOLDOWN SECONDS(5)
#define SHIELD_Y_SPEED float2fx(0.5)

#define SHIELD_DURATION SECONDS(3)
#define SHIELD_HITBOX_W 12
#define SHIELD_HITBOX_H 8

// Boss wave motion
#define BOSS_ACC_Y          float2fx(1.0/32)
#define BOSS_PEAK_Y_SPEED   int2fx(1)

// Boss AI patterns (hp thresholds)
#define BOSS_PHASE_1        235
#define BOSS_PHASE_2        180
#define BOSS_PHASE_3        120
#define BOSS_PHASE_4        60

#define BOSS_SPELL_SPEED int2fx(2)
#define BOSS_SPELL_SPEED4 int2fx(3)

extern wizard_t player;
extern shield_t shield;
extern wizard_t boss;
extern boss_ai_t boss_ai;

void init_game(void);

#endif
