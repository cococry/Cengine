#pragma once

#include "../core/defines.h"

#define DEFINE_KEY(name, code) KEY_##name = code

typedef enum keycode {
    DEFINE_KEY(BACKSPACE, 0x08),
    DEFINE_KEY(ENTER, 0x0D),
    DEFINE_KEY(TAB, 0x09),
    DEFINE_KEY(SHIFT, 0x10),
    DEFINE_KEY(CONTROL, 0x11),

    DEFINE_KEY(PAUSE, 0x13),
    DEFINE_KEY(CAPITAL, 0x14),

    DEFINE_KEY(ESCAPE, 0x1B),

    DEFINE_KEY(CONVERT, 0x1C),
    DEFINE_KEY(NONCONVERT, 0x1D),
    DEFINE_KEY(ACCEPT, 0x1E),
    DEFINE_KEY(MODECHANGE, 0x1F),

    DEFINE_KEY(SPACE, 0x20),
    DEFINE_KEY(PRIOR, 0x21),
    DEFINE_KEY(NEXT, 0x22),
    DEFINE_KEY(END, 0x23),
    DEFINE_KEY(HOME, 0x24),
    DEFINE_KEY(LEFT, 0x25),
    DEFINE_KEY(UP, 0x26),
    DEFINE_KEY(RIGHT, 0x27),
    DEFINE_KEY(DOWN, 0x28),
    DEFINE_KEY(SELECT, 0x29),
    DEFINE_KEY(PRINT, 0x2A),
    DEFINE_KEY(EXECUTE, 0x2B),
    DEFINE_KEY(SNAPSHOT, 0x2C),
    DEFINE_KEY(INSERT, 0x2D),
    DEFINE_KEY(DELETE, 0x2E),
    DEFINE_KEY(HELP, 0x2F),

    DEFINE_KEY(A, 0x41),
    DEFINE_KEY(B, 0x42),
    DEFINE_KEY(C, 0x43),
    DEFINE_KEY(D, 0x44),
    DEFINE_KEY(E, 0x45),
    DEFINE_KEY(F, 0x46),
    DEFINE_KEY(G, 0x47),
    DEFINE_KEY(H, 0x48),
    DEFINE_KEY(I, 0x49),
    DEFINE_KEY(J, 0x4A),
    DEFINE_KEY(K, 0x4B),
    DEFINE_KEY(L, 0x4C),
    DEFINE_KEY(M, 0x4D),
    DEFINE_KEY(N, 0x4E),
    DEFINE_KEY(O, 0x4F),
    DEFINE_KEY(P, 0x50),
    DEFINE_KEY(Q, 0x51),
    DEFINE_KEY(R, 0x52),
    DEFINE_KEY(S, 0x53),
    DEFINE_KEY(T, 0x54),
    DEFINE_KEY(U, 0x55),
    DEFINE_KEY(V, 0x56),
    DEFINE_KEY(W, 0x57),
    DEFINE_KEY(X, 0x58),
    DEFINE_KEY(Y, 0x59),
    DEFINE_KEY(Z, 0x5A),

    DEFINE_KEY(LWIN, 0x5B),
    DEFINE_KEY(RWIN, 0x5C),
    DEFINE_KEY(APPS, 0x5D),

    DEFINE_KEY(SLEEP, 0x5F),

    DEFINE_KEY(NUMPAD0, 0x60),
    DEFINE_KEY(NUMPAD1, 0x61),
    DEFINE_KEY(NUMPAD2, 0x62),
    DEFINE_KEY(NUMPAD3, 0x63),
    DEFINE_KEY(NUMPAD4, 0x64),
    DEFINE_KEY(NUMPAD5, 0x65),
    DEFINE_KEY(NUMPAD6, 0x66),
    DEFINE_KEY(NUMPAD7, 0x67),
    DEFINE_KEY(NUMPAD8, 0x68),
    DEFINE_KEY(NUMPAD9, 0x69),
    DEFINE_KEY(MULTIPLY, 0x6A),
    DEFINE_KEY(ADD, 0x6B),
    DEFINE_KEY(SEPARATOR, 0x6C),
    DEFINE_KEY(SUBTRACT, 0x6D),
    DEFINE_KEY(DECIMAL, 0x6E),
    DEFINE_KEY(DIVIDE, 0x6F),
    DEFINE_KEY(F1, 0x70),
    DEFINE_KEY(F2, 0x71),
    DEFINE_KEY(F3, 0x72),
    DEFINE_KEY(F4, 0x73),
    DEFINE_KEY(F5, 0x74),
    DEFINE_KEY(F6, 0x75),
    DEFINE_KEY(F7, 0x76),
    DEFINE_KEY(F8, 0x77),
    DEFINE_KEY(F9, 0x78),
    DEFINE_KEY(F10, 0x79),
    DEFINE_KEY(F11, 0x7A),
    DEFINE_KEY(F12, 0x7B),
    DEFINE_KEY(F13, 0x7C),
    DEFINE_KEY(F14, 0x7D),
    DEFINE_KEY(F15, 0x7E),
    DEFINE_KEY(F16, 0x7F),
    DEFINE_KEY(F17, 0x80),
    DEFINE_KEY(F18, 0x81),
    DEFINE_KEY(F19, 0x82),
    DEFINE_KEY(F20, 0x83),
    DEFINE_KEY(F21, 0x84),
    DEFINE_KEY(F22, 0x85),
    DEFINE_KEY(F23, 0x86),
    DEFINE_KEY(F24, 0x87),

    DEFINE_KEY(NUMLOCK, 0x90),
    DEFINE_KEY(SCROLL, 0x91),

    DEFINE_KEY(NUMPAD_EQUAL, 0x92),

    DEFINE_KEY(LSHIFT, 0xA0),
    DEFINE_KEY(RSHIFT, 0xA1),
    DEFINE_KEY(LCONTROL, 0xA2),
    DEFINE_KEY(RCONTROL, 0xA3),
    DEFINE_KEY(LMENU, 0xA4),
    DEFINE_KEY(RMENU, 0xA5),

    DEFINE_KEY(SEMICOLON, 0xBA),
    DEFINE_KEY(PLUS, 0xBB),
    DEFINE_KEY(COMMA, 0xBC),
    DEFINE_KEY(MINUS, 0xBD),
    DEFINE_KEY(PERIOD, 0xBE),
    DEFINE_KEY(SLASH, 0xBF),
    DEFINE_KEY(GRAVE, 0xC0),

    KEYS_MAX_KEYS
} keycode;
typedef struct window_properties {
    u32 width, height;
    u32 xpos, ypos;
    const char* title;
    bool8 vsync, resizable;
} window_properties;

