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
    positions.clear();
    positions.clear();
    positions.clear();
    indices.clear();
}

size_t Mesh::GetVertexCount() const
{
    return positions.size();
}

size_t Mesh::GetTriangleCount() const
{
    return indices.size() / 3;
}

bool Mesh::IsValid() const
{
    return (indices.size() % 3) == 0;
}

bool MeshFactory::LoadFromFile(const std::string& path, Mesh& outMesh)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return false;
    }

    Mesh mesh;
    mesh.positions.reserve(16384);
    
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

            mesh.positions.emplace_back(x,y,z);
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
