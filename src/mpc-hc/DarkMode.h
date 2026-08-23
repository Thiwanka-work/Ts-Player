/*
 * Dark Mode support for MPC-HC
 *
 * Applies Windows 10/11 dark title bar via DwmSetWindowAttribute,
 * and provides dark color constants / brush helpers for painting
 * toolbars, seek bar, status bar, and dialog backgrounds.
 *
 * Requires Windows 10 build 18362 (19H1) or later for full dark
 * title-bar support.  On earlier systems the window chrome stays
 * light, but the client-area colors are still applied.
 */

#pragma once

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

// ─── Colour palette ──────────────────────────────────────────────────────────
namespace DarkMode
{
    // Background colours
    constexpr COLORREF BG_WINDOW      = RGB(0x1E, 0x1E, 0x1E); // main window / panels
    constexpr COLORREF BG_TOOLBAR     = RGB(0x25, 0x25, 0x25); // toolbar / seekbar
    constexpr COLORREF BG_STATUSBAR   = RGB(0x18, 0x18, 0x18); // status bar
    constexpr COLORREF BG_DIALOG      = RGB(0x2D, 0x2D, 0x2D); // dialog boxes

    // Foreground (text) colours
    constexpr COLORREF FG_TEXT        = RGB(0xF0, 0xF0, 0xF0);
    constexpr COLORREF FG_TEXT_DIM    = RGB(0xA0, 0xA0, 0xA0);

    // Accent
    constexpr COLORREF ACCENT         = RGB(0x00, 0x78, 0xD4); // Windows blue

    // Seek-bar colours
    constexpr COLORREF SEEKBAR_TRACK  = RGB(0x40, 0x40, 0x40);
    constexpr COLORREF SEEKBAR_POS    = RGB(0x00, 0x78, 0xD4);
    constexpr COLORREF SEEKBAR_THUMB  = RGB(0xCC, 0xCC, 0xCC);

    // ─── Brushes (created on first use, never deleted – process lifetime) ─
    inline HBRUSH GetWindowBrush()
    {
        static HBRUSH hBr = CreateSolidBrush(BG_WINDOW);
        return hBr;
    }
    inline HBRUSH GetToolbarBrush()
    {
        static HBRUSH hBr = CreateSolidBrush(BG_TOOLBAR);
        return hBr;
    }
    inline HBRUSH GetStatusBarBrush()
    {
        static HBRUSH hBr = CreateSolidBrush(BG_STATUSBAR);
        return hBr;
    }
    inline HBRUSH GetDialogBrush()
    {
        static HBRUSH hBr = CreateSolidBrush(BG_DIALOG);
        return hBr;
    }

    // ─── Apply dark title bar to an HWND ─────────────────────────────────
    // Works on Windows 10 19H1+ (build 18362).
    // On older systems this is a no-op.
    inline void ApplyDarkTitleBar(HWND hWnd)
    {
        // DWMWA_USE_IMMERSIVE_DARK_MODE = 20 (available since build 18362)
        // The older, unofficial value 19 is tried as fallback for builds < 18362.
        constexpr DWORD DWMWA_USE_IMMERSIVE_DARK_MODE = 20;
        constexpr DWORD DWMWA_USE_IMMERSIVE_DARK_MODE_OLD = 19;

        BOOL bDark = TRUE;
        if (FAILED(DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &bDark, sizeof(bDark)))) {
            // Fallback for Windows 10 builds 17763–18362
            DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE_OLD, &bDark, sizeof(bDark));
        }
    }

} // namespace DarkMode
