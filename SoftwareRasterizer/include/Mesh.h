#pragma once

#include <vector>
#include <string>

#include "Color.h"
#include "Math.h"
#include "SRMath.h"
#include "Types.h"

namespace SR
{
    // Runtime / temporary vertex used after transform.
    struct Vertex
    {
        Vec3 position;
    };
    
    struct Mesh
    {
        std::vector<float> positionsX;
        std::vector<float> positionsY;
        std::vector<float> positionsZ;

        // Triangle index buffer
        std::vector<u32> indices;

        Color32 color = Color32::FromRGBA(255, 255, 255, 255);

        void Clear();
        size_t GetVertexCount() const;

        size_t GetTriangleCount() const;
        bool IsValid() const;
        Vec3 GetPosition(u32 index) const;
    };
    
    class MeshFactory
    {
    public:
        static Mesh CreateCube(float halfExtent = 0.5f);
    };
}
