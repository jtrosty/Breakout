internal void clear_screen(u32 color) {
    u32 *pixel = render_buffer.pixels;

    for (int y = 0; y < render_buffer.height; y++) {
        for (int x = 0; x < render_buffer.width; x++) {
            *pixel++ = color;
        }
    }
}

internal void draw_rect_int_pixels(int x0, int y0, int x1, int y1, u32 color) {
    x0 = clamp(0, x0, render_buffer.width);
    x1 = clamp(0, x1, render_buffer.width);
    y0 = clamp(0, y0, render_buffer.height);
    y1 = clamp(0, y1, render_buffer.height);

    for (int y = y0; y < y1; y++) {
        u32 *pixel = render_buffer.pixels + x0 + (render_buffer.width * y);
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

internal void draw_rect(v2 p, v2 half_size, u32 color) {
    // Convert the units to pixel and call draw_rect_in_pixels. 
    f32 aspect_multiplier = (f32) render_buffer.height;
    if(((f32)render_buffer.width / (f32) render_buffer.height) < 1.77f) 
        aspect_multiplier = (f32)render_buffer.width / 1.77f;

    f32 scale = 0.01f;
    half_size.x *= aspect_multiplier * scale;
    half_size.y *= aspect_multiplier * scale;

    p.x *= aspect_multiplier * scale;   
    p.y *= aspect_multiplier * scale;   

    p.x += (f32)render_buffer.width * .5f;   
    p.y += (f32)render_buffer.height * .5f;   

    int x0 = (int) (p.x - half_size.x);
    int y0 = (int) (p.y - half_size.y);
    int x1 = (int) (p.x + half_size.x);
    int y1 = (int) (p.y + half_size.y);

    draw_rect_int_pixels(x0, y0, x1, y1, color);
}