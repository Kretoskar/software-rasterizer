#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdint>

#include "Rasterizer.h"
#include "Types.h"
#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    HINSTANCE instance = GetModuleHandleW(nullptr);

    SR::Window window(instance, 1280, 720, L"Software Rasterizer");
    window.Show(SW_SHOW);

    SR::Rasterizer rasterizer(window.Buffer());

    float angle = 0.0f;

    while (window.ProcessMessages())
    {
        SR::Backbuffer& buffer = window.Buffer();
        buffer.Clear(SR::Color32::FromRGBA(30, 30, 30, 255));
        rasterizer.BeginFrame();

        const float width = static_cast<float>(buffer.GetWidth());
        const float height = static_cast<float>(buffer.GetHeight());
        const float aspect = (height > 0.0f) ? (width / height) : 1.0f;

        SR::Triangle triangle;
        triangle.v0 = { -0.75f, -0.5f, 0.0f };
        triangle.v1 = {  0.75f, -0.5f, 0.0f };
        triangle.v2 = {  0.0f,   0.75f, 0.0f };
        triangle.color = SR::Color32::FromRGBA(255, 180, 40, 255);
        
        SR::Triangle triangle2;
        triangle2.v0 = { -0.75f, -0.5f, 0.0f };
        triangle2.v1 = {  0.75f, -0.5f, 0.0f };
        triangle2.v2 = {  0.0f,   0.75f, 0.0f };
        triangle2.color = SR::Color32::FromRGBA(0, 180, 40, 255);

        SR::Mat4 model =
            SR::Mat4::Translation(0.0f, 0.0f, 3.0f) *
            SR::Mat4::RotationY(angle) *
            SR::Mat4::Scale(2.0f, 2.0f, 2.0f);

        const SR::Mat4 view = SR::Mat4::Identity();

        const SR::Mat4 projection = SR::Mat4::Perspective(
            90.0f,
            aspect,
            0.1f,
            100.0f);

        rasterizer.DrawTriangle(triangle, model, projection * view);

        model = SR::Mat4::Translation(3.0f, 0.0f, 3.0f) *
            SR::Mat4::RotationY(angle);
        
        rasterizer.DrawTriangle(triangle2, model, projection * view);
        
        window.Present();
        angle += 0.01f;
    }

    return 0;
}