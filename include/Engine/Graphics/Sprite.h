#pragma once
#include <d3d11.h>
#include "Engine/Core/Object.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/Shader.h"
#include "Engine/Math/Maths.h"

struct Vertex2D {
    Vec3 position;
    Vec2 uv;
};

// Manages 2D Sprites
class Sprite : public Object {

    public:
        static Sprite *create(Texture *texture, Shader *shader);
        Vec2 m_position;
        Vec2 m_size;
        Texture *m_texture;
        Shader *m_shader;
        ID3D11Buffer *m_buffer;
        Mat4 transform();
        void render(Renderer *renderer) override;

    private:
        Sprite(ID3D11Device *device, Texture *texture, Shader *shader);

};