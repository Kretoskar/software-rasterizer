#include "Window.h"

using namespace SR;

Window::~Window()
{
    if (_hwnd)
    {
        DestroyWindow(_hwnd);
    }
}

bool Window::ProcessMessages()
{
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

void Window::Show(i32 cmdShow)
{
    ShowWindow(_hwnd, cmdShow);
}

void Window::Present()
{
    RECT r{};
    GetClientRect(_hwnd, &r);

    HDC dc = GetDC(_hwnd);
    _backbuffer.Present(dc, r.right - r.left, r.bottom - r.top);
    ReleaseDC(_hwnd, dc);
}

void Window::Register()
{
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = &Window::StaticWndProc;
    wc.hInstance = _instance;
    wc.lpszClassName = L"SoftwareRasterizerWindowClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassExW(&wc))
    {
        throw std::runtime_error("RegisterClassExW failed");
    }
}

void Window::Create(int width, int height, const wchar_t* title)
{
    _hwnd = CreateWindowExW
    (
        0,
        L"SoftwareRasterizerWindowClass",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        _instance,
        this
    );

    if (!_hwnd)
    {
        throw std::runtime_error("CreateWindowExW failed");
    }
}

LRESULT Window::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* window = nullptr;

    if (msg == WM_NCCREATE)
    {
        auto* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        window = static_cast<Window*>(create->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->_hwnd = hwnd;
    }
    else
    {
        window = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (window)
    {
        return window->WndProc(msg, wParam, lParam);
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        {
            i32 width = LOWORD(lParam);
            i32 height = HIWORD(lParam);
            _backbuffer.Resize(width, height);
            return 0;
        }

    case WM_PAINT:
        {
            PAINTSTRUCT ps{};
            HDC dc = BeginPaint(_hwnd, &ps);

            RECT r{};
            GetClientRect(_hwnd, &r);
            _backbuffer.Present(dc, r.right - r.left, r.bottom - r.top);

            EndPaint(_hwnd, &ps);
            return 0;
        }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(_hwnd, msg, wParam, lParam);
}
