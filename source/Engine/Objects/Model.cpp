#include "Engine/Engine.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include <float.h>

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
    return Multiply(Translate(m_position), Multiply(Rotate(m_rotation), Scale(m_scale)));
}

// Returns AABB After Transformations
AABB Model::getAABB() {
    return TransformAABB(m_mesh->m_aabb, transform());
}

// Renders The Model
void Model::render(Renderer *renderer) {
    renderer->renderModel(this);
}

Model *Model::create(Mesh *mesh, Shader *shader) {
    return new Model(mesh, shader);
}