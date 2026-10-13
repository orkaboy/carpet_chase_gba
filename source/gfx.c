#include "gfx.h"

#include <tonc.h>

#define NUM_HW_SPRITES 128 // Max 128
// OAM shadow buffer
OBJ_ATTR obj_buffer[NUM_HW_SPRITES];

void update_oam(void) {
    // TODO: not optimal, only need to copy sprites that move (and are in use)
    oam_copy(oam_mem, obj_buffer, NUM_HW_SPRITES);
}

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

