#include "boss_ai.h"

#include "gamestate.h"
#include "spell.h"

#define BOSS_BOUNDS_L (16)
#define BOSS_BOUNDS_R (SCREEN_WIDTH-16)

// libtonc's float2fx isn't recognized as const
#define C_FLOAT2FX(f) ((FIXED)(f*FIX_SCALEF))
#define abs(a) (max(a, -a))

// Wait some time
const ai_instruction_t w_3s = { WAIT, 0, SECONDS(3) };
const ai_instruction_t w_1s = { WAIT, 0, SECONDS(1) };
const ai_instruction_t w_750ms = { WAIT, 0, SECONDS(0.75) };
const ai_instruction_t w_500ms = { WAIT, 0, SECONDS(0.5) };
const ai_instruction_t w_250ms = { WAIT, 0, SECONDS(0.25) };
const ai_instruction_t w_125ms = { WAIT, 0, SECONDS(0.125) };
// Shooting ahead or at an angle
const ai_instruction_t s_s = {SHOOT, 0, 0};
const ai_instruction_t s_s_l1 = {SHOOT, C_FLOAT2FX( 0.2), 0};
const ai_instruction_t s_s_l2 = {SHOOT, C_FLOAT2FX( 0.4), 0};
const ai_instruction_t s_s_l3 = {SHOOT, C_FLOAT2FX( 0.6), 0};
const ai_instruction_t s_s_l4 = {SHOOT, C_FLOAT2FX( 0.8), 0};
const ai_instruction_t s_s_r1 = {SHOOT, C_FLOAT2FX(-0.2), 0};
const ai_instruction_t s_s_r2 = {SHOOT, C_FLOAT2FX(-0.4), 0};
const ai_instruction_t s_s_r3 = {SHOOT, C_FLOAT2FX(-0.6), 0};
const ai_instruction_t s_s_r4 = {SHOOT, C_FLOAT2FX(-0.8), 0};
// Shooting with an offset to the boss
const ai_instruction_t s_o_l1 = {SHOOT_OFF, C_FLOAT2FX(- 8.0), 0};
const ai_instruction_t s_o_l2 = {SHOOT_OFF, C_FLOAT2FX(-16.0), 0};
const ai_instruction_t s_o_l3 = {SHOOT_OFF, C_FLOAT2FX(-24.0), 0};
const ai_instruction_t s_o_l4 = {SHOOT_OFF, C_FLOAT2FX(-32.0), 0};
const ai_instruction_t s_o_r1 = {SHOOT_OFF, C_FLOAT2FX(  8.0), 0};
const ai_instruction_t s_o_r2 = {SHOOT_OFF, C_FLOAT2FX( 16.0), 0};
const ai_instruction_t s_o_r3 = {SHOOT_OFF, C_FLOAT2FX( 24.0), 0};
const ai_instruction_t s_o_r4 = {SHOOT_OFF, C_FLOAT2FX( 32.0), 0};

#define MOVE_OFFSET_DURATION(ins) (4 * ins->duration)
#define GET_MOVE_OFFSET(ins) (ins->duration * fx2int(ins->velx))

const ai_instruction_t mv_s_l1 = {MOVE, C_FLOAT2FX(-2.0), 1};
const ai_instruction_t mv_s_l2 = {MOVE, C_FLOAT2FX(-2.0), 2};
const ai_instruction_t mv_s_l3 = {MOVE, C_FLOAT2FX(-2.0), 3};
const ai_instruction_t mv_s_l4 = {MOVE, C_FLOAT2FX(-2.0), 4};
const ai_instruction_t mv_s_r1 = {MOVE, C_FLOAT2FX( 2.0), 1};
const ai_instruction_t mv_s_r2 = {MOVE, C_FLOAT2FX( 2.0), 2};
const ai_instruction_t mv_s_r3 = {MOVE, C_FLOAT2FX( 2.0), 3};
const ai_instruction_t mv_s_r4 = {MOVE, C_FLOAT2FX( 2.0), 4};

