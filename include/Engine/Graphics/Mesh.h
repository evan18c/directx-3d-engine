#pragma once
#include "Engine/Math/Maths.h"
#include <map>
#include <vector>
#include <d3d11.h>
#include <windef.h>

// Vertex Structure
struct Vertex {
    Vec3 pos;
    Vec2 uv;
    Vec3 normal;
};

// Faces Of The Mesh
struct Face {
    int vertices[3];
    int uvs[3];
    int normals[3];
    char material[MAX_PATH];
};

// Parts Of Mesh With Unique Material
struct MeshPart {
    ID3D11ShaderResourceView *srv;
    ID3D11Buffer *vertexBuffer;
    int vertexCount;
};

// Contains All Object Data, Verterices + UVs + Normals
class Mesh {

    public:
        Mesh(ID3D11Device *device, const char *objPath);
        static Mesh *create(const char *objPath);
        std::vector<MeshPart> m_parts;
        std::vector<Vec3> m_triangles;
};