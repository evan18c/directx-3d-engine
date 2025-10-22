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
        ID3D11Buffer *m_transformBuffer3D;
        ID3D11Buffer *m_transformBuffer2D;

        // Methods
        Renderer(HWND hwnd, const int width, const int height);
        void beginFrame();
        void endFrame();
        void renderModel(Model *model);
        void renderSprite(Sprite *sprite);
};

// Buffers For Interacting With HLSL
struct TransformBuffer3D
{
    Mat4 model;
    Mat4 view;
    Mat4 projection;
};
struct TransformBuffer2D
{
    Mat4 transform;
};