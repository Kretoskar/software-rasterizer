#pragma once

#include <vector>

#include "BackBuffer.h"
#include "Color.h"
#include "Mesh.h"
#include "SRMath.h"

namespace SR
{
    struct Triangle
    {
        Vec3 v0;
        Vec3 v1;
        Vec3 v2;
        Color32 color;
    };

    class Rasterizer
    {
    public:
        explicit Rasterizer(Backbuffer& backbuffer)
            : _backbuffer(backbuffer) {}

        void BeginFrame();

        void DrawTriangle(const ScreenVertex& p0, const ScreenVertex& p1, const ScreenVertex& p2, Color32 color);
        
        void DrawMesh(const Mesh& mesh);

    private:
        float EdgeFunction(float ax, float ay, float bx, float by, float px, float py) const;
        void ResizeDepthBufferIfNeeded();

        Backbuffer& _backbuffer;
        std::vector<float> _depthBuffer;
    };
}
