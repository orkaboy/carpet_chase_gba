#include <tonc.h>

#include "gamestate.h"
#include "gamelogic.h"
#include "gfx.h"

int main(void)
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

    // Set start rand seed
    sqran(0);

    init_gfx();
    init_game();

	while(1)
	{
        // Update game logic
        update_game();
        // Update graphics (sprites update shadow OAM)
        update_gfx();
        // Wait for VBlank, so it's safe to update OAM
		VBlankIntrWait();
        // Copy OAM shadow buffer etc.
        update_vblank();
	}

	return 0;
}
