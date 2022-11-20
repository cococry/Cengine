#ifdef _WIN32
#ifdef _WIN64

#include "platform.h"
#include "../core/logging.h"
#include "../core/event_system.h"
#include "../core/global_state.h"

#include "opengl/gl_loader.h"

#include "opengl/loader/glad_wgl.h"

#include <Windows.h>
#include <windowsx.h>
#include <stdlib.h>

input_struct input_state;

typedef struct win32_backend_handle {
    HINSTANCE instance;
    HWND window;
    u32 window_style;
    HDC device_context;
    HGLRC render_context;
} win32_backend_handle;

static LRESULT CALLBACK win32_update_messeges(HWND window, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
            dispatch_event(window_close_event, g_state->app);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // TODO: Dispatch event
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            bool8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            keycode key = (keycode)w_param;

            _platform_process_key(key, pressed);
        } break;
        case WM_MOUSEMOVE: {
            u32 xpos = GET_X_LPARAM(l_param);
            u32 ypos = GET_Y_LPARAM(l_param);

            _platform_process_mouse_move(xpos, ypos);
        } break;
        case WM_MOUSEWHEEL: {
            u32 y_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            if (y_delta != 0) {
                y_delta = (y_delta < 0) ? -1 : 1;

                _platform_process_mouse_wheel(y_delta);
            }

        } break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP: {
            bool8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            mouse_button button = BUTTON_MAX_BUTTONS;
            switch (msg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = BUTTON_LEFT;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = BUTTON_MIDDLE;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = BUTTON_RIGHT;
                    break;
                default:
                    break;
            }

            if (button != BUTTON_MAX_BUTTONS) {
                _platform_process_mouse_button(button, pressed);
            }
        } break;
    }

    return DefWindowProcA(window, msg, w_param, l_param);
}

static bool8 update_messages() {
    INT64 counts_per_sec = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);

    double sec_per_count = 1.0f / 1000.0f;

    INT64 prev_time = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&prev_time);

    MSG msg;
    if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT) {
            return false;
        }
    } else {
        INT64 current_time = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&current_time);

        g_state->app->state.delta_time = (float)((current_time - prev_time) * sec_per_count);
    }

    return true;
}

window* platform_window_create(window_properties props) {
    load_opengl();
    window* ret = (window*)malloc(sizeof(window));
    ASSERT_MSG(ret != nullptr || sizeof(ret) != sizeof(window), "Failed to create Win32 window.");

    ret->backend_handle = malloc(sizeof(win32_backend_handle));
    ASSERT_MSG(ret->backend_handle != nullptr || sizeof(ret->backend_handle) != sizeof(win32_backend_handle),
               "Failed to create Win32 window.");

    ret->props = props;

    win32_backend_handle* backend_handle = (win32_backend_handle*)ret->backend_handle;
    backend_handle->window_style = 0;
    backend_handle->instance = GetModuleHandleA(0);

    HICON icon = LoadIcon(backend_handle->instance, IDI_APPLICATION);
    WNDCLASSA window_class;
    window_class.style = CBN_DBLCLK;
    window_class.lpfnWndProc = win32_update_messeges;
    window_class.cbWndExtra = 0;
    window_class.cbClsExtra = 0;
    window_class.hInstance = backend_handle->instance;
    window_class.hIcon = icon;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = NULL;
    window_class.lpszClassName = "cengineWindow";

    ASSERT_MSG(RegisterClassA(&window_class), "Failed to register window class.");

    u32 clientx = props.xpos;
    u32 clienty = props.ypos;
    u32 client_width = props.width;
    u32 client_height = props.height;

    u32 windowx = clientx;
    u32 windowy = clienty;
    u32 window_width = client_width;
    u32 window_height = client_height;

    backend_handle->window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    backend_handle->window_style |= WS_MINIMIZEBOX;
    backend_handle->window_style |= WS_MAXIMIZEBOX;
    if (props.resizable)
        backend_handle->window_style |= WS_THICKFRAME;

    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, backend_handle->window_style, 0, window_ex_style);

    windowx += border_rect.left;
    windowy += border_rect.top;

    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom + border_rect.top;

    HWND handle = CreateWindowExA(window_ex_style, "cengineWindow",
                                  props.title, backend_handle->window_style, windowx, windowy, window_width, window_height,
                                  0, 0, backend_handle->instance, 0);

    ASSERT_MSG(handle, "Failed to create Win32 window handle.");

    backend_handle->window = handle;

    bool8 activate = true;
    i32 show_window_flags = activate ? SW_SHOW : SW_SHOWNOACTIVATE;

    ShowWindow(backend_handle->window, show_window_flags);

    LOG_TRACE("Created Win32 Window: '%s' (%ix%i), Position on screen: %ix%i\t Vsync: %s | Resizable: %s",
              props.title,
              props.width,
              props.height,
              props.xpos,
              props.ypos,
              props.vsync ? "On" : "Off",
              props.resizable ? "On" : "Off");

    HDC device_context = GetDC(backend_handle->window);

    int pixel_format_attributes[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0};

    int pixel_format = 0;
    UINT num_formats = 0;

    bool8 res = wglChoosePixelFormatARB(device_context, pixel_format_attributes, nullptr, 1, &pixel_format, &num_formats);

    ASSERT_MSG(res, "Failed to choose pixel format.");

    PIXELFORMATDESCRIPTOR pixel_format_desc = {};
    DescribePixelFormat(device_context, pixel_format, sizeof(PIXELFORMATDESCRIPTOR), &pixel_format_desc);
    SetPixelFormat(device_context, pixel_format, &pixel_format_desc);

    int opengl_attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0};

    backend_handle->render_context = wglCreateContextAttribsARB(device_context, 0, opengl_attributes);
    ASSERT_MSG(backend_handle->render_context, "Failed to create rendering context.");

    backend_handle->device_context = device_context;

    wglMakeCurrent(backend_handle->device_context, backend_handle->render_context);

    return ret;
}

