#pragma once
#include <windows.h>
#include "Engine/Utils/Clock.h"

// Contains Window Functionality
class Window {

    public:
        Window(const int width, const int height, const char *title);
        BOOL update();
        HWND m_hwnd;
        int m_width;
        int m_height;
        static bool s_keys[256];
        static bool s_cursor;
        static double s_delta;
        static int s_mdx;
        static int s_mdy;

    private:
        double m_last;
        void showCursor();
        bool shownCursor;
        void hideCursor();
        bool hiddenCursor;
};