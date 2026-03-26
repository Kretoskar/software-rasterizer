#include "BackBuffer.h"

using namespace SR;

void Backbuffer::Resize(WindowCoord width, WindowCoord height)
{
    if (width <= 0 || height <= 0)
    {
        return;
    }

    Release();

    _width = width;
    _height = height;
    
    _info = {};
    _info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    _info.bmiHeader.biWidth = width;
    _info.bmiHeader.biHeight = -height; // top-down
    _info.bmiHeader.biPlanes = 1;
    _info.bmiHeader.biBitCount = 32;
    _info.bmiHeader.biCompression = BI_RGB;

    const size_t size = static_cast<size_t>(width) * static_cast<size_t>(height) * 4;
    _memory = VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    _pixels = static_cast<Color32*>(_memory);
    
    _stride = _width * sizeof(Color32);
    _stridePixels = _width;
}

void Backbuffer::Clear(Color32 color)
{
    if (!_pixels)
    {
        return;
    }

    const WindowCoord pixelCount = _width * _height;
    for (WindowCoord i = 0; i < pixelCount; ++i)
    {
        _pixels[i] = color;
    }
}

void Backbuffer::PutPixel(WindowCoord x, WindowCoord y, Color32 color)
{
    if (!_pixels)
    {
        return;
    }
    if (x < 0 || y < 0 || x >= _width || y >= _height)
    {
        return;
    }

    _pixels[y * _stridePixels + x] = color;
}

void Backbuffer::Present(HDC dc, WindowCoord clientWidth, WindowCoord clientHeight) const
{
    if (!_memory)
    {
        return;
    }
    
    StretchDIBits
    (
        dc,
        0, 0, clientWidth, clientHeight,
        0, 0, _width, _height,
        _memory,
        &_info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

void Backbuffer::Release()
{
    if (_memory)
    {
        VirtualFree(_memory, 0, MEM_RELEASE);
        _memory = nullptr;
    }
}
