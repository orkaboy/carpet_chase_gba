#include "gamestate.h"

#include "gfx.h"

wizard_t player;
wizard_t boss;

void init_game(void) {
    init_wizard(&player, SPRITE_P_WIZARD, 5);
    init_wizard(&boss, SPRITE_E_WIZARD, 255);

    move_wizard(&player, int2fx(SCREEN_WIDTH/2 - 8), int2fx(SCREEN_HEIGHT - 32));
    move_wizard(&boss, int2fx(SCREEN_WIDTH/2 - 8), int2fx(32));
}
