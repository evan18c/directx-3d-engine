#include "Engine/Graphics/Model.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include "Engine/Utils/Files.h"
#include "Engine/Core/Renderer.h"

// Creates Model
Model::Model(Mesh *mesh, Shader *shader) {

    // Construction
    m_position = { 0.0f, 0.0f, 0.0f };
    m_rotation = { 0.0f, 0.0f, 0.0f };
    m_scale    = { 1.0f, 1.0f, 1.0f };
    m_mesh = mesh;
    m_shader = shader;
    m_type = ObjectType::MODEL;
    
}

// Returns Transform Matrix (M in MVP)
Mat4 Model::transform() {
    return Multiply(Multiply(Scale(m_scale), Rotate(m_rotation)), Translate(m_position));
}

// Returns AABB After Transformations
AABB Model::getAABB() {
    AABB aabb;
    Vec3 minV = { FLT_MAX,   FLT_MAX,  FLT_MAX };
    Vec3 maxV = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    for (Vec3 triangle : m_mesh->m_triangles) {
        Vec3 transformed = TransformPoint(triangle, transform());
        if (transformed.x < minV.x) minV.x = transformed.x;
        if (transformed.y < minV.y) minV.y = transformed.y;
        if (transformed.z < minV.z) minV.z = transformed.z;
        if (transformed.x > maxV.x) maxV.x = transformed.x;
        if (transformed.y > maxV.y) maxV.y = transformed.y;
        if (transformed.z > maxV.z) maxV.z = transformed.z;
    }
    aabb.min = minV;
    aabb.max = maxV;
    return aabb;
}

// Renders The Model
void Model::render(Renderer *renderer) {
    renderer->renderModel(this);
}

Model *Model::create(Mesh *mesh, Shader *shader) {
    return new Model(mesh, shader);
}