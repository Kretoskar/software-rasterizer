#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>

#include "Types.h"
#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    HINSTANCE instance = GetModuleHandleW(nullptr);

    SR::Window window(instance, 1280, 720, L"Software Rasterizer");
    window.Show(SW_SHOW);

    i32 t = 0;

    while (window.ProcessMessages())
    {
        SR::Backbuffer& buffer = window.Buffer();

        SR::Color32 clearColor = SR::Color32::FromRGBA(100, 100, 100, 255);
        buffer.Clear(clearColor);

        for (u16 y = 0; y < 255; ++y)
        {
            for (u16 x = 0; x < 255; ++x)
            {
                u8 r = x;
                u8 g = y;
                u8 b = 0;
                buffer.PutPixel(x, y, SR::Color32::FromRGBA(r,g,b));
            }
        }

        window.Present();
        ++t;
    }

    return 0;
}