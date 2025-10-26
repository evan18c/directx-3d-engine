#include "Engine/Engine.h"
#include <d3dcompiler.h>

Shader::Shader(ID3D11Device *device, const char *vsCode, const char *psCode, Layout layout) {

    // Create Vertex Shader
    ID3DBlob *vsBlob;
    D3DCompile(vsCode, strlen(vsCode), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &vsBlob, NULL);
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vs);

    // Create Vertex Shader's Input Layout
    switch (layout) {
        case Layout::MODEL: {
            D3D11_INPUT_ELEMENT_DESC layout2[] = {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };
            device->CreateInputLayout(layout2, ARRAYSIZE(layout2), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_il);
            break;
        }
        case Layout::SPRITE: {
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
    D3DCompile(psCode, strlen(psCode), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &psBlob, NULL);
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_ps);
    psBlob->Release();

}

Shader *Shader::create(const char *vsPath, const char *psPath, Layout layout) {
    const char *vsCode = Files::ReadFile(vsPath);
    const char *psCode = Files::ReadFile(psPath);
    Shader *shader = new Shader(Engine::renderer->m_device, vsCode, psCode, layout);
    free((void *)vsCode);
    free((void *)psCode);
    return shader;
}

// Default Vertex Shader
const char *DEFAULT_VERTEX_SHADER_MODEL = R"(
struct LightData {
   float4 position;
   float4 color;
   float4 params;
};

struct VS_INPUT {
   float3 position : POSITION;
   float2 uv       : TEXCOORD;
   float3 normal   : NORMAL;
};

struct PS_INPUT {
   float4 position : SV_POSITION;
   float2 uv       : TEXCOORD;
   float3 normal   : NORMAL;
   float3 worldPos : POSITION;
};

cbuffer SceneBuffer : register(b0)
{
   row_major float4x4 model;
   row_major float4x4 view;
   row_major float4x4 projection;
   int lightCount;
   float3 pad;
   LightData lights[32];
}

PS_INPUT main(VS_INPUT input) {
   PS_INPUT output;

   float4 worldPos = mul(float4(input.position, 1.0f), model);
   float4 viewPos  = mul(worldPos, view);
   output.position = mul(viewPos, projection);

   output.normal = mul(float4(input.normal, 0.0f), model);
   output.uv = input.uv;
   output.worldPos = worldPos.xyz;

   return output;
}
)";

// Default Pixel Shader
const char *DEFAULT_PIXEL_SHADER_MODEL = R"(
struct LightData {
   float4 position;
   float4 color;
   float4 params;
};

cbuffer SceneBuffer : register(b0)
{
   row_major float4x4 model;
   row_major float4x4 view;
   row_major float4x4 projection;
   int lightCount;
   float3 pad;
   LightData lights[32];
};
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct PS_INPUT {
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
    float3 worldPos : POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 baseColor = tex0.Sample(samp0, input.uv).rgb;

    float3 N = normalize(input.normal);
    float3 color = baseColor * 0.1;

    for (int i = 0; i < lightCount; i++)
    {

        float3 lightPos = lights[i].position;
        float3 lightColor = lights[i].color;
        float intensity = lights[i].params.x;
        float radius = lights[i].params.y;

        float3 L = lightPos - input.worldPos;
        float dist = length(L);
        L = normalize(L);

        float atten = saturate(1.0 - dist / radius);
        float NdotL = max(dot(N, L), 0.0);
        
        float3 diffuse = baseColor * lightColor * NdotL * intensity * atten;
        color += diffuse;
    }
    color = saturate(color);
    return float4(color, 1.0);
}
)";

const char *DEFAULT_VERTEX_SHADER_SPRITE = R"(
cbuffer Transform : register(b0)
{
    row_major float4x4 transform;
}

struct VS_IN {
    float3 pos : POSITION;
    float2 uv  : TEXCOORD;
};

struct VS_OUT {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

VS_OUT main(VS_IN input) {
    VS_OUT output;
    output.pos = mul(float4(input.pos, 1.0f), transform);
    output.uv = input.uv;
    return output;
}
)";

const char *DEFAULT_PIXEL_SHADER_SPRITE = R"(
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

struct VS_OUT {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

float4 main(VS_OUT input) : SV_Target {
    return tex.Sample(samLinear, input.uv);
}
)";

Shader *Shader::default3D() {
    return new Shader(Engine::renderer->m_device, DEFAULT_VERTEX_SHADER_MODEL, DEFAULT_PIXEL_SHADER_MODEL, Layout::MODEL);
}

Shader *Shader::default2D() {
    return new Shader(Engine::renderer->m_device, DEFAULT_VERTEX_SHADER_SPRITE, DEFAULT_PIXEL_SHADER_SPRITE, Layout::SPRITE);
}