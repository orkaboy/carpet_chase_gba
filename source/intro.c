#include "intro.h"

#include "ui.h"
#include "gfx.h"
#include <tonc.h>
#include <maxmod.h>

typedef struct intro_text_t {
    int x_off;
    const char* str;
    COLOR text_col;
} intro_text_t;

#define PLAYER_TEXT_COL RGB8_(255, 197, 0)
#define ENEMY_TEXT_COL RGB8_(179, 40, 0)

#define INTRO_LINES 4

const intro_text_t _story[INTRO_LINES] = {
    {.x_off = 1, .str = "PRESS START", .text_col = 0},
    {.x_off = 1, .str = "YOU TOOK MY CARPET!", .text_col = PLAYER_TEXT_COL},
    {.x_off = 1, .str = "I TAKE WHAT I WANT!", .text_col = ENEMY_TEXT_COL},
    {.x_off = 1, .str = "I TAKE YOUR LIFE!", .text_col = PLAYER_TEXT_COL}
};

INLINE void render_line(const intro_text_t* text) {
    render_text(text->x_off, text->str, text->text_col);
}

void run_intro(void) {
    int intro_state = 0;

    update_gfx();

    clear_ui();
    render_line(&_story[0]);

    while(1) {
        key_poll();
        if(intro_state == 0) {
            if(key_hit(KEY_START)) {
                intro_state++;
                render_line(&_story[intro_state]);
            }
        } else {
            if(key_hit(KEY_A)) {
                intro_state++;
                if(intro_state >= INTRO_LINES) {
                    clear_ui();
                    force_draw_player_hp();
                    force_draw_boss_hp();
                    return;
                } else {
                    render_line(&_story[intro_state]);
                }
            }
        }

        VBlankIntrWait();
        // Maxmod, update music
		mmFrame();
        update_vblank();
    }
}
