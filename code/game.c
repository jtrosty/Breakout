/*
TODO: 
(OK) initial platform layer (window, rendering, input time)
Initial Basic gameplay
Engine improments (audio, threading, bitmap)
More gameplay exploration
polish pass (engine and game) 

*/

v2 player_p;
v2 player_dp;
v2 player_half_size;
f32 ball_base_speed;
f32 ball_speed_multiplier;

v2 ball_p;
v2 ball_dp;
v2 ball_half_size;

v2 block_size;
v2 block_half_size;

struct {
    v2 p;
    f32 ball_speed_multiplier;
    u32 color;
    int life;
} typedef Block;

Block blocks[256];
int next_block;

v2 arena_half_size;

b32 initialized = false;
b32 first_ball_movement = true;

#include "collision.c"

enum {
    GM_NORMAL,
    GM_WALL,
    GM_CONSTRUCTION,
    GM_SPACED,
    GM_PONG,
} typedef Game_Mode;

internal void start_game(Game_Mode game_mode) {
    initialized = false;
    first_ball_movement = true;
    ball_dp.x = 0.f;
    ball_p.x = 0.f;

    ball_base_speed = -50;
    ball_dp.y = ball_base_speed;
    ball_half_size = (v2){.75, .75};

    player_p.y = -40;
    player_half_size = (v2){10, 2};

    arena_half_size = (v2){85, 45};

    block_half_size = (v2) {4, 2};

    next_block = 0; // @Incomplete: Reset teh blocks here

    switch (game_mode) {
        case GM_NORMAL: {
    #define num_x 20 
    #define num_y 12 

            f32 x_offset = block_half_size.x * num_x;
            f32 y_offset = block_half_size.y * num_y - 25;
            

            for (int y = 0; y < num_y; y++) {
                for (int x = 0; x < num_x; x++) {
                    Block *block = blocks + next_block++;
                    if (next_block >= array_count(blocks)) {
                        next_block = 0;
                    }
                    block->life = 1;
                    block->p.x = x * block_half_size.x * 2.1f - x_offset;
                    block->p.y = y * block_half_size.y * 2.1f- y_offset - 10; // @Hardcoded value
                    block->color = make_color_from_grey((y*  255 / num_y));
                    block->ball_speed_multiplier = 1 + (f32)y * 1.25f / (f32)num_y;
                }
            }
        } break;
        case GM_WALL: {

        } break;
        default: {
            assert(0);
        } 
    }
}

