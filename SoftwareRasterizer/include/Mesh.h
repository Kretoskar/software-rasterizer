#pragma once

#include <vector>
#include <string>

#include "Color.h"
#include "Math.h"
#include "SRMath.h"
#include "Types.h"

namespace SR
{
    struct Mesh
    {
        std::vector<Vec3> positions;

        // Triangle index buffer
        std::vector<u32> indices;

        Color32 color = Color32::FromRGBA(255, 255, 255, 255);

        void Clear();
        size_t GetVertexCount() const;

        size_t GetTriangleCount() const;
        bool IsValid() const;
    };
    
    class MeshFactory
    {
    public:
        static bool LoadFromFile(const std::string& path, Mesh& outMesh);
    };
}
