#include "event_system.h"
#include "logging.h"
#include <stdlib.h>
#include <string.h>

event_handler* event_listeners[max_events];

void init_listeners(event_handler* handlers[], u32 size) {
    for (u32 i = 0; i < max_events; i++) {
        handlers[i] = nullptr;
    }
}

void destroy_listeners(event_handler* handlers[], u32 size) {
    event_handler *to_delete, *next;
    for (u32 i = 0; i < max_events; i++) {
        to_delete = handlers[i];

        while (to_delete) {
            next = to_delete->next;
            free(to_delete);
            to_delete = next;
        }
    }
}

void register_event(event_type type, event_callback callback, char* name) {
    event_handler** handlers = &event_listeners[type];

    if (*handlers == nullptr) {
        *handlers = (event_handler*)malloc(sizeof(event_handler));
        (*handlers)->callback = callback;

        strncpy((*handlers)->name, name, MAX_EVENT_NAME_LENGTH);
        (*handlers)->name[MAX_EVENT_NAME_LENGTH - 1] = '\0';

        (*handlers)->next = nullptr;
    } else {
        while ((*handlers)->next != nullptr) {
            (*handlers)++;
        }

        (*handlers)->next = (event_handler*)malloc(sizeof(event_handler));
        (*handlers)->next->callback = callback;

        strncpy((*handlers)->next->name, name, MAX_EVENT_NAME_LENGTH);
        (*handlers)->next->name[MAX_EVENT_NAME_LENGTH - 1] = '\0';

        (*handlers)->next->next = nullptr;
    }
}

void dispatch_event(event_type type, void* data) {
    event_handler* handlers = event_listeners[type];
    for (; handlers != nullptr; handlers = handlers->next) {
        handlers->callback(data);
    }
}