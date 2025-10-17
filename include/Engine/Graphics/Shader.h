#pragma once
#include <d3d11.h>

// Stores Vertex + Pixel Shader Information
class Shader {
    
    private:
        Shader(ID3D11Device *device, const char *vsPath, const char *psPath);
        ID3D11InputLayout *m_il;
        ID3D11VertexShader *m_vs;
        ID3D11PixelShader *m_ps;

    friend class Renderer;
};