void platform_window_update(window* wnd) {
    update_messages();

    wglSwapIntervalEXT(wnd->props.vsync);
    win32_backend_handle* backend_handle = (win32_backend_handle*)wnd->backend_handle;
    wglSwapLayerBuffers(backend_handle->device_context, WGL_SWAP_MAIN_PLANE);
}

bool8 platform_window_close_requested(window* wnd) {
    return !update_messages();
}

void platform_window_set_width(window* wnd, u32 width) {
    win32_backend_handle* backend_handle = (win32_backend_handle*)wnd->backend_handle;
    u32 window_x = 0, window_y = 0;
    RECT rect = {0, 0, 0, 0};
    if (GetWindowRect(backend_handle->window, &rect)) {
        window_x = rect.left;
        window_x = rect.top;
    }
    RECT* tile_client_area = 0;
    GetClientRect(backend_handle->window, tile_client_area);

    u32 height = tile_client_area->bottom - tile_client_area->top;

    SetWindowPos(backend_handle->window, backend_handle->window, window_x, window_y, width, height, SWP_NOREPOSITION);

    wnd->props.width = width;
}

void platform_window_set_height(window* wnd, u32 height) {
    win32_backend_handle* backend_handle = (win32_backend_handle*)wnd->backend_handle;
    u32 window_x = 0, window_y = 0;
    RECT rect = {0, 0, 0, 0};
    if (GetWindowRect(backend_handle->window, &rect)) {
        window_x = rect.left;
        window_x = rect.top;
    }
    RECT* tile_client_area = 0;
    GetClientRect(backend_handle->window, tile_client_area);

    u32 width = tile_client_area->right - tile_client_area->left;

    SetWindowPos(backend_handle->window, backend_handle->window, window_x, window_y, width, height, SWP_NOREPOSITION);

    wnd->props.height = height;
}

void platform_window_set_title(window* wnd, const char* title) {
    win32_backend_handle* backend_handle = (win32_backend_handle*)wnd->backend_handle;
    SetWindowTextA(backend_handle->window, title);
    wnd->props.title = title;
}
void platform_window_set_xpos(window* wnd, u32 xpos) {
    win32_backend_handle* backend_handle = (win32_backend_handle*)wnd->backend_handle;
    RECT* tile_client_area = 0;
    GetClientRect(backend_handle->window, tile_client_area);

    u32 width = tile_client_area->right - tile_client_area->left;
    u32 height = tile_client_area->bottom - tile_client_area->top;

    SetWindowPos(backend_handle->window, backend_handle->window, xpos, wnd->props.ypos, width, height, SWP_NOREPOSITION);

    wnd->props.xpos = xpos;
}

void platform_window_set_ypos(window* wnd, u32 ypos) {
    win32_backend_handle* backend_handle = (win32_backend_handle*)wnd->backend_handle;
    RECT* tile_client_area = 0;
    GetClientRect(backend_handle->window, tile_client_area);

    u32 width = tile_client_area->right - tile_client_area->left;
    u32 height = tile_client_area->bottom - tile_client_area->top;

    SetWindowPos(backend_handle->window, backend_handle->window, wnd->props.xpos, ypos, width, height, SWP_NOREPOSITION);

    wnd->props.ypos = ypos;
}

void platform_window_destroy(window* wnd) {
    win32_backend_handle* backend_handle = (win32_backend_handle*)wnd->backend_handle;
    DestroyWindow(backend_handle->window);
    free((void*)wnd);
}

i32 platform_get_mouse_x() {
    return input_state.mouse_now.xpos;
}

i32 platform_get_mouse_y() {
    return input_state.mouse_now.ypos;
}

i32 platform_get_mouse_x_delta() {
    return input_state.mouse_now.xpos - input_state.mouse_prev.xpos;
}

i32 platform_get_mouse_y_delta() {
    return input_state.mouse_now.ypos - input_state.mouse_prev.ypos;
}

