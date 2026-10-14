#include "boss_ai.h"

#include "gamestate.h"
#include "spell.h"

#define BOSS_BOUNDS_X (SCREEN_WIDTH/2-16)

// For some reason, libtonc's float2fx isn't recognized as const
#define C_FLOAT2FX(f) ((FIXED)(f*FIX_SCALEF))

const ai_instruction_t w_3s = { WAIT, 0, SECONDS(3) };
const ai_instruction_t w_1s = { WAIT, 0, SECONDS(1) };
const ai_instruction_t w_750ms = { WAIT, 0, SECONDS(0.75) };
const ai_instruction_t w_500ms = { WAIT, 0, SECONDS(0.5) };
const ai_instruction_t w_250ms = { WAIT, 0, SECONDS(0.25) };
const ai_instruction_t w_125ms = { WAIT, 0, SECONDS(0.125) };

const ai_instruction_t s_s = {SHOOT, 0, 0};
const ai_instruction_t s_s_l = {SHOOT, C_FLOAT2FX(0.2), 0};
const ai_instruction_t s_s_l2 = {SHOOT, C_FLOAT2FX(0.4), 0};
const ai_instruction_t s_s_l3 = {SHOOT, C_FLOAT2FX(0.6), 0};
const ai_instruction_t s_s_l4 = {SHOOT, C_FLOAT2FX(0.8), 0};
const ai_instruction_t s_s_r = {SHOOT, C_FLOAT2FX(-0.2), 0};
const ai_instruction_t s_s_r2 = {SHOOT, C_FLOAT2FX(-0.4), 0};
const ai_instruction_t s_s_r3 = {SHOOT, C_FLOAT2FX(-0.6), 0};
const ai_instruction_t s_s_r4 = {SHOOT, C_FLOAT2FX(-0.8), 0};

// TODO: Move templates

// TODO: More patterns

const ai_instruction_t* pattern_start[] = {&w_3s, &s_s, NULL};

void init_boss_ai(boss_ai_t* ai) {
    ai->cur_ins = pattern_start;
    ai->progress = 0;
    ai->phase = 0;
    ai->target_x = 0;
    // Wave motion
    ai->wave_vely = 0;
    ai->wave_dir = 0;
}

void _update_phase(boss_ai_t* ai) {
    if(boss.hp > BOSS_PHASE_1) {
        ai->phase = 0;
    } else if(boss.hp > BOSS_PHASE_2) {
        ai->phase = 1;
    } else if(boss.hp > BOSS_PHASE_3) {
        ai->phase = 2;
    } else if(boss.hp > BOSS_PHASE_4) {
        ai->phase = 3;
    }
}

int _handle_instruction(boss_ai_t* ai) {
    const ai_instruction_t* cur_ins = *(ai->cur_ins);

    // TODO: Handle pattern specifics
    switch(cur_ins->type) {
        case WAIT:
            ai->progress++;
            return ai->progress >= cur_ins->duration;
        case MOVE: break;
        case MOVE_ABS: break;
        case SHOOT: {
            return spawn_enemy_spell(
                boss.x,
                boss.y + int2fx(8),
                cur_ins->velx,
                (ai->phase >= 3) ? BOSS_SPELL_SPEED4 : BOSS_SPELL_SPEED
            );
        }
        default: break;
    }
    return 0;
}

void update_boss_ai(boss_ai_t* ai) {
    // Select the current boss phase (controls patterns, speed and general difficulty)
    _update_phase(ai);
    // Handle the current instruction. The return value is true if we should progress to the next instruction
    int should_progress = _handle_instruction(ai);
    // Move to next instruction in pattern
    if(should_progress) {
        ai->progress = 0;
        ai->cur_ins++;
    }

    // Check if current pattern is done
    if(*ai->cur_ins == NULL) {
        // TODO: Select new pattern based on phase
        switch(ai->phase) {
            case 0: break;
            case 1: break;
            case 2: break;
            case 3: break;
            default: break;
        }
        // TODO: Temp
        ai->cur_ins = pattern_start;
    }
}

// Y-axis wave motion
void update_boss_wave(boss_ai_t* ai) {
    if(ai->wave_dir) {
        ai->wave_vely += BOSS_ACC_Y;
        if(ai->wave_vely >= BOSS_PEAK_Y_SPEED) {
            ai->wave_dir = 0;
        }
    } else {
        ai->wave_vely -= BOSS_ACC_Y;
        if(ai->wave_vely <= -BOSS_PEAK_Y_SPEED) {
            ai->wave_dir = 1;
        }
    }
    boss.y += ai->wave_vely;
}

