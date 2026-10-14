#ifndef GFX_H
#define GFX_H

#include <tonc.h>

// BG layer 0 used for HUD
#define TEXT_CBB 1
#define TEXT_SBB 31
// Define BGs in VRAM
#define BG1_CBB 0
#define BG1_SBB 30
// Sprite CBB
#define OAM_CBB 4

// Background palettes
#define PAL_UI 0
#define PAL_BG 1
// Sprite palettes
#define PAL_SPR 0


// Define sprite positions in OAM
#define SPRITE_P_WIZARD 0
#define SPRITE_E_WIZARD (SPRITE_P_WIZARD+1)
#define SPRITE_P_CARPET (SPRITE_E_WIZARD+1)
#define SPRITE_E_CARPET (SPRITE_P_CARPET+1)
#define SPRITE_SHIELD (SPRITE_E_CARPET+1)

#define SPRITE_E_SUMMON0 (SPRITE_SHIELD+1)
#define NUM_E_SUMMONS 4

#define SPRITE_E_SPELL0 (SPRITE_E_SUMMON0 + NUM_E_SUMMONS)
#define NUM_E_SPELLS 32

#define SPRITE_P_SPELL0 (SPRITE_E_SPELL0 + NUM_E_SPELLS) 
#define NUM_P_SPELLS 8
// Max 128
#define HW_SPRITES_LAST (SPRITE_P_SPELL0 + NUM_P_SPELLS)

#define NUM_HW_SPRITES 128
// OAM shadow buffer
extern OBJ_ATTR obj_buffer[NUM_HW_SPRITES];

INLINE void draw_sprite(int hwsprite, int tid) {
    obj_set_attr(&obj_buffer[hwsprite], ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PRIO(1) | ATTR2_PALBANK(PAL_SPR) | tid);
}

INLINE void hide_sprite(int hwsprite) {
    obj_set_attr(&obj_buffer[hwsprite], ATTR0_HIDE, 0, 0);
}

#define SPRITE_SIZE 16
#define SPRITE_HSIZE (SPRITE_SIZE/2)

INLINE void move_sprite(int hwsprite, int x, int y) {
    obj_set_pos(&obj_buffer[hwsprite], x-SPRITE_HSIZE, y-SPRITE_HSIZE);
}

void init_gfx(void);
void update_gfx(void);
// Updates that need to be done during vblank, like updating OAM
void update_vblank(void);

#endif
