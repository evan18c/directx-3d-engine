#include "Engine/Core/Renderer.h"
#include "Engine/Core/Window.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

// Initializes Direct3D
Renderer::Renderer(HWND hwnd, const int width, const int height) {

    // -------------------- 1. Create Device and Swap Chain -------------------- //
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_context);

    // -------------------- 2. Create Render View -------------------- //
    ID3D11Texture2D *backBuffer;
    m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);
    m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
    backBuffer->Release();

    // -------------------- 3. Create Depth Buffer and Stencil View -------------------- //
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    ID3D11Texture2D* depthBuffer;
    m_device->CreateTexture2D(&depthDesc, NULL, &depthBuffer);
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    m_device->CreateDepthStencilView(depthBuffer, &dsvDesc, &m_depthStencilView);
    depthBuffer->Release();
    m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView); // Bind Render Target AND Depth Buffer

    // -------------------- 4. Create and set Viewport -------------------- //
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &viewport);

    // -------------------- 4. Create Depth Stencil State -------------------- //
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS; // standard depth test
    m_device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);
    m_context->OMSetDepthStencilState(m_depthStencilState, 0);

    // -------------------- 6. Create Transform Buffer -------------------- //
    D3D11_BUFFER_DESC transformBufferDesc = {};
    transformBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    transformBufferDesc.ByteWidth = sizeof(TransformBuffer);
    transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transformBufferDesc.CPUAccessFlags = 0;
    m_device->CreateBuffer(&transformBufferDesc, NULL, &m_transformBuffer);

    // -------------------- 10. Create Model List -------------------- //
    m_modelList = new std::vector<Model *>();

    // -------------------- 11. Create Camera -------------------- //
    m_camera = new Camera();
    
}

// Called Every Window Update
void Renderer::update() {

    // Clear Window
    float clearColor[4] = { 0.0f, 0.25f, 0.5f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Initialize Drawing
    m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Update Camera
    m_camera->update();

    // Render Models
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    for (int i=0; i<m_modelList->size(); i++) {

        // Set Model Shaders
        m_context->IASetInputLayout(m_modelList->at(i)->m_shader->m_il);
        m_context->VSSetShader(m_modelList->at(i)->m_shader->m_vs, NULL, 0);
        m_context->PSSetShader(m_modelList->at(i)->m_shader->m_ps, NULL, 0);

        // Uploading Vertices + Textures + Lightning
        m_context->IASetVertexBuffers(0, 1, &m_modelList->at(i)->m_mesh->m_buffer, &stride, &offset);
        m_context->PSSetShaderResources(0, 1, &m_modelList->at(i)->m_texture->m_srv);

        // Uploading MVP Matrix
        TransformBuffer t;
        t.model = m_modelList->at(i)->transform();
        t.view = LookAt(m_camera->m_position, m_camera->m_look, {0.0f, 1.0f, 0.0f});
        t.projection = Projection(70.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
        m_context->UpdateSubresource(m_transformBuffer, 0, NULL, &t, 0, 0);
        m_context->VSSetConstantBuffers(0, 1, &m_transformBuffer);

        // Drawing The Model
        m_context->Draw(m_modelList->at(i)->m_mesh->m_vertexCount, 0);
    }
    m_modelList->clear();

    // ImGui Pre
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // ImGui
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Once);
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %f", 1.0f / Window::s_delta);
    ImGui::End();

    // ImGui Post
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Present
    m_swapChain->Present(0, 0);
}

// Factory Method For Creating Mesh
Mesh *Renderer::createMesh(const char *objFile) {
    return new Mesh(m_device, objFile);
}

Texture *Renderer::createTexture(const char *bmpFile) {
    return new Texture(m_device, bmpFile);
}

// Factory Method For Creating Shader
Shader *Renderer::createShader(const char *vsPath, const char *psPath) {
    return new Shader(m_device, vsPath, psPath);
}

// Factory Method For Creating Model
Model *Renderer::createModel(Mesh *mesh, Texture *texture, Shader *shader) {
    return new Model(m_device, mesh, texture, shader);
}

// Renders Model Next Frame
void Renderer::renderModel(Model *model) {
    m_modelList->push_back(model);
}