#include "Engine/Engine.h"
#include <vector>
#include <stdio.h>
#include <string.h>

Mesh::Mesh(ID3D11Device *device, const char *objPath) {

    // Object File Variables
    std::vector<Vec3> vertices;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;
    std::vector<int> vertexIndices;
    std::vector<int> uvIndices;
    std::vector<int> normalIndices;

    // Parsing Object File
    FILE *file = fopen(objPath, "r");
    while (true) {
        char lineHeader[256];
        int result = fscanf(file, "%s", lineHeader);
        if (result == EOF) break;

        // Vertex
        if (strcmp(lineHeader, "v") == 0) {
            Vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertex.z = -vertex.z; // flip
            vertices.push_back(vertex);
        }

        // UV
        else if (strcmp(lineHeader, "vt") == 0) {
            Vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uvs.push_back(uv);
        }

        // Normal
        else if (strcmp(lineHeader, "vn") == 0) {
            Vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normal.z = -normal.z; // flip
            normals.push_back(normal);
        }

        // Face
        else if (strcmp(lineHeader, "f") == 0) {
            int vi[3], ui[3], ni[3];
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vi[0], &ui[0], &ni[0], &vi[1], &ui[1], &ni[1], &vi[2], &ui[2], &ni[2]);
            vertexIndices.push_back(vi[0]);
            vertexIndices.push_back(vi[2]);
            vertexIndices.push_back(vi[1]);
            uvIndices.push_back(ui[0]);
            uvIndices.push_back(ui[2]);
            uvIndices.push_back(ui[1]);
            normalIndices.push_back(ni[0]);
            normalIndices.push_back(ni[2]);
            normalIndices.push_back(ni[1]);
        }
    }

    // Creating And Populating Vertex List
    m_vertexCount = vertexIndices.size();
    Vertex vertices2[m_vertexCount];
    for (int i=0; i<m_vertexCount; i++) {
        vertices2[i].pos = vertices[vertexIndices[i]-1];
        vertices2[i].uv = uvs[uvIndices[i]-1];
        vertices2[i].normal = normals[normalIndices[i]-1];

        // Storing Triangles For Physics Calculations
        m_triangles.push_back(vertices2[i].pos);
    }


    // Buffer Description
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(vertices2);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    // Vertex Data
    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices2;
    device->CreateBuffer(&desc, &data, &m_buffer);

}

Mesh *Mesh::create(const char *objPath) {
    return new Mesh(Engine::renderer->m_device, objPath);
}