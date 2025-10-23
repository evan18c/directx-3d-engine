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
        Model(Mesh *mesh, Shader *shader);
        static Model *create(Mesh *mesh, Shader *shader);
        Mesh *m_mesh;
        Shader *m_shader;
        Vec3 m_position;
        Vec3 m_rotation;
        Vec3 m_scale;
        AABB getAABB();
        Mat4 transform();
        void render(Renderer *renderer) override;
};