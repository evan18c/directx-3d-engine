#pragma once
#include <vector>
#include <functional>
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Engine/Utils/Files.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/Sprite.h"
#include "Engine/Math/Maths.h"

// Contains Code For Interacting With DirectX
class Renderer {

    public:
        // DirectX
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_context;
        IDXGISwapChain *m_swapChain;
        ID3D11RenderTargetView *m_renderTargetView;
        ID3D11DepthStencilView *m_depthStencilView;
        ID3D11DepthStencilState *m_depthStencilState;
        ID3D11BlendState *m_blendState;
        ID3D11SamplerState *m_samplerState;
        ID3D11RasterizerState *m_rasterState;
        ID3D11Buffer *m_sceneBuffer;
        ID3D11Buffer *m_transformBuffer2D;

        // Methods
        Renderer(HWND hwnd, const int width, const int height);
        void beginFrame();
        void endFrame();
        void renderModel(Model *model);
        void renderSprite(Sprite *sprite);
};

// Lights In A Scene
struct LightData {
    Vec4 position;
    Vec4 color;
    Vec4 params; // x = intensity, y = radius
};

// Scene Buffer, contains MVP and Lights
struct SceneBuffer
{
    Mat4 model;
    Mat4 view;
    Mat4 projection;
    int lightCount;
    float pad[3];
    LightData lights[32];
};

// 2D Scene Buffer
struct TransformBuffer2D
{
    Mat4 transform;
};