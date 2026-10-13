#include <tonc.h>

#include "gamelogic.h"
#include "gfx.h"

int main(void)
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

    init_gfx();

	while(1)
	{
        // Update game logic
        update_game();

        // TODO: Update graphics (sprites update shadow OAM)

        // Wait for VBlank, so it's safe to update OAM
		VBlankIntrWait();
        // Copy OAM shadow buffer
        update_oam();
        // TODO: Any other graphics regs that should be updated here?
	}

	return 0;
}
