#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdint>
#include <chrono>
#include <vector>
#include <numeric>
#include <sstream>

#include "Rasterizer.h"
#include "Types.h"
#include <random>
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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    float rand[100];
    for (float& x : rand)
    {
        x = dist(gen);
    }

    std::vector<double> frameTimesMs;
    frameTimesMs.reserve(10000);

    using clock = std::chrono::high_resolution_clock;

    while (window.ProcessMessages())
    {
        auto frameStart = clock::now();

        SR::Backbuffer& buffer = window.Buffer();
        buffer.Clear(SR::Color32::FromRGBA(30, 30, 30, 255));
        rasterizer.BeginFrame();

        const float width = static_cast<float>(buffer.GetWidth());
        const float height = static_cast<float>(buffer.GetHeight());
        const float aspect = (height > 0.0f) ? (width / height) : 1.0f;

        SR::Mat4 model =
            SR::Mat4::Translation(0.0f, -1.5f, 5.0f) *
            SR::Mat4::RotationXYZ(SR::Deg2Rad(-90.0f), angle, 0.0f) *
            SR::Mat4::Scale(0.1f, 0.1f, 0.1f);

        const SR::Mat4 view = SR::Mat4::Identity();
        const SR::Mat4 projection = SR::Mat4::Perspective(90.0f, aspect, 0.1f, 100.0f);
        const SR::Mat4 viewProjection = projection * view;

        rasterizer.DrawMesh(mesh, model, viewProjection);

        for (float i : rand)
        {
            model = SR::Mat4::Translation(i, 0.0f, 0.0f) * model;
            rasterizer.DrawMesh(mesh, model, viewProjection);
        }

        window.Present();
        angle += 0.02f;

        auto frameEnd = clock::now();
        double frameMs = std::chrono::duration<double, std::milli>(frameEnd - frameStart).count();
        frameTimesMs.push_back(frameMs);
    }

    if (!frameTimesMs.empty())
    {
        double totalMs = std::accumulate(frameTimesMs.begin(), frameTimesMs.end(), 0.0);
        double averageMs = totalMs / static_cast<double>(frameTimesMs.size());

        std::ostringstream oss;
        oss << "Frames: " << frameTimesMs.size() << "\n"
            << "Average frame time: " << averageMs << " ms\n";

        MessageBoxA(nullptr, oss.str().c_str(), "Frame Timing", MB_OK);
    }

    return 0;
}