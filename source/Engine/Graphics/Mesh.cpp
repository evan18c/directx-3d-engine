#include "Engine/Engine.h"
#include <string>
#include <vector>
#include <filesystem>
#include <stdio.h>
#include <string.h>

Mesh::Mesh(ID3D11Device *device, const char *objPath) {

    // Object File Variables
    std::vector<Vec3> vertices;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;
    std::vector<Face> faces;

    // Parsing Object File
    FILE *file = fopen(objPath, "r");
    char mtllib[MAX_PATH];
    char currentMaterial[MAX_PATH];
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
            Face face;
            face.vertices[0] = vi[0];
            face.vertices[1] = vi[2];
            face.vertices[2] = vi[1];
            face.uvs[0] = ui[0];
            face.uvs[1] = ui[2];
            face.uvs[2] = ui[1];
            face.normals[0] = ni[0];
            face.normals[1] = ni[2];
            face.normals[2] = ni[1];
            strcpy(face.material, currentMaterial);
            faces.push_back(face);
        }

        // Material Library
        else if (strcmp(lineHeader, "mtllib") == 0) {
            fscanf(file, "%s\n", mtllib);
        }

        // Material
        else if (strcmp(lineHeader, "usemtl") == 0) {
            fscanf(file, "%s\n", currentMaterial);
        }
    }
    fclose(file);

    // Organizing Vertices
    std::map<std::string, std::vector<Vertex>> materialVertices;
    for (Face face : faces) {
        for (int i=0; i<3; i++) {
            Vertex v;
            v.pos = vertices[face.vertices[i] - 1];
            v.uv = uvs[face.uvs[i] - 1];
            v.normal = normals[face.normals[i] - 1];
            materialVertices[face.material].push_back(v);
            m_triangles.push_back(v.pos); // used for physics
        }
    }

    // Dividing m_triangles into chunks for faster physics calculations
    const int TRIANGLES_PER_CHUNK = 50;
    for (int i=0; i<m_triangles.size(); i+=TRIANGLES_PER_CHUNK*3) {

        // Create new chunk
        TriangleChunk chunk;

        // Copy Triangles
        for (int j=i; j<i+TRIANGLES_PER_CHUNK*3 && j<m_triangles.size(); j++) {
            chunk.triangles.push_back(m_triangles[j]);
        }

        // Calculate AABB
        Vec3 minV = { FLT_MAX,   FLT_MAX,  FLT_MAX };
        Vec3 maxV = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
        for (Vec3 triangle : chunk.triangles) {
            if (triangle.x < minV.x) minV.x = triangle.x;
            if (triangle.y < minV.y) minV.y = triangle.y;
            if (triangle.z < minV.z) minV.z = triangle.z;
            if (triangle.x > maxV.x) maxV.x = triangle.x;
            if (triangle.y > maxV.y) maxV.y = triangle.y;
            if (triangle.z > maxV.z) maxV.z = triangle.z;
        }
        chunk.aabb.min = minV;
        chunk.aabb.max = maxV;

        // Add to chunk list
        m_chunks.push_back(chunk);
    }

    // Directory Of Object File
    std::string objDir = std::filesystem::path(objPath).parent_path().string();

    // Opening Material Library
    std::string mtlPath = objDir + "/" + mtllib;
    MaterialFile mf = MaterialFile(mtlPath.c_str());

    // Creating MeshParts
    for (const auto &[key, val] : materialVertices) {

        // MeshPart
        MeshPart mp;

        // Vertex Count
        mp.vertexCount = val.size();

        // Creating Texture
        std::string imgPath = objDir + "/" + mf.m_materials[key]->map_Kd;
        Texture *texture = Texture::create(imgPath.c_str());
        mp.srv = texture->m_srv;
        delete texture;

        // Buffer Description
        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = val.size() * sizeof(Vertex);
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        // Vertex Data
        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = val.data();

        // Creating Buffer
        device->CreateBuffer(&desc, &data, &mp.vertexBuffer);

        // Update m_parts Vector
        m_parts.push_back(mp);
    }

    // Calculating AABB
    Vec3 minV = { FLT_MAX,   FLT_MAX,  FLT_MAX };
    Vec3 maxV = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    for (Vec3 triangle : m_triangles) {
        if (triangle.x < minV.x) minV.x = triangle.x;
        if (triangle.y < minV.y) minV.y = triangle.y;
        if (triangle.z < minV.z) minV.z = triangle.z;
        if (triangle.x > maxV.x) maxV.x = triangle.x;
        if (triangle.y > maxV.y) maxV.y = triangle.y;
        if (triangle.z > maxV.z) maxV.z = triangle.z;
    }
    m_aabb.min = minV;
    m_aabb.max = maxV;

}

Mesh *Mesh::create(const char *objPath) {
    return new Mesh(Engine::renderer->m_device, objPath);
}