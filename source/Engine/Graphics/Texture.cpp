#include "Engine/Engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(ID3D11Device *device, const char *data, const int *dimensions) {

    // Texture Description
    D3D11_TEXTURE2D_DESC desc2 = {};
    desc2.Width = dimensions[0];
    desc2.Height = dimensions[1];
    desc2.MipLevels = 1;
    desc2.ArraySize = 1;
    desc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc2.SampleDesc.Count = 1;
    desc2.Usage = D3D11_USAGE_DEFAULT;
    desc2.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // Data Description
    D3D11_SUBRESOURCE_DATA data2 = {};
    data2.pSysMem = data;
    data2.SysMemPitch = dimensions[0] * 4;

    // Creating Texture and SRV
    device->CreateTexture2D(&desc2, &data2, &m_texture);
    device->CreateShaderResourceView(m_texture, NULL, &m_srv);
    
    // Freeing Memory
    free((void *)data);

}

Texture *Texture::create(const char *path) {
    int dimensions[2];
    int channels;
    unsigned char *raw = stbi_load(path, &dimensions[0], &dimensions[1], &channels, 4);
    return new Texture(Engine::renderer->m_device, (const char *)raw, dimensions);
}