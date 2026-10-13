#include <tonc.h>

#include "gamestate.h"
#include "gamelogic.h"
#include "gfx.h"
#include "ui.h"

#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"

int main(void)
{
	// Init interrupts and VBlank irq.
	irq_init(NULL);
    // Add Maxmod IRQ handler
	irq_add(II_VBLANK, mmVBlank);

    // Load Maxmod music
    mmInitDefault((mm_addr)soundbank_bin, 8);

    // Set start rand seed
    sqran(0);

    init_gfx();
    init_game();

    render_text("     Wizard!");

    // Start the Maxmod music on loop
    mmStart(MOD_ARABIA_2, MM_PLAY_LOOP);

	while(1)
	{
        // Update game logic
        update_game();
        // Update graphics (sprites update shadow OAM)
        update_gfx();
        // Wait for VBlank, so it's safe to update OAM
		VBlankIntrWait();
        // Maxmod, update music
		mmFrame();
        // Copy OAM shadow buffer etc.
        update_vblank();
	}

	return 0;
}
