#include "gamestate.h"

#include "gfx.h"
#include "spell.h"

wizard_t player;
shield_t shield;
wizard_t boss;
boss_ai_t boss_ai;

void init_game(void) {
    init_wizard(&player, 0, SPRITE_P_WIZARD, PLAYER_MAX_HP);
    init_wizard(&boss, 16, SPRITE_E_WIZARD, BOSS_MAX_HP);
    init_shield(&shield);
    init_boss_ai(&boss_ai);

    init_spell_manager();

    move_wizard(&player, int2fx(SCREEN_WIDTH/2), int2fx(SCREEN_HEIGHT - 32));
    move_wizard(&boss, int2fx(SCREEN_WIDTH/2), int2fx(48));
}