const ai_instruction_t mv_s_l1s = {MOVE, C_FLOAT2FX(-1.0), 2};
const ai_instruction_t mv_s_l2s = {MOVE, C_FLOAT2FX(-1.0), 4};
const ai_instruction_t mv_s_l3s = {MOVE, C_FLOAT2FX(-1.0), 6};
const ai_instruction_t mv_s_l4s = {MOVE, C_FLOAT2FX(-1.0), 8};
const ai_instruction_t mv_s_r1s = {MOVE, C_FLOAT2FX( 1.0), 2};
const ai_instruction_t mv_s_r2s = {MOVE, C_FLOAT2FX( 1.0), 4};
const ai_instruction_t mv_s_r3s = {MOVE, C_FLOAT2FX( 1.0), 6};
const ai_instruction_t mv_s_r4s = {MOVE, C_FLOAT2FX( 1.0), 8};

const ai_instruction_t mv_s_l1f = {MOVE, C_FLOAT2FX(-4.0), 1};
const ai_instruction_t mv_s_l2f = {MOVE, C_FLOAT2FX(-4.0), 2};
const ai_instruction_t mv_s_l3f = {MOVE, C_FLOAT2FX(-4.0), 3};
const ai_instruction_t mv_s_l4f = {MOVE, C_FLOAT2FX(-4.0), 4};
const ai_instruction_t mv_s_r1f = {MOVE, C_FLOAT2FX( 4.0), 1};
const ai_instruction_t mv_s_r2f = {MOVE, C_FLOAT2FX( 4.0), 2};
const ai_instruction_t mv_s_r3f = {MOVE, C_FLOAT2FX( 4.0), 3};
const ai_instruction_t mv_s_r4f = {MOVE, C_FLOAT2FX( 4.0), 4};

// TODO: Move templates

