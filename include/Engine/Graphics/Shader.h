#pragma once
#include <d3d11.h>

// Layout Of Shader
enum class Layout {
    MODEL,
    SPRITE
};

// Stores Vertex + Pixel Shader Information
class Shader {
    
    public:
        static Shader *create(const char *vsPath, const char *psPath, Layout layout);
        static Shader *default3D();
        static Shader *default2D();

    private:
        Shader(ID3D11Device *device, const char *vsCode, const char *psCode, Layout layout);
        ID3D11InputLayout *m_il;
        ID3D11VertexShader *m_vs;
        ID3D11PixelShader *m_ps;

    friend class Renderer;
};