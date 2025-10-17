#pragma once
#include <windows.h>
#include "Engine/Utils/Clock.h"

// Forward Definition
class Renderer;

// Contains Window Functionality
class Window {

    public:
        Window(const int width, const int height, const char *title);
        BOOL update();
        static Renderer *s_renderer;
        static bool s_keys[256];
        static bool s_mouse;
        static double s_delta;
        static int s_mdx;
        static int s_mdy;
        double m_fpsCap;

    private:
        int m_width;
        int m_height;
        HWND m_hwnd;
        double m_last;
};