// TODO: More patterns
const ai_instruction_t* pattern_start[] = {&w_3s, NULL};
// Correcting position
const ai_instruction_t* pattern_move_l[] = {&mv_s_l1, NULL};
const ai_instruction_t* pattern_move_lf[] = {&mv_s_l1f, NULL};
const ai_instruction_t* pattern_move_r[] = {&mv_s_r1, NULL};
const ai_instruction_t* pattern_move_rf[] = {&mv_s_r1f, NULL};
// Phase 0
const ai_instruction_t* pattern_phase0_ss[] = {&s_s, &w_1s, NULL};
const ai_instruction_t* pattern_phase0_ls[] = {&mv_s_l3s, &s_s, &w_1s, NULL};
const ai_instruction_t* pattern_phase0_rs[] = {&mv_s_r3s, &s_s, &w_1s, NULL};
// Phase 1+
const ai_instruction_t* pattern_sweep_l[] = {&s_s, &mv_s_l2, &s_s, &mv_s_l2, &s_s, &mv_s_l2, &s_s, &mv_s_l2, &w_750ms, NULL};
const ai_instruction_t* pattern_sweep_r[] = {&s_s, &mv_s_r2, &s_s, &mv_s_r2, &s_s, &mv_s_r2, &s_s, &mv_s_r2, &w_750ms, NULL};
const ai_instruction_t* pattern_spray[] = {&s_s, &mv_s_l2, &s_s, &mv_s_r2, &s_s, &mv_s_l2, &s_s, &mv_s_r2, &s_s, &w_750ms, NULL};
const ai_instruction_t* pattern_fan[] = {&s_s, &s_s_l3, &s_s_r3, &w_500ms, &s_s_l2, &s_s_r2, &w_750ms, NULL};
// Phase 2+
const ai_instruction_t* pattern_sweep_l2[] = {&s_s_l2, &mv_s_l1, &s_s_l2, &mv_s_l1, &s_s_l2, &mv_s_l1, &s_s_l2, &mv_s_l1, &w_500ms, NULL};
const ai_instruction_t* pattern_sweep_r2[] = {&s_s_r2, &mv_s_r1, &s_s_r2, &mv_s_r1, &s_s_r2, &mv_s_r1, &s_s_r2, &mv_s_r1, &w_500ms, NULL};
const ai_instruction_t* pattern_broadside[] = {&s_s, &s_o_l2, &s_o_r2, &s_o_l4, &s_o_r4, &w_1s, NULL};
const ai_instruction_t* pattern_spray2[] = {&s_s_l2, &s_s_r2, &mv_s_l2, &s_s_l2, &s_s_r2, &mv_s_r2, &s_s_l2, &s_s_r2, &mv_s_l2, &s_s_l2, &s_s_r2, &mv_s_r2, &s_s_l2, &s_s_r2, &w_750ms, NULL};
const ai_instruction_t* pattern_weave[] = {&mv_s_l1f, &s_s_l2, &mv_s_r1f, &mv_s_r1f, &s_s_r2, &mv_s_l1f, NULL};
// Phase 3+
const ai_instruction_t* pattern_broadside2[] = {&s_s, &w_125ms, &s_o_l1, &s_o_r1, &w_125ms, &s_o_l2, &s_o_r2, &w_125ms, &s_o_l3, &s_o_r3, &w_125ms, &s_o_l4, &s_o_r4, &w_1s, NULL};
const ai_instruction_t* pattern_tricky_r[] = {&mv_s_r1, &s_s_r1, &mv_s_r1, &s_s_r1, &mv_s_r1, &s_s_r1, &mv_s_r1, &s_s_r1, &s_o_l4, &s_o_r4, &mv_s_l4f, &w_1s, NULL};
const ai_instruction_t* pattern_tricky_l[] = {&mv_s_l1, &s_s_l1, &mv_s_l1, &s_s_l1, &mv_s_l1, &s_s_l1, &mv_s_l1, &s_s_l1, &s_o_r4, &s_o_l4, &mv_s_r4f, &w_1s, NULL};
// Phase 4
const ai_instruction_t* pattern_sweep_l3[] = {&s_s_l1, &s_s_l2, &mv_s_l1, &s_s_l1, &s_s_l2, &mv_s_l1, &s_s_l1, &s_s_l2, &mv_s_l1, &s_s_l1, &s_s_l2, &mv_s_l1, &w_500ms, NULL};
const ai_instruction_t* pattern_sweep_r3[] = {&s_s_r1, &s_s_r2, &mv_s_r1, &s_s_r1, &s_s_r2, &mv_s_r1, &s_s_r1, &s_s_r2, &mv_s_r1, &s_s_r1, &s_s_r2, &mv_s_r1, &w_500ms, NULL};
const ai_instruction_t* pattern_spray3[] = {&s_s, &s_s_r1, &w_125ms, &s_s, &s_s_l1, &w_125ms, &s_s, &s_s_r1, &w_125ms, &s_s, &s_s_l1, &w_125ms, &s_s, &s_s_r1, &w_125ms, &s_s, &s_s_l1, &w_125ms, NULL};
const ai_instruction_t* pattern_weave2[] = {&mv_s_l4f, &s_s_r1, &mv_s_r4f, &s_s, &s_s_l1, &mv_s_r4f, &s_s, &s_s_r1, &mv_s_l4f, &s_s, &s_s_l1, &mv_s_l4f, &s_s, &s_s_r1, &mv_s_r4f, &s_s, &s_s_l1, &w_500ms, NULL};

void init_boss_ai(boss_ai_t* ai) {
    ai->cur_ins = pattern_start;
    ai->progress = 0;
    ai->phase = 0;
    ai->move_offset = 0;
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
    } else {
        ai->phase = 4;
    }
}

