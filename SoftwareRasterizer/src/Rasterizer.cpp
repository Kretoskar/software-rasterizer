#include "Rasterizer.h"

#include <cmath>


using namespace SR;

void Rasterizer::BeginFrame()
{
    ResizeDepthBufferIfNeeded();
    std::fill(_depthBuffer.begin(), _depthBuffer.end(), std::numeric_limits<float>::infinity());
}

void Rasterizer::DrawTriangle(const ScreenVertex& p0, const ScreenVertex& p1, const ScreenVertex& p2, Color32 color)
{
    if (!p0.valid || !p1.valid || !p2.valid)
    {
        return;
    }
    
    const u32 width = _backbuffer.GetWidth();
    const u32 height = _backbuffer.GetHeight();

    if (width == 0 || height == 0)
    {
        return;
    }

    const float area = EdgeFunction(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    if (area <= 0.0f)
    {
        return;
    }

    // Compute AABB
    const i32 minX = std::max(0, static_cast<i32>(std::floor(std::min({ p0.x, p1.x, p2.x }))));
    const i32 minY = std::max(0, static_cast<i32>(std::floor(std::min({ p0.y, p1.y, p2.y }))));
    const i32 maxX = std::min(static_cast<i32>(width) - 1,  static_cast<i32>(std::ceil(std::max({ p0.x, p1.x, p2.x }))));
    const i32 maxY = std::min(static_cast<i32>(height) - 1, static_cast<i32>(std::ceil(std::max({ p0.y, p1.y, p2.y }))));

    const float invArea = 1.0f / area;

    for (i32 y = minY; y <= maxY; ++y)
    {
        for (i32 x = minX; x <= maxX; ++x)
        {
            const float px = static_cast<float>(x) + 0.5f;
            const float py = static_cast<float>(y) + 0.5f;

            // Check on which side of edges is the pixel
            const float w0 = EdgeFunction(p1.x, p1.y, p2.x, p2.y, px, py);
            const float w1 = EdgeFunction(p2.x, p2.y, p0.x, p0.y, px, py);
            const float w2 = EdgeFunction(p0.x, p0.y, p1.x, p1.y, px, py);

            const bool inside =
                (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) ||
                (w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f);

            if (!inside)
            {
                continue;
            }

            const float b0 = w0 * invArea;
            const float b1 = w1 * invArea;
            const float b2 = w2 * invArea;

            const float depth = b0 * p0.depth + b1 * p1.depth + b2 * p2.depth;

            const u32 index = static_cast<u32>(y) * width + static_cast<u32>(x);
            if (depth < _depthBuffer[index])
            {
                _depthBuffer[index] = depth;
                _backbuffer.PutPixel(static_cast<u32>(x), static_cast<u32>(y), color);
            }
        }
    }
}

void Rasterizer::DrawMesh(const Mesh& mesh)
{
    if (!mesh.IsValid())
    {
        return;
    }

    const size_t triangleCount = mesh.GetTriangleCount();
    for (size_t i = 0; i < triangleCount; ++i)
    {
        const u32 i0 = mesh.indices[i * 3 + 0];
        const u32 i1 = mesh.indices[i * 3 + 1];
        const u32 i2 = mesh.indices[i * 3 + 2];

        // TODO: actual SOA version
        DrawTriangle(mesh.screnPositions[i0], mesh.screnPositions[i1], mesh.screnPositions[i2], GetCiede2000Color(i));
    }
}

float Rasterizer::EdgeFunction(float ax, float ay, float bx, float by, float px, float py) const
{
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

void Rasterizer::ResizeDepthBufferIfNeeded()
{
    const u32 width = _backbuffer.GetWidth();
    const u32 height = _backbuffer.GetHeight();
    const size_t size = static_cast<size_t>(width) * static_cast<size_t>(height);

    if (_depthBuffer.size() != size)
    {
        _depthBuffer.resize(size);
    }
}
