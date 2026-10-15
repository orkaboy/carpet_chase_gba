#include "shield.h"

#include "collision.h"
#include "gfx.h"
#include "gamestate.h"

#define SHIELD_BORDER_OFFSET 24
#define SHIELD_TILE_SIZE 4
#define SHIELD_TID (11*SHIELD_TILE_SIZE)
#define SHIELD_PU_TID0 (12*SHIELD_TILE_SIZE)
#define SHIELD_PU_FRAME_CNT 15
#define SHIELD_PU_NUM_FRAMES 2
#define SHIELD_PU_COLL_SIZE 8

void init_shield(shield_t* s) {
    s->state = S_COOLDOWN;
    s->cooldown = SHIELD_INIT_COOLDOWN;
    s->x = 0;
    s->y = 0;
    s->anim_frame = 0;
    s->frame_timer = SHIELD_PU_FRAME_CNT;
}

void update_shield(shield_t* s) {
    switch(s->state) {
    case S_INIT:
        // Reset cooldown
        s->cooldown = SHIELD_COOLDOWN;
        s->anim_frame = 0;
        s->frame_timer = SHIELD_PU_FRAME_CNT;
        s->state = S_COOLDOWN;
        break;
    case S_COOLDOWN:
        s->cooldown--;
        if(s->cooldown <= 0) {
            s->state = S_SPAWNED;
            // Spawn the powerup top of the screen
            s->x = int2fx(qran_range(SHIELD_BORDER_OFFSET, SCREEN_WIDTH - SHIELD_BORDER_OFFSET));
            s->y = int2fx(-16);
            draw_sprite(SPRITE_SHIELD, SHIELD_PU_TID0);
            move_sprite(SPRITE_SHIELD, fx2int(s->x), fx2int(s->y));
        }
        break;
    case S_SPAWNED:
        s->y += SHIELD_Y_SPEED;
        s->frame_timer--;
        if(s->frame_timer <= 0) {
            s->frame_timer = SHIELD_PU_FRAME_CNT;
            s->anim_frame++;
            if(s->anim_frame >= SHIELD_PU_NUM_FRAMES) {
                s->anim_frame = 0;
            }
            // Update animation
            draw_sprite(SPRITE_SHIELD, SHIELD_PU_TID0 + (s->anim_frame*SHIELD_TILE_SIZE));
        }
        move_sprite(SPRITE_SHIELD, fx2int(s->x), fx2int(s->y));
        // Check for OOB
        if(s->y > SCREEN_OOB_B) {
            s->state = S_INIT;
            obj_hide(&obj_buffer[SPRITE_SHIELD]);
        } else {
            // Check for pickup
            update_bb(
                BB_SHIELD,
                fx2int(s->x) - SHIELD_PU_COLL_SIZE/2,
                fx2int(s->y) - SHIELD_PU_COLL_SIZE/2,
                SHIELD_PU_COLL_SIZE,
                SHIELD_PU_COLL_SIZE
            );
            if(check_collision(BB_PLAYER, BB_SHIELD)) {
                s->state = S_HELD;
                obj_hide(&obj_buffer[SPRITE_SHIELD]);
            }
        }
        break;
    case S_HELD:
        // TODO: Have a button trigger this?
        s->state = S_ACTIVE;
        s->cooldown = SHIELD_DURATION;
        break;
    case S_ACTIVE:
        s->x = player.x;
        s->y = player.y - int2fx(16);
        draw_sprite(SPRITE_SHIELD, SHIELD_TID);
        move_sprite(SPRITE_SHIELD, fx2int(s->x), fx2int(s->y));
        // Update BB
        update_bb(
            BB_SHIELD,
            fx2int(s->x) - SHIELD_HITBOX_W/2,
            fx2int(s->y) - SHIELD_HITBOX_H/2,
            SHIELD_HITBOX_W,
            SHIELD_HITBOX_H
        );
        // Check expiration
        s->cooldown--;
        if(s->cooldown <= 0) {
            obj_hide(&obj_buffer[SPRITE_SHIELD]);
            s->state = S_INIT;
        }
        break;
    default:
        s->state = S_INIT;
        break;
    }
}
