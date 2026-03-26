#pragma once
#include <windows.h>
#include <stdexcept>

#include "BackBuffer.h"
#include "Types.h"

namespace SR
{
class Window
{
public:
    Window(HINSTANCE instance, i16 width, i16 height, const wchar_t* title)
        : _instance(instance)
    {
        Register();
        Create(width, height, title);
    }

    ~Window();

    // Process Win32 messages
    bool ProcessMessages();

    // Show Win32 window
    void Show(i32 cmdShow);

    HWND Handle() const { return _hwnd; }
    Backbuffer& Buffer() { return _backbuffer; }

    // Push pixels to Win32 window
    void Present();

private:
    // Register Win32 window class
    void Register();

    // Spawn actual window
    void Create(int width, int height, const wchar_t* title);

    // Callback for window messages
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    // Push message to window object
    LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

    HINSTANCE _instance = nullptr;
    HWND _hwnd = nullptr;
    Backbuffer _backbuffer;
};
}


