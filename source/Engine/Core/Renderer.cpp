#include "Engine/Engine.h"
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

    // -------------------- 5. Create Depth Stencil State -------------------- //
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS; // standard depth test
    m_device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);
    m_context->OMSetDepthStencilState(m_depthStencilState, 0);

    // -------------------- 6. Create Blend State -------------------- //
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_device->CreateBlendState(&blendDesc, &m_blendState);
    m_context->OMSetBlendState(m_blendState, NULL, 0xffffffff);

    // -------------------- 7. Create SceneBuffer -------------------- //
    D3D11_BUFFER_DESC transformBufferDesc = {};
    transformBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    transformBufferDesc.ByteWidth = sizeof(SceneBuffer);
    transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transformBufferDesc.CPUAccessFlags = 0;
    m_device->CreateBuffer(&transformBufferDesc, NULL, &m_sceneBuffer);

    // -------------------- 8. Create Transform Buffer 2D -------------------- //
    D3D11_BUFFER_DESC transformBufferDesc2 = {};
    transformBufferDesc2.Usage = D3D11_USAGE_DEFAULT;
    transformBufferDesc2.ByteWidth = sizeof(TransformBuffer2D);
    transformBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transformBufferDesc2.CPUAccessFlags = 0;
    m_device->CreateBuffer(&transformBufferDesc2, NULL, &m_transformBuffer2D);
    
    // -------------------- 9. Create Sampler State -------------------- //
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_device->CreateSamplerState(&sampDesc, &m_samplerState);

    // -------------------- 10. Create Raster State -------------------- //
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.FrontCounterClockwise = FALSE;
    rasterDesc.DepthClipEnable = TRUE;
    m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    m_context->RSSetState(m_rasterState);


    // -------------------- 11. ImGui Init -------------------- //
    ImGui_ImplDX11_Init(m_device, m_context);

}

// Called at the start of every frame.
void Renderer::beginFrame() {

    // Clear Window
    float clearColor[4] = { 0.0f, 0.25f, 0.5f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView, clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // ImGui Pre
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

}

// Called at the end of every frame.
void Renderer::endFrame() {

    // ImGui Post
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Present
    m_swapChain->Present(1, 0);

}

// Renders Model Next Frame
void Renderer::renderModel(Model *model) {

    // Constants
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0;

    // Init
    m_context->OMSetDepthStencilState(m_depthStencilState, 0);
    m_context->PSSetSamplers(0, 1, &m_samplerState);
    m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set Model Shaders
    m_context->IASetInputLayout(model->m_shader->m_il);
    m_context->VSSetShader(model->m_shader->m_vs, NULL, 0);
    m_context->PSSetShader(model->m_shader->m_ps, NULL, 0);

    // Creating Scene Buffer
    SceneBuffer sb;

    // Updating MVP Matrix
    sb.model = model->transform();
    sb.view = LookAt(Engine::camera->m_position, Engine::camera->m_look, {0.0f, 1.0f, 0.0f});
    sb.projection = Projection(70.0f, (float)Engine::window->m_width / (float)Engine::window->m_height, 0.1f, 1000.0f);

    // Lighting
    sb.lightCount = 0;
    for (Object *obj : model->m_scene->m_objects) {
        if (obj->m_type == ObjectType::LIGHT) {
            Light *light = static_cast<Light *>(obj);
            sb.lights[sb.lightCount].position.x = light->m_position.x;
            sb.lights[sb.lightCount].position.y = light->m_position.y;
            sb.lights[sb.lightCount].position.z = light->m_position.z;
            sb.lights[sb.lightCount].color.x = light->m_color.x;
            sb.lights[sb.lightCount].color.y = light->m_color.y;
            sb.lights[sb.lightCount].color.z = light->m_color.z;
            sb.lights[sb.lightCount].params.x = light->m_intensity;
            sb.lights[sb.lightCount].params.y = light->m_radius;
            sb.lightCount++;
        }
    }

    // Updating + Uploading SceneBuffer
    m_context->UpdateSubresource(m_sceneBuffer, 0, NULL, &sb, 0, 0);
    m_context->VSSetConstantBuffers(0, 1, &m_sceneBuffer);
    m_context->PSSetConstantBuffers(0, 1, &m_sceneBuffer);

    // Render MeshParts
    for (MeshPart mp : model->m_mesh->m_parts) {
        m_context->IASetVertexBuffers(0, 1, &mp.vertexBuffer, &stride, &offset);
        m_context->PSSetShaderResources(0, 1, &mp.srv);
        m_context->Draw(mp.vertexCount, 0);
    }
}

// Renders Sprite Next Frame
void Renderer::renderSprite(Sprite *sprite) {

    // Constants
    const UINT stride2 = sizeof(Vertex2D);
    const UINT offset2 = 0;

    // Init
    m_context->OMSetDepthStencilState(NULL, 0);
    m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // Set Sprite Shaders
    m_context->IASetInputLayout(sprite->m_shader->m_il);
    m_context->VSSetShader(sprite->m_shader->m_vs, NULL, 0);
    m_context->PSSetShader(sprite->m_shader->m_ps, NULL, 0);

    // Uploading Data
    m_context->IASetVertexBuffers(0, 1, &sprite->m_buffer, &stride2, &offset2);
    m_context->PSSetShaderResources(0, 1, &sprite->m_texture->m_srv);

    // Uploading Transformation Matrix
    TransformBuffer2D t;
    t.transform = sprite->transform();
    m_context->UpdateSubresource(m_transformBuffer2D, 0, NULL, &t, 0, 0);
    m_context->VSSetConstantBuffers(0, 1, &m_transformBuffer2D);

    // Draw The 2 Triangles
    m_context->Draw(4, 0);
}