internal void simulate_game(Input *input, f32 dt) {
    if (!initialized) {
        start_game(0);
    }

    f32 speed = 40;
    
    f32 player_new_x = pixels_to_world(input->mouse).x;
    player_dp.x = (player_new_x - player_p.x) / dt;
    player_p.x = player_new_x;
    v2 ball_desired_p = add_v2(ball_p, mul_v2(ball_dp, dt));
    v2 player_desired_p;
    player_desired_p.x = pixels_to_world(input->mouse).x;
    player_desired_p.y = player_p.y;

    //ball_p = add_v2(ball_p, mul_v2(ball_dp, dt));

    // Axis Aligned Bounding Box (AABB for collision detection)
    // @robustness: subtract waht the ball already moved to get to colliding position.
    if (ball_dp.y < 0 && is_colliding(player_p, player_half_size, ball_desired_p, ball_half_size)) {
        //is_colliding = true; // FOR TESTING @DELETE LATER
        // Player collision with ball
        ball_dp.y *= -1;
        //ball_dp.x += clamp(-100, player_dp.x, 100);
        ball_dp.x = (ball_p.x - player_p.x) * 7.5f;
        ball_desired_p.y = player_p.y + player_half_size.y;
        first_ball_movement = false;
    } 
    else if (ball_desired_p.x + ball_half_size.x > arena_half_size.x) {
        ball_desired_p.x = arena_half_size.x - ball_half_size.x;
        ball_dp.x *= -1;
    }
    else if (ball_desired_p.x - ball_half_size.x < -arena_half_size.x) {
        ball_desired_p.x = -arena_half_size.x + ball_half_size.x;
        ball_dp.x *= -1;
    }
    if (ball_desired_p.y + ball_half_size.x > arena_half_size.y) {
        ball_desired_p.y = arena_half_size.y - ball_half_size.y;
        ball_dp.y *= -1;
    }

#if 0
    if (ball_desired_p.y - ball_half_size.x < -arena_half_size.y) {
        ball_desired_p.y = -arena_half_size.y + ball_half_size.y;
        ball_dp.y *= -1;
    }
#endif


    clear_screen_and_draw_rec((v2) {0,0}, arena_half_size, 0x551100, 0x220500);
    for (Block *block = blocks; block != blocks + array_count(blocks); block ++) {
        if (!block->life) continue;

        if(!first_ball_movement) {
            {
                // This is the Ball/block collision, starts with the y. 
                f32 diff = ball_desired_p.y - ball_p.y;
                if (diff != 0) {
                    f32 collision_point;
                    if (ball_dp.y > 0) collision_point = block->p.y - block_half_size.y - ball_half_size.y;
                    else collision_point = block->p.y + block_half_size.y + ball_half_size.y;
                    f32 t_y = (collision_point - ball_p.y) / diff;
                    if (t_y >= 0.f && t_y <= 1.f) { 
                        f32 target_x = lerp(ball_p.x, t_y, ball_desired_p.x);
                        if(target_x + ball_half_size.x > block->p.x - block_half_size.x && 
                        target_x - ball_half_size.x < block->p.x + block_half_size.x) {
                                ball_desired_p.y = lerp(ball_p.y, t_y, ball_desired_p.y);
                                //ball_dp.y *= -1;
                                if (block->ball_speed_multiplier > ball_speed_multiplier) ball_speed_multiplier = block->ball_speed_multiplier;
                                if (ball_dp.y > 0) ball_dp.y = -ball_base_speed * ball_speed_multiplier;
                                else ball_dp.y = ball_base_speed * ball_speed_multiplier;
                                block->life--;
                        } 
                    }
                }
            }

            {
                // Calculate a New Diff for x-asiz collisions
                f32 diff = ball_desired_p.x - ball_p.x;
                if (diff != 0) {
                    f32 collision_point;
                    if (ball_dp.x > 0) collision_point = block->p.x - block_half_size.x - ball_half_size.x;
                    else collision_point = block->p.x + block_half_size.x + ball_half_size.x;
                    f32 t_x = (collision_point - ball_p.x) / diff;
                    if (t_x >= 0.f && t_x <= 1.f) { 
                        f32 target_y = lerp(ball_p.y, t_x, ball_desired_p.y);
                        if(target_y + ball_half_size.y > block->p.y - block_half_size.y && 
                        target_y - ball_half_size.y < block->p.y + block_half_size.y) {
                                ball_desired_p.x = lerp(ball_p.x, t_x, ball_desired_p.x);
                                ball_dp.x *= -1;
                                if (block->ball_speed_multiplier > ball_speed_multiplier) ball_speed_multiplier = block->ball_speed_multiplier;
                                if (ball_dp.y > 0) ball_dp.y = ball_base_speed * ball_speed_multiplier;
                                else ball_dp.y = -ball_base_speed * ball_speed_multiplier;
                                block->life--;
                        } 
                    }
                }
            }
            //do_block_vs_ball_collision(block, block_half_size, ball_p, ball_desired_p);
        }
        draw_rect(block->p, block_half_size, block->color);
    }

    ball_p = ball_desired_p;
    player_dp.x = (player_desired_p.x - player_p.x) / dt;
    player_p = player_desired_p;

    draw_rect( ball_p, ball_half_size, 0x00ffff);
    draw_rect( player_p, player_half_size, 0x00ff00);

    // Game over
    if (ball_desired_p.y - ball_half_size.y < -50) {
        start_game(0);
    } 
}

