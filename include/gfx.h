#ifndef GFX_H
#define GFX_H

#include <tonc.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

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
extern OBJ_ATTR obj_buffer[NUM_HW_SPRITES];

void init_gfx(void);
void update_gfx(void);
// Updates that need to be done during vblank, like updating OAM
void update_vblank(void);

#endif
