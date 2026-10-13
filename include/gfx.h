#ifndef GFX_H
#define GFX_H

void init_gfx(void);
void update_gfx(void);
// Updates that need to be done during vblank, like updating OAM
void update_vblank(void);

#endif
