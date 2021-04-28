// overlapping
inline b32 is_colliding(v2 p1, v2 half_size1, v2 p2, v2 half_size2) {
    // aabb vs aabb collision
    return  (p1.y + half_size1.y > p2.y - half_size2.y && 
            p1.y + half_size1.y < p2.y + half_size2.y && 
            p1.x + half_size1.x > p2.x - half_size2.x &&
            p1.x - half_size1.x < p2.x + half_size2.x); 
}

inline b32 sweep_for_aabb(v2 start, v2 end, v2 object_p, v2 object_half_size) {
    // Linear interp (1-t)A + tB = C, we are solving for C below, Start A, End B
    f32 t_y = (object_p.y - start.y) / (end.y - start.y);
    if (t_y >= 0.f && t_y <= 1.f) {
        return true;
    }
    return false;

}

inline void do_block_vs_ball_collision(Block *block, v2 block_half_size, v2 ball_p, v2 ball_desired_p) {
    if (sweep_for_aabb(ball_p, ball_desired_p, block->p, block_half_size)) {
        ball_dp.y *= -1;
    }
#if 0
    if (aabb_vs_aabb(block->p, block_half_size, ball_p, ball_half_size)) {            
        ball_dp.y *= -1;
        if(old_ball_p.x < (block->p.x - block_half_size.x)) {
            ball_dp.x *= -1;
        }
        if(old_ball_p.x > (block->p.x + block_half_size.x)) {
            ball_dp.x *= -1;
        }
        if(old_ball_p.y < (block->p.y - block_half_size.y)) {
            ball_dp.y *= -1;
        }
        if(old_ball_p.y > (block->p.y + block_half_size.y)) {
            ball_dp.y *= -1;
        }
#endif
/*
            if (collide_x_right) {
                ball_p.x *= -1;
                ball_p.x = block->p.x + block_half_size.x;
            } 
            else if (collide_x_left) {
                ball_p.x *= -1;
                ball_p.x = block->p.x + block_half_size.x;
            }

            if (collide_y_up) {
                ball_p.y *= -1;
                ball_p.y = block->p.y + block_half_size.y;
            } 
            else if (collide_y_down) {
                ball_p.y *= -1;
                ball_p.y = block->p.y + block_half_size.y;
            }
    b32 collide_y_down  = block->p.y - block_half_size.y < ball_p.y + ball_half_size.y; 
    b32 collide_y_up    = block->p.y + block_half_size.y > ball_p.y - ball_half_size.y; 

    b32 collide_x_left  = block->p.x - block_half_size.x < ball_p.x + ball_half_size.x; 
    b32 collide_x_right = block->p.x + block_half_size.x > ball_p.x - ball_half_size.x; 
            */
    
    
}