int _handle_instruction(boss_ai_t* ai) {
    const ai_instruction_t* cur_ins = *(ai->cur_ins);

    // TODO: Handle pattern specifics
    switch(cur_ins->type) {
        case WAIT:
            ai->progress++;
            return ai->progress >= cur_ins->duration;
        case MOVE:
            ai->progress++;
            boss.x += cur_ins->velx;
            if(ai->progress >= MOVE_OFFSET_DURATION(cur_ins)) {
                ai->move_offset += GET_MOVE_OFFSET(cur_ins);
                return 1;
            }
            break;
        case SHOOT: {
            return spawn_enemy_spell(
                boss.x,
                boss.y + int2fx(8),
                cur_ins->velx,
                (ai->phase >= 3) ? BOSS_SPELL_SPEED4 : BOSS_SPELL_SPEED
            );
        }
        case SHOOT_OFF: {
            return spawn_enemy_spell(
                boss.x + cur_ins->velx,
                boss.y + int2fx(8),
                0,
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
        FIXED xdiff = player.x - boss.x;
        FIXED xdist = abs(xdiff);
        // TODO: Select new pattern based on phase
        switch(ai->phase) {
            // PHASE 0, simple pattern
            case 0: {
                // Player is ahead
                if(xdist < int2fx(8)) { ai->cur_ins = pattern_phase0_ss; }
                // Left (otherwise right)
                else if(xdiff < 0) { ai->cur_ins = pattern_phase0_ls; }
                else { ai->cur_ins = pattern_phase0_rs; }
                return;
            }
            // PHASE 1, moves around more, shoots a lot
            case 1: {
                // Boss is in the corners
                if(boss.x < int2fx(-BOSS_BOUNDS_L)) {
                    ai->cur_ins = pattern_sweep_r;
                } else if(boss.x > int2fx(BOSS_BOUNDS_R)) {
                    ai->cur_ins = pattern_sweep_l;
                }
                // Player is ahead
                else if(xdist < int2fx(16)) {
                    int r = qran_range(0, 2);
                    const ai_instruction_t** lut[] = {
                        pattern_fan,
                        pattern_spray
                    };
                    ai->cur_ins = lut[r];
                }
                // Move towards player
                else if(xdiff < 0) { ai->cur_ins = pattern_move_l; }
                else { ai->cur_ins = pattern_move_r; }
                return;
            }
            case 2: {
                // Boss is in the corners
                if(boss.x < int2fx(-BOSS_BOUNDS_L)) {
                    ai->cur_ins = pattern_sweep_r2;
                } else if(boss.x > int2fx(BOSS_BOUNDS_R)) {
                    ai->cur_ins = pattern_sweep_l2;
                }
                // Player is ahead
                else if(xdist < int2fx(16)) {
                    int r = qran_range(0, 4);
                    const ai_instruction_t** lut[] = {
                        pattern_fan,
                        pattern_spray2,
                        pattern_broadside,
                        pattern_weave
                    };
                    ai->cur_ins = lut[r];
                }
                // Move towards player
                else if(xdiff < 0) { ai->cur_ins = pattern_move_l; }
                else { ai->cur_ins = pattern_move_r; }
                return;
            }
            case 3: {
                // Boss is in the corners
                if(boss.x < int2fx(-BOSS_BOUNDS_L)) {
                    ai->cur_ins = pattern_sweep_r2;
                } else if(boss.x > int2fx(BOSS_BOUNDS_R)) {
                    ai->cur_ins = pattern_sweep_l2;
                }
                // Player is ahead
                else if(xdist < int2fx(16)) {
                    int r = qran_range(0, 6);
                    const ai_instruction_t** lut[] = {
                        pattern_spray2,
                        pattern_broadside,
                        pattern_broadside2,
                        pattern_weave,
                        pattern_tricky_l,
                        pattern_tricky_r
                    };
                    ai->cur_ins = lut[r];
                }
                // Move towards player
                else if(xdiff < 0) { ai->cur_ins = pattern_move_l; }
                else { ai->cur_ins = pattern_move_r; }
                return;
            }
            case 4: {
                // Boss is in the corners
                if(boss.x < int2fx(-BOSS_BOUNDS_L)) {
                    ai->cur_ins = pattern_sweep_r3;
                } else if(boss.x > int2fx(BOSS_BOUNDS_R)) {
                    ai->cur_ins = pattern_sweep_l3;
                }
                // Player is ahead
                else if(xdist < int2fx(16)) {
                    int r = qran_range(0, 6);
                    const ai_instruction_t** lut[] = {
                        pattern_spray3,
                        pattern_broadside2,
                        pattern_weave,
                        pattern_weave2,
                        pattern_tricky_l,
                        pattern_tricky_r
                    };
                    ai->cur_ins = lut[r];
                }
                // Move towards player
                else if(xdiff < 0) { ai->cur_ins = pattern_move_lf; }
                else { ai->cur_ins = pattern_move_rf; }
                return;
            }
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

