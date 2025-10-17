#pragma once
#include "Engine/Math/Maths.h"
#include <vector>
#include <d3d11.h>

// Vertex Structure
struct Vertex {
    Vec3 pos;
    Vec2 uv;
    Vec3 normal;
};

// Contains All Object Data, Verterices + UVs + Normals
class Mesh {

    public:
        AABB m_aabb;
        std::vector<Vec3> m_triangles;

    private:
        Mesh(ID3D11Device *device, const char *objPath);
        ID3D11Buffer *m_buffer;
        int m_vertexCount;

    friend class Renderer;
};