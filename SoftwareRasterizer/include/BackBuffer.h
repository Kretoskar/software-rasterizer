#pragma once

#define NOMINMAX
#include <windows.h>
#include <cstdint>

#include "Color.h"
#include "Types.h"

namespace SR
{
    typedef u32 WindowCoord;
    
    class Backbuffer
    {
    public:
        Backbuffer() = default;
        ~Backbuffer() { Release(); }
    
        // Resize the window with the bacbuffer
        void Resize(WindowCoord width, WindowCoord height);
        
        // Clear the window with given color
        void Clear(Color32 color);
        // Put pixel to backbuffer
        void PutPixel(WindowCoord x, WindowCoord y, Color32 color);
    
        // Preset the backbuffer to window
        void Present(HDC dc, WindowCoord clientWidth, WindowCoord clientHeight) const;
    
        WindowCoord GetWidth() const { return _width; }
        WindowCoord GetHeight() const { return _height; }
    
    private:
        void Release();
    
        // raw memory of tightly packed pixels, stored row by row
        // so to get pixel at xy - y * width + x;
        void* _memory = nullptr;
        // _memory cast to Color32 ptr for easier access
        Color32* _pixels = nullptr;
        
        WindowCoord _width = 0;
        WindowCoord _height = 0;
        
        // stride to get to next element in _memory
        i32 _stride = 0;
        // stride to get to next element in _pixels
        i32 _stridePixels = 0;
        
        // format description of backbuffer for Win32
        BITMAPINFO _info = {};
    };
}
