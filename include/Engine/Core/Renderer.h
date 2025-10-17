#pragma once
#include <vector>
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Engine/Utils/Files.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Math/Maths.h"

// Contains Code For Interacting With DirectX
class Renderer {

    public:
        Camera *m_camera;
        void renderModel(Model *model);

        // Factory Creation Methods
        Mesh *createMesh(const char *objFile);
        Texture *createTexture(const char *bmpFile);
        Shader *createShader(const char *vsPath, const char *psPath);
        Model *createModel(Mesh *mesh, Texture *texture, Shader *shader);

        // Models To Be Rendered
        std::vector<Model *> *m_modelList;
        
    private:
        // Methods
        Renderer(HWND hwnd, const int width, const int height);
        void update();

        // Window
        int m_width, m_height;

        // DirectX
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_context;
        IDXGISwapChain *m_swapChain;
        ID3D11RenderTargetView *m_renderTargetView;
        ID3D11DepthStencilView *m_depthStencilView;
        ID3D11DepthStencilState *m_depthStencilState;
        ID3D11Buffer *m_transformBuffer;

    friend class Window;

};

// Buffer For Interacting With HLSL
struct TransformBuffer
{
    Mat4 model;
    Mat4 view;
    Mat4 projection;
};