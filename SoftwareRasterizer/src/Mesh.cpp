#include "Mesh.h"

using namespace SR;

void Mesh::Clear()
{
    positionsX.clear();
    positionsY.clear();
    positionsZ.clear();
    indices.clear();
}

size_t Mesh::GetVertexCount() const
{
    return positionsX.size();
}

size_t Mesh::GetTriangleCount() const
{
    return indices.size() / 3;
}

bool Mesh::IsValid() const
{
    return positionsX.size() == positionsY.size() &&
       positionsY.size() == positionsZ.size() &&
       (indices.size() % 3) == 0;
}

Vec3 Mesh::GetPosition(uint32_t index) const
{
    return
    {
        positionsX[index],
        positionsY[index],
        positionsZ[index]
    };
}

Mesh MeshFactory::CreateCube(float halfExtent)
{
    Mesh mesh;
    
    mesh.positionsX =
    {
        -halfExtent,  halfExtent,  halfExtent, -halfExtent,   // 0,1,2,3 back face
        -halfExtent,  halfExtent,  halfExtent, -halfExtent    // 4,5,6,7 front face
    };

    mesh.positionsY =
    {
        -halfExtent, -halfExtent,  halfExtent,  halfExtent,
        -halfExtent, -halfExtent,  halfExtent,  halfExtent
    };

    mesh.positionsZ =
    {
        -halfExtent, -halfExtent, -halfExtent, -halfExtent,
         halfExtent,  halfExtent,  halfExtent,  halfExtent
    };

    // 12 triangles, counter-clockwise when viewed from outside
    mesh.indices =
    {
        // Front  (+Z)
        4, 5, 6,
        4, 6, 7,

        // Back   (-Z)
        1, 0, 3,
        1, 3, 2,

        // Left   (-X)
        0, 4, 7,
        0, 7, 3,

        // Right  (+X)
        5, 1, 2,
        5, 2, 6,

        // Top    (+Y)
        3, 7, 6,
        3, 6, 2,

        // Bottom (-Y)
        0, 1, 5,
        0, 5, 4
    };

    mesh.color = Color32::FromRGBA(255, 180, 40, 255);
    return mesh;
}
