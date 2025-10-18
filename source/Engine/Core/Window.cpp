#include "Engine/Core/Window.h"
#include "Engine/Core/Renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

// Static Initialization
int Window::s_width, Window::s_height;
Renderer *Window::s_renderer;
bool Window::s_keys[256] = {};
bool Window::s_cursor = false;
double Window::s_delta = 0;
int Window::s_mdx = 0;
int Window::s_mdy = 0;

// Main Window Callback Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostMessageA(hwnd, WM_QUIT, NULL, NULL);
            return 0;
        case WM_KEYDOWN:
            if (wParam < 256) Window::s_keys[wParam] = true;
            return 0;
        case WM_KEYUP:
            if (wParam < 256) Window::s_keys[wParam] = false;
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Initializes Window
Window::Window(const int width, const int height, const char *title) {

    // Init
    s_width = width;
    s_height = height;
    m_last = Clock::GetTimeSeconds();
    m_fpsCap = 180.0;

    // Creating Class
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = "DirectX_Engine";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);

    // Creating Window
    m_hwnd = CreateWindowExA(0, "DirectX_Engine", title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, GetModuleHandleA(NULL), NULL);
    ShowWindow(m_hwnd, SW_NORMAL);

    // Init Renderer
    s_renderer = new Renderer(m_hwnd, width, height);

    // ImGui Initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(s_renderer->m_device, s_renderer->m_context);

    // Init Cursor
    POINT center = {s_width/2, s_height/2};
    ClientToScreen(m_hwnd, &center);
    SetCursorPos(center.x, center.y);

}

// Updates Window.
// Returns FALSE if Window should close.
BOOL Window::update() {

    // Framerate Start
    double startTime = Clock::GetTimeMilliseconds();

    // Dispatches Window Message
    MSG msg = {};
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return FALSE;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Update Delta
    double current = Clock::GetTimeSeconds();
    s_delta = current - m_last;
    m_last = current;

    // Updating Mouse
    if (!s_cursor) {
        hideCursor();
        POINT mouse;
        GetCursorPos(&mouse);
        ScreenToClient(m_hwnd, &mouse);
        s_mdx = mouse.x - s_width / 2;
        s_mdy = mouse.y - s_height / 2;
        POINT center = {s_width/2, s_height/2};
        ClientToScreen(m_hwnd, &center);
        SetCursorPos(center.x, center.y);
    } else {
        showCursor();
        s_mdx = 0;
        s_mdy = 0;
    }

    // DirectX Rendering
    s_renderer->update();

    // Framerate End
    double frameTime = Clock::GetTimeMilliseconds() - startTime;
    double sleepTime = (1000.0 / m_fpsCap) - frameTime;
    if (sleepTime > 0) Sleep(sleepTime);

    // Return TRUE to continue
    return TRUE;

}

void Window::showCursor() {
    if (!shownCursor) {
        ShowCursor(TRUE);
        shownCursor = true;
        hiddenCursor = false;
    }
}

void Window::hideCursor() {
    if (!hiddenCursor) {
        ShowCursor(FALSE);
        hiddenCursor = true;
        shownCursor = false;
    }
}