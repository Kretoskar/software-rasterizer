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

        void DrawTriangle(
            const Triangle& triangle,
            const Mat4& model, 
            const Mat4& viewProjection);
        
        void DrawMesh(const Mesh& mesh, const Mat4& model, const Mat4& viewProjection);

    private:
        struct ScreenVertex
        {
            float x = 0.0f;
            float y = 0.0f;
            float depth = 0.0f;
        };

        bool TransformToScreen(const Vec3& vertex, const Mat4& mvp, ScreenVertex& out) const;
        float EdgeFunction(float ax, float ay, float bx, float by, float px, float py) const;
        void ResizeDepthBufferIfNeeded();

        Backbuffer& _backbuffer;
        std::vector<float> _depthBuffer;
    };
}
