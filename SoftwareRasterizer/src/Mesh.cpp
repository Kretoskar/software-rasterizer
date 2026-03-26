#include "Mesh.h"

#include <fstream>
#include <sstream>

using namespace SR;

namespace
{
    // Parse the vertex index from OBJ token
    // Supports:
    //   "3"
    //   "3/1"
    //   "3/1/2"
    //   "3//2"
    bool ParseObjVertexIndex(const std::string& token, u32& outIndex)
    {
        if (token.empty())
        {
            return false;
        }

        const size_t slashPos = token.find('/');
        const std::string indexPart = (slashPos == std::string::npos)
            ? token
            : token.substr(0, slashPos);

        if (indexPart.empty())
        {
            return false;
        }
        
        const i32 objIndex = std::stoi(indexPart);
        if (objIndex <= 0)
        {
            return false;
        }

        outIndex = static_cast<u32>(objIndex - 1);
        return true;
    }
}

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

bool MeshFactory::LoadFromFile(const std::string& path, Mesh& outMesh)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return false;
    }

    Mesh mesh;
    mesh.positionsX.reserve(16384);
    mesh.positionsY.reserve(16384);
    mesh.positionsZ.reserve(16384);

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;

        if (prefix == "v")
        {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            stream >> x >> y >> z;

            mesh.positionsX.push_back(x);
            mesh.positionsY.push_back(y);
            mesh.positionsZ.push_back(z);
        }
        else if (prefix == "f")
        {
            std::vector<u32> faceIndices;
            std::string token;

            while (stream >> token)
            {
                u32 vertexIndex = 0;
                if (!ParseObjVertexIndex(token, vertexIndex))
                {
                    return false;
                }

                faceIndices.push_back(vertexIndex);
            }

            if (faceIndices.size() < 3)
            {
                continue;
            }

            // Fan triangulation
            for (size_t i = 1; i + 1 < faceIndices.size(); ++i)
            {
                mesh.indices.push_back(faceIndices[0]);
                mesh.indices.push_back(faceIndices[i]);
                mesh.indices.push_back(faceIndices[i + 1]);
            }
        }
    }

    if (!mesh.IsValid())
    {
        return false;
    }

    outMesh = std::move(mesh);
    return true;
}
