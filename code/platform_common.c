struct { 
    b32 is_down;
    b32 changed;
} typedef Button;

enum {
    BUTTON_LEFT,
    BUTTON_RIGHT, 
    BUTTON_UP,
    BUTTON_DOWN,

    BUTTON_COUNT,
};

struct {
    int mouse_x;
    int mouse_y;

    Button buttons[BUTTON_COUNT];

} typedef Input;

// Spimplies the usage of the code in other places.
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
#define is_down(b) (input->buttons[b].is_down)