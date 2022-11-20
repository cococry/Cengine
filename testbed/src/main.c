#include <cengine/core/application.h>

int main(void) {
    window_properties window_props;
    window_props.width = 1280;
    window_props.height = 720;
    window_props.title = "Cengine Testbed";
    window_props.xpos = 0;
    window_props.ypos = 30;
    window_props.vsync = true;
    window_props.resizable = true;
    application* app = application_create(window_props);
    application_run(app);
    application_shutdown(app);

    return 0;
}