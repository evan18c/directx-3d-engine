#include "Engine/Graphics/Model.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include "Engine/Utils/Files.h"

// Creates Model
Model::Model(ID3D11Device *device, Mesh *mesh, Texture *texture, Shader *shader) {

    // Construction
    m_position = { 0.0f, 0.0f, 0.0f };
    m_rotation = { 0.0f, 0.0f, 0.0f };
    m_scale    = { 1.0f, 1.0f, 1.0f };
    m_mesh = mesh;
    m_texture = texture;
    m_shader = shader;
    
}

// Returns Transform Matrix (M in MVP)
Mat4 Model::transform() {
    return Multiply(Multiply(Scale(m_scale), Rotate(m_rotation)), Translate(m_position));
}

// Returns AABB After Transformations
AABB Model::getAABB() {
    return m_mesh->m_aabb;
}