i32 platform_get_mouse_scroll_x() {
    return input_state.mouse_now.scroll_x;
}

i32 platform_get_mouse_scroll_y() {
    return input_state.mouse_now.scroll_y;
}

i32 platform_get_mouse_scroll_x_delta() {
    return input_state.mouse_now.scroll_x - input_state.mouse_prev.scroll_x;
}

i32 platform_get_mouse_scroll_y_delta() {
    return input_state.mouse_now.scroll_y - input_state.mouse_prev.scroll_y;
}

void _platform_process_key(keycode key, bool8 pressed) {
    input_state.key_changed = true;
    dispatch_event(key_changed_event, &key);
    if (input_state.keyboard_now.keys[key] != pressed) {
        input_state.keyboard_now.keys[key] = pressed;

        if (pressed) {
            input_state.key_went_down = true;
            dispatch_event(key_pressed_event, &key);
        } else {
            input_state.key_released = true;
            dispatch_event(key_released_event, &key);
        }
    }
}

void _platform_process_mouse_button(mouse_button button, bool8 pressed) {
    input_state.button_changed = true;
    dispatch_event(mouse_button_changed_event, &button);
    if (input_state.mouse_now.buttons[button] != pressed) {
        input_state.mouse_now.buttons[button] = pressed;

        if (pressed) {
            input_state.button_went_down = true;
            dispatch_event(mouse_button_pressed_event, &button);
        } else {
            input_state.button_released = true;
            dispatch_event(mouse_button_released_event, &button);
        }
    }
}

void _platform_process_mouse_wheel(u32 y_delta) {
    input_state.mouse_now.scroll_x = 0;
    input_state.mouse_now.scroll_y = y_delta;

    dispatch_event(mouse_scrolled_event, &y_delta);
}

void _platform_process_mouse_move(u32 xpos, u32 ypos) {
    if (input_state.mouse_now.xpos != xpos || input_state.mouse_now.ypos != ypos) {
        input_state.mouse_now.xpos = xpos;
        input_state.mouse_now.ypos = ypos;

        u32 pos[2] = {xpos, ypos};
        dispatch_event(mouse_moved_event, &pos);
    }
}
void platform_input_update() {
    input_state.keyboard_prev = input_state.keyboard_now;
    input_state.mouse_prev = input_state.mouse_now;

    input_state.key_released = false;
    input_state.key_went_down = false;
    input_state.key_changed = false;

    input_state.button_released = false;
    input_state.button_went_down = false;
    input_state.button_changed = false;
}

void platform_input_init() {
    input_state.keyboard_now.keys = malloc(sizeof(bool8) * KEYS_MAX_KEYS);
    input_state.keyboard_prev.keys = malloc(sizeof(bool8) * KEYS_MAX_KEYS);

    input_state.mouse_now.buttons = malloc(sizeof(bool8) * BUTTON_MAX_BUTTONS);
    input_state.mouse_prev.buttons = malloc(sizeof(bool8) * BUTTON_MAX_BUTTONS);

    input_state.mouse_now.scroll_x = 0;
    input_state.mouse_now.scroll_y = 0;
    input_state.mouse_now.xpos = 0;
    input_state.mouse_now.ypos = 0;

    input_state.mouse_prev.scroll_x = 0;
    input_state.mouse_prev.scroll_y = 0;
    input_state.mouse_prev.xpos = 0;
    input_state.mouse_prev.ypos = 0;

    memset(input_state.mouse_now.buttons, false, sizeof(bool8) * BUTTON_MAX_BUTTONS);
    memset(input_state.mouse_prev.buttons, false, sizeof(bool8) * BUTTON_MAX_BUTTONS);

    memset(input_state.keyboard_now.keys, false, sizeof(bool8) * KEYS_MAX_KEYS);
    memset(input_state.keyboard_prev.keys, false, sizeof(bool8) * KEYS_MAX_KEYS);

    input_state.key_went_down = false;
    input_state.key_released = false;
    input_state.key_changed = false;

    input_state.button_released = false;
    input_state.button_went_down = false;
    input_state.button_changed = false;
}

void platform_input_shutdown() {
}

bool8 platform_key_went_down(keycode key) {
    return input_state.key_went_down;
}

bool8 platform_is_key_down(keycode key) {
    return input_state.keyboard_now.keys[key];
}

bool8 platform_is_key_released(keycode key) {
    return input_state.key_released;
}

bool8 platform_key_changed(keycode key) {
    return input_state.key_changed;
}

bool8 platform_mouse_button_went_down(mouse_button button) {
    return input_state.button_went_down;
}

bool8 platform_is_mouse_button_down(mouse_button button) {
    return input_state.mouse_now.buttons[button];
}

bool8 platform_is_mouse_button_released(mouse_button button) {
    return input_state.button_released;
}

bool8 platform_mouse_button_changed(mouse_button button) {
    return input_state.button_changed;
}

#endif
#endif