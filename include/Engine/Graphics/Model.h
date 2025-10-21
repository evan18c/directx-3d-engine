#pragma once
#include <d3d11.h>
#include "Engine/Core/Object.h"
#include "Engine/Math/Maths.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Shader.h"

// Manages Models
class Model : public Object {

    public:
        static Model *create(Mesh *mesh, Texture *texture, Shader *shader);
        Model(Mesh *mesh, Texture *texture, Shader *shader);
        Mesh *m_mesh;
        Texture *m_texture;
        Shader *m_shader;
        Vec3 m_position;
        Vec3 m_rotation;
        Vec3 m_scale;
        AABB getAABB();
        Mat4 transform();
        void render(Renderer *renderer) override;
};