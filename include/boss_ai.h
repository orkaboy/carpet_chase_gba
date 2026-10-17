#ifndef BOSS_AI_H
#define BOSS_AI_H

#include <tonc.h>

typedef enum ins_type {
    WAIT = 0,
    MOVE,
    SHOOT,
    SHOOT_OFF,
    SUMMON
} ins_type;

typedef struct ai_instruction_t {
    ins_type type;
    FIXED velx;
    int duration;
} ai_instruction_t;

typedef struct boss_ai_t {
    const ai_instruction_t** cur_ins;
    int progress;
    int phase;
    int move_offset;
    // Wave motion
    FIXED wave_vely;
    int wave_dir;
} boss_ai_t;

void init_boss_ai(boss_ai_t* ai);

void update_boss_ai(boss_ai_t* ai);
void update_boss_wave(boss_ai_t* ai);

#endif
