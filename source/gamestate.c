#include "gamestate.h"

#include "gfx.h"

wizard_t player;
wizard_t boss;

void init_game(void) {
    init_wizard(&player, 0, SPRITE_P_WIZARD, PLAYER_MAX_HP);
    init_wizard(&boss, 16, SPRITE_E_WIZARD, BOSS_MAX_HP);

    move_wizard(&player, int2fx(SCREEN_WIDTH/2), int2fx(SCREEN_HEIGHT - 32));
    move_wizard(&boss, int2fx(SCREEN_WIDTH/2), int2fx(32));
}
