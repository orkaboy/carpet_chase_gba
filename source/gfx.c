#include "gfx.h"

#include <tonc.h>

// Define sprite positions in OAM
#define SPRITE_P_WIZARD 0
#define SPRITE_E_WIZARD (SPRITE_P_WIZARD+1)
#define SPRITE_P_CARPET (SPRITE_E_WIZARD+1)
#define SPRITE_E_CARPET (SPRITE_P_CARPET+1)
#define SPRITE_P_SHIELD (SPRITE_E_CARPET+1)
#define SPRITE_SHIELD_PU (SPRITE_P_SHIELD+1)

#define SPRITE_E_SUMMON0 (SPRITE_SHIELD_PU+1)
#define NUM_E_SUMMONS 4

#define SPRITE_E_SPELL0 (SPRITE_E_SUMMON0 + NUM_E_SUMMONS)
#define NUM_E_SPELLS 32

#define SPRITE_P_SPELL0 (SPRITE_E_SPELL0 + NUM_E_SPELLS) 
#define NUM_P_SPELLS 8
// Max 128
#define HW_SPRITES_LAST (SPRITE_P_SPELL0 + NUM_P_SPELLS)

#define NUM_HW_SPRITES 128
// OAM shadow buffer
OBJ_ATTR obj_buffer[NUM_HW_SPRITES];

void update_oam(void) {
    // Only need to copy sprites that are in use
    oam_copy(oam_mem, obj_buffer, HW_SPRITES_LAST);
}

// BG layer 0 used for HUD
#define TEXT_CBB 1
#define TEXT_SBB 31
// Define BGs in VRAM
#define BG1_CBB 0
#define BG1_SBB 30


void init_gfx(void) {

    // TODO: Load palettes

    // TODO: Load tiles (BG)

    // TODO: Load tiles (sprites)

    // TODO: Load tile-maps? Can generate these instead.

    // TODO: Configure BG registers
    
    // TODO: Configure OAM registers

    // Clear OAM
	oam_init(obj_buffer, NUM_HW_SPRITES);

	// Video mode 0, enable bg 0/1, enable sprites (1D).
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

