#include <cengine/core/application.h>

#include "game/game.h"

int main(void) {
    game_callbacks game_cbs = game_callbacks_create();
    game_cbs.game_init_cb = game_init;
    game_cbs.game_update_cb = game_update;
    game_cbs.game_terminate_cb = game_terminate;
    window_properties window_props;
    window_props.width = 1280;
    window_props.height = 720;
    window_props.title = "Cengine Testbed";
    window_props.xpos = 0;
    window_props.ypos = 30;
    window_props.vsync = true;
    window_props.resizable = true;
    application* app = application_create(window_props, game_cbs);
    application_run(app);
    application_shutdown(app);

    return 0;
}