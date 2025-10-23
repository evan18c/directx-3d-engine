#pragma once
#include <d3d11.h>

// Contains All Texture Data + Loading
class Texture {

    public:
        Texture(ID3D11Device *device, const char *bmpPath);
        static Texture *create(const char *bmpPath);
        ID3D11Texture2D *m_texture;
        ID3D11ShaderResourceView *m_srv;
        
};