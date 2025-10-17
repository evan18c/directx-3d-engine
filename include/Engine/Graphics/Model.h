#pragma once
#include <d3d11.h>
#include "Engine/Math/Maths.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Shader.h"

// Manages Models
class Model {

    public:
        Mesh *m_mesh;
        Texture *m_texture;
        Shader *m_shader;
        Vec3 m_position;
        Vec3 m_rotation;
        Vec3 m_scale;
        AABB getAABB();

    private:
        Model(ID3D11Device *device, Mesh *mesh, Texture *texture, Shader *shader);
        Mat4 transform();

    friend class Renderer;
};