typedef struct window {
    window_properties props;
    void* backend_handle;
} window;

typedef struct keyboard_struct {
    bool8* keys;
} keyboard_struct;

typedef struct mouse_struct {
    bool8* buttons;

    i32 xpos, ypos;
    i32 scroll_x, scroll_y;
} mouse_struct;

typedef struct input_struct {
    keyboard_struct keyboard_prev;
    keyboard_struct keyboard_now;
    mouse_struct mouse_prev;
    mouse_struct mouse_now;

    bool8 key_went_down;
    bool8 key_released;
    bool8 key_changed;

    bool8 button_went_down;
    bool8 button_released;
    bool8 button_changed;

} input_struct;

window* platform_window_create(window_properties props);

void platform_window_update(window* wnd);

bool8 platform_window_close_requested(window* wnd);

void platform_window_set_width(window* wnd, u32 width);

void platform_window_set_height(window* wnd, u32 height);

void platform_window_set_title(window* wnd, const char* title);

void platform_window_set_xpos(window* wnd, u32 xpos);

void platform_window_set_ypos(window* wnd, u32 ypos);

void platform_window_destroy(window* wnd);

typedef enum mouse_button {
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_MIDDLE,
    BUTTON_MAX_BUTTONS
} mouse_button;

i32 platform_get_mouse_x();

i32 platform_get_mouse_y();

i32 platform_get_mouse_x_delta();

i32 platform_get_mouse_y_delta();

i32 platform_get_mouse_scroll_x();

i32 platform_get_mouse_scroll_y();

i32 platform_get_mouse_scroll_x_delta();

i32 platform_get_mouse_scroll_y_delta();

void _platform_process_key(keycode key, bool8 pressed);

void _platform_process_mouse_button(mouse_button button, bool8 pressed);

void _platform_process_mouse_wheel(u32 y_delta);

void _platform_process_mouse_move(u32 xpos, u32 ypos);

void platform_input_update();

void platform_input_init();

void platform_input_shutdown();

bool8 platform_key_went_down(keycode key);

bool8 platform_is_key_down(keycode key);

bool8 platform_is_key_released(keycode key);

bool8 platform_key_changed(keycode key);

bool8 platform_mouse_button_went_down(mouse_button button);

bool8 platform_is_mouse_button_down(mouse_button button);

bool8 platform_is_mouse_button_released(mouse_button button);

bool8 platform_mouse_button_changed(mouse_button button);
