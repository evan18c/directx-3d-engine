#pragma once
#include <d3d11.h>
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Math/Maths.h"

struct Vertex2D {
    Vec3 position;
    Vec2 uv;
};

// Manages 2D Sprites
class Sprite {

    public:
        Vec2 m_position;
        Vec2 m_size;
        Texture *m_texture;
        Shader *m_shader;
        Mat4 transform();

    private:
        Sprite(ID3D11Device *device, Texture *texture, Shader *shader);
        ID3D11Buffer *m_buffer;

    friend class Renderer;

};