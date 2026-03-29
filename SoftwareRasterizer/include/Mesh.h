#pragma once

#include <vector>
#include <string>

#include "BackBuffer.h"
#include "Color.h"
#include "Math.h"
#include "SRMath.h"
#include "Types.h"

namespace SR
{
    struct ScreenVertex
    {
        float x = 0.0f;
        float y = 0.0f;
        float depth = 0.0f;
        bool valid = true;
    };
    
    struct Mesh
    {
        std::vector<Vec3> positions;
        std::vector<ScreenVertex> screnPositions;
        
        // Triangle index buffer
        std::vector<u32> indices;

        Color32 color = Color32::FromRGBA(255, 255, 255, 255);

        void Clear();
        size_t GetVertexCount() const;

        size_t GetTriangleCount() const;
        bool IsValid() const;
        void FillScreenPositions(const Mat4& mvp, const Backbuffer& backbuffer);
        

        static void TransformToScreen(const Vec3& vertex, const Mat4& mvp, const Backbuffer& backbuffer, ScreenVertex& out);
    };
    
    class MeshFactory
    {
    public:
        static bool LoadFromFile(const std::string& path, Mesh& outMesh);
    };
}
