#include "gl_loader.h"

#include "loader/glad.h"
#include "loader/glad_wgl.h"

#include <Windows.h>

#include "../../core/logging.h"

void load_opengl() {
    WNDCLASSEX window_class = {};
    window_class.style = CS_OWNDC;
    window_class.lpfnWndProc = DefWindowProcA;
    window_class.lpszClassName = TEXT("glLoading");
    window_class.cbSize = sizeof(WNDCLASSEX);

    ATOM class_id = RegisterClassEx(&window_class);

    HWND window = CreateWindowEx(
        0,
        MAKEINTATOM(class_id),
        TEXT("glLoading"),
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        window_class.hInstance,
        0);

    ASSERT_MSG(window, "Failed to create GL loading window.");

    HDC window_dc = GetDC(window);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(window_dc, &pfd);
    SetPixelFormat(window_dc, pixelFormat, &pfd);

    HGLRC render_context = wglCreateContext(window_dc);
    ASSERT_MSG(render_context, "Failed to initialize Glad-loading rendering context.");

    bool8 res = wglMakeCurrent(window_dc, render_context);
    ASSERT_MSG(res, "Failed to set Glad-loading rendering context.");

    ASSERT_MSG(gladLoadWGL(window_dc), "Failed to initialize Glad with WGL.");

    ASSERT_MSG(gladLoadGL(), "Failed to load Glad.");
}