#include "Engine/Engine.h"

Texture::Texture(ID3D11Device *device, const char *bmpPath) {

    // Opening Texture
    int dimensions[2];
    const char *raw = (char *)Files::ReadBMP(bmpPath, dimensions);
    int width = dimensions[0];
    int height = dimensions[1];

    // Texture Description
    D3D11_TEXTURE2D_DESC desc2 = {};
    desc2.Width = width;
    desc2.Height = height;
    desc2.MipLevels = 1;
    desc2.ArraySize = 1;
    desc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc2.SampleDesc.Count = 1;
    desc2.Usage = D3D11_USAGE_DEFAULT;
    desc2.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // Data Description
    D3D11_SUBRESOURCE_DATA data2 = {};
    data2.pSysMem = raw;
    data2.SysMemPitch = width * 4;

    // Creating Texture and SRV
    device->CreateTexture2D(&desc2, &data2, &m_texture);
    device->CreateShaderResourceView(m_texture, NULL, &m_srv);
    
    // Freeing Memory
    free((void *)raw);

}

Texture *Texture::create(const char *bmpPath) {
    return new Texture(Engine::renderer->m_device, bmpPath);
}