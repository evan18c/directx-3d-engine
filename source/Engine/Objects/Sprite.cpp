#include "Engine/Engine.h"

Sprite::Sprite(ID3D11Device *device, Texture *texture, Shader *shader) {

    // ---------- Construction ---------- //
    m_position = { (float)Engine::window->m_width / 2.0f, (float)Engine::window->m_height / 2.0f };
    m_size = { 100.0f, 100.0f };
    m_texture = texture;
    m_shader = shader;

    // ---------- Buffer Creation ---------- //

    Vertex2D vertices[] = {
        { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f, 0.0f }
    };

    // Buffer Description
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(vertices);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    // Vertex Data
    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices;
    device->CreateBuffer(&desc, &data, &m_buffer);

}

// Returns Transformation Matrix
Mat4 Sprite::transform() {
    return MakeSpriteTransform(m_position.x, m_position.y, m_size.x, m_size.y, Engine::window->m_width, Engine::window->m_height);
}

Sprite *Sprite::create(Texture *texture, Shader *shader) {
    return new Sprite(Engine::renderer->m_device, texture, shader);
}

void Sprite::render(Renderer *renderer) {
    renderer->renderSprite(this);
}