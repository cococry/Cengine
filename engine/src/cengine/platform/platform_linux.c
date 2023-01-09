#include <cengine/macros.h>
#if CENGINE_IS_LINUX
#include "../core/event_system.h"
#include "../core/global_state.h"
#include "../core/logging.h"
#include "platform.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "opengl/ext/glcorearb.h"
#include "opengl/gl_loader.h"

static window *global_window = 0;

static bool8 update_messages() { return true; }

void cengine_input_keyboard_callback(GLFWwindow *win, int key, int scancode,
                                     int action, int mods) {

  window *w = (window *)glfwGetWindowUserPointer(win);

  if (!w)
    return;

  bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;

  w->input_state.key_changed = true;
  dispatch_event(key_changed_event, &key);
  if (w->input_state.keyboard_now.keys[key] != pressed) {
    w->input_state.keyboard_now.keys[key] = pressed;

    if (pressed) {
      w->input_state.key_went_down = true;
      dispatch_event(key_pressed_event, &key);
    } else {
      w->input_state.key_released = true;
      dispatch_event(key_released_event, &key);
    }
  }
}

// static input_struct input_state;

window *platform_window_create(window_properties props) {
  if (global_window != 0)
    return global_window;
  if (!glfwInit()) {
    fprintf(stderr, "Failed to init glfw.\n");
    return 0;
  }

  u32 clientx = props.xpos;
  u32 clienty = props.ypos;
  u32 client_width = props.width;
  u32 client_height = props.height;

  u32 windowx = clientx;
  u32 windowy = clienty;
  u32 window_width = client_width;
  u32 window_height = client_height;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_FLOATING, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  if (window_width <= 0)
    window_width = 640;
  if (window_height <= 0)
    window_height = 480;

  GLFWwindow *win =
      glfwCreateWindow(window_width, window_height, "Window", NULL, NULL);

  window *ret = (window *)malloc(sizeof(window));
  ret->backend_handle = win;
  ret->props = props;
  glfwSetWindowUserPointer(win, ret);
  glfwSetKeyCallback(win, cengine_input_keyboard_callback);
  glfwMakeContextCurrent(win);

  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW.\n");
    return 0;
  }
  glEnable(GL_TEXTURE_2D);

  // ASSERT_MSG(backend_handle->render_context, "Failed to create rendering
  // context.");

  // backend_handle->device_context = device_context;

  // QueryPerformanceFrequency(&ticks_per_second);
  // QueryPerformanceCounter(&last_tick_count);
  global_window = ret;
  return ret;
}

void platform_window_update(window *wnd) {
  update_messages();
  glfwSwapBuffers(wnd->backend_handle);

  g_state->app->state.delta_time = 1.0f / 60.0f;
}

bool8 platform_window_close_requested(window *wnd) {
  return !update_messages();
}

void platform_window_set_width(window *wnd, u32 width) {}

void platform_window_set_height(window *wnd, u32 height) {}

void platform_window_set_title(window *wnd, const char *title) {}
void platform_window_set_xpos(window *wnd, u32 xpos) {}

void platform_window_set_ypos(window *wnd, u32 ypos) {}

void platform_window_destroy(window *wnd) {}

i32 platform_get_mouse_x() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.xpos;
}

i32 platform_get_mouse_y() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.ypos;
}

i32 platform_get_mouse_x_delta() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.xpos - input_state.mouse_prev.xpos;
}

i32 platform_get_mouse_y_delta() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.ypos - input_state.mouse_prev.ypos;
}

i32 platform_get_mouse_scroll_x() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.scroll_x;
}

i32 platform_get_mouse_scroll_y() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.scroll_y;
}

i32 platform_get_mouse_scroll_x_delta() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.scroll_x - input_state.mouse_prev.scroll_x;
}

i32 platform_get_mouse_scroll_y_delta() {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.scroll_y - input_state.mouse_prev.scroll_y;
}

void _platform_process_key(keycode key, bool8 pressed) {
  input_struct *input_state = &global_window->input_state;
  input_state->key_changed = true;
  dispatch_event(key_changed_event, &key);
  if (input_state->keyboard_now.keys[key] != pressed) {
    input_state->keyboard_now.keys[key] = pressed;

    if (pressed) {
      input_state->key_went_down = true;
      dispatch_event(key_pressed_event, &key);
    } else {
      input_state->key_released = true;
      dispatch_event(key_released_event, &key);
    }
  }
}

void _platform_process_mouse_button(mouse_button button, bool8 pressed) {
  input_struct *input_state = &global_window->input_state;
  input_state->button_changed = true;
  dispatch_event(mouse_button_changed_event, &button);
  if (input_state->mouse_now.buttons[button] != pressed) {
    input_state->mouse_now.buttons[button] = pressed;

    if (pressed) {
      input_state->button_went_down = true;
      dispatch_event(mouse_button_pressed_event, &button);
    } else {
      input_state->button_released = true;
      dispatch_event(mouse_button_released_event, &button);
    }
  }
}

void _platform_process_mouse_wheel(u32 y_delta) {
#if CENGINE_IS_LINUX
  return;
#else
  input_state.mouse_now.scroll_x = 0;
  input_state.mouse_now.scroll_y = y_delta;

  dispatch_event(mouse_scrolled_event, &y_delta);
#endif
}

void _platform_process_mouse_move(u32 xpos, u32 ypos) {
#if CENGINE_IS_LINUX
  return;
#else
  if (input_state.mouse_now.xpos != xpos ||
      input_state.mouse_now.ypos != ypos) {
    input_state.mouse_now.xpos = xpos;
    input_state.mouse_now.ypos = ypos;

    u32 pos[2] = {xpos, ypos};
    dispatch_event(mouse_moved_event, &pos);
  }
#endif
}
void platform_input_update() {
  input_struct *input_state = &global_window->input_state;
  input_state->keyboard_prev = input_state->keyboard_now;
  input_state->mouse_prev = input_state->mouse_now;

  input_state->key_released = false;
  input_state->key_went_down = false;
  input_state->key_changed = false;

  input_state->button_released = false;
  input_state->button_went_down = false;
  input_state->button_changed = false;
}

void platform_input_init(input_struct *input) {
  input->keyboard_now.keys = malloc(sizeof(bool8) * KEYS_MAX_KEYS);
  input->keyboard_prev.keys = malloc(sizeof(bool8) * KEYS_MAX_KEYS);

  input->mouse_now.buttons = malloc(sizeof(bool8) * BUTTON_MAX_BUTTONS);
  input->mouse_prev.buttons = malloc(sizeof(bool8) * BUTTON_MAX_BUTTONS);

  input->mouse_now.scroll_x = 0;
  input->mouse_now.scroll_y = 0;
  input->mouse_now.xpos = 0;
  input->mouse_now.ypos = 0;

  input->mouse_prev.scroll_x = 0;
  input->mouse_prev.scroll_y = 0;
  input->mouse_prev.xpos = 0;
  input->mouse_prev.ypos = 0;

  memset(input->mouse_now.buttons, false, sizeof(bool8) * BUTTON_MAX_BUTTONS);
  memset(input->mouse_prev.buttons, false, sizeof(bool8) * BUTTON_MAX_BUTTONS);

  memset(input->keyboard_now.keys, false, sizeof(bool8) * KEYS_MAX_KEYS);
  memset(input->keyboard_prev.keys, false, sizeof(bool8) * KEYS_MAX_KEYS);

  input->key_went_down = false;
  input->key_released = false;
  input->key_changed = false;

  input->button_released = false;
  input->button_went_down = false;
  input->button_changed = false;
}

void platform_input_shutdown() {}

bool8 platform_key_went_down(keycode key) {
  input_struct input_state = global_window->input_state;
  return input_state.key_went_down &&
         input_state.keyboard_now.keys[key] == true;
}

bool8 platform_is_key_down(keycode key) {
  input_struct input_state = global_window->input_state;
  return input_state.keyboard_now.keys[key];
}

bool8 platform_is_key_released(keycode key) {
  input_struct input_state = global_window->input_state;
  return input_state.key_released &&
         input_state.keyboard_now.keys[key] == false;
}

bool8 platform_key_changed(keycode key) {
  input_struct input_state = global_window->input_state;
  return input_state.key_changed && input_state.keyboard_now.keys[key] !=
                                        input_state.keyboard_prev.keys[key];
}

bool8 platform_mouse_button_went_down(mouse_button button) {
  input_struct input_state = global_window->input_state;
  return input_state.button_went_down &&
         input_state.mouse_now.buttons[button] == true;
}

bool8 platform_is_mouse_button_down(mouse_button button) {
  input_struct input_state = global_window->input_state;
  return input_state.mouse_now.buttons[button];
}

bool8 platform_is_mouse_button_released(mouse_button button) {
  input_struct input_state = global_window->input_state;
  return input_state.button_released &&
         input_state.mouse_now.buttons[button] == false;
}

bool8 platform_mouse_button_changed(mouse_button button) {
  input_struct input_state = global_window->input_state;
  return input_state.button_changed &&
         input_state.mouse_now.buttons[button] !=
             input_state.mouse_prev.buttons[button];
}

#endif
