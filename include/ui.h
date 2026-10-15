#ifndef UI_H
#define UI_H

#include <tonc.h>

//! Create a 15bit BGR color, using 8bit components
#define RGB8_(red, green, blue) ((red>>3) + ((green>>3)<<5) + ((blue>>3)<<10))

void draw_player_hp(void);
void draw_boss_hp(void);

void force_draw_player_hp(void);
void force_draw_boss_hp(void);

void render_text(int xoff, const char* text, COLOR col);

void update_ui_glow(void);

#endif
