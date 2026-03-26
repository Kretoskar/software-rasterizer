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
    
    SR::Mesh mesh;
    SR::MeshFactory::LoadFromFile("../assets/dog.obj", mesh);

    mesh.color = SR::Color32::FromRGBA(255, 180, 40, 255);
    
    while (window.ProcessMessages())
    {
        SR::Backbuffer& buffer = window.Buffer();
        buffer.Clear(SR::Color32::FromRGBA(30, 30, 30, 255));
        rasterizer.BeginFrame();

        const float width = static_cast<float>(buffer.GetWidth());
        const float height = static_cast<float>(buffer.GetHeight());
        const float aspect = (height > 0.0f) ? (width / height) : 1.0f;

        const SR::Mat4 model =
            SR::Mat4::Translation(0.0f, 0.0f, 3.0f) *
            SR::Mat4::RotationY(angle) * 
            SR::Mat4::Scale(0.1f, 0.1f, 0.1f);

        const SR::Mat4 view = SR::Mat4::Identity();
        const SR::Mat4 projection = SR::Mat4::Perspective(90.0f, aspect, 0.1f, 100.0f);
        const SR::Mat4 viewProjection = projection * view;

        rasterizer.DrawMesh(mesh, model, viewProjection);

        window.Present();
        angle += 0.01f;
    }

    return 0;
}