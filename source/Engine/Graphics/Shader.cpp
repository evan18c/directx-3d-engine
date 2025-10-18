#include "Engine/Graphics/Shader.h"
#include "Engine/Utils/Files.h"
#include <d3dcompiler.h>

Shader::Shader(ID3D11Device *device, const char *vsPath, const char *psPath, Layout layout) {

    // Create Vertex Shader
    ID3DBlob *vsBlob;
    const char *vs = Files::ReadFile(vsPath);
    D3DCompile(vs, strlen(vs), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &vsBlob, NULL);
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vs);

    // Create Vertex Shader's Input Layout
    switch (layout) {
        case MODEL: {
            D3D11_INPUT_ELEMENT_DESC layout2[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };
            device->CreateInputLayout(layout2, ARRAYSIZE(layout2), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_il);
            break;
        }
        case SPRITE: {
            D3D11_INPUT_ELEMENT_DESC layout2[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            device->CreateInputLayout(layout2, ARRAYSIZE(layout2), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_il);
            break;
        }
    }
    vsBlob->Release();
    
    // Create Pixel Shader
    ID3DBlob *psBlob;
    const char *ps = Files::ReadFile(psPath);
    D3DCompile(ps, strlen(ps), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &psBlob, NULL);
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_ps);
    psBlob->Release();

}