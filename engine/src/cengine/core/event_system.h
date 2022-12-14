#pragma once

#include "../core/defines.h"

#define MAX_EVENT_NAME_LENGTH 48

typedef enum event_type {
    window_close_event = 0,
    window_resize_event,
    key_pressed_event,
    key_released_event,
    key_changed_event,
    mouse_button_pressed_event,
    mouse_button_released_event,
    mouse_button_changed_event,
    mouse_scrolled_event,
    mouse_moved_event,
    quad_moved_event,
    max_events,
} event_type;

typedef void (*event_callback)(void*);

typedef struct event_handler {
    event_callback callback;
    struct event_handler* next;
    char name[48];
} event_handler;

extern event_handler* event_listeners[max_events];

void init_listeners(event_handler* handlers[], u32 size);

void destroy_listeners(event_handler* handlers[], u32 size);

void register_event(event_type type, event_callback callback, char* name);

void dispatch_event(event_type type, void* data);