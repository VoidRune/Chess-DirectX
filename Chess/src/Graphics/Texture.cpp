#include "Texture.h"
#include "stb_image.h"
#include <iostream>
#include <string>

Texture::Texture(const char* name, bool linearFilter, bool flipTex) :
	name(name)
{
    int nrChannels;
    stbi_set_flip_vertically_on_load(flipTex);
    data = stbi_load(name, &m_Width, &m_Height, &nrChannels, 0);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_Width;
    desc.Height = m_Height;
    desc.MipLevels = 1;         //0 = all mip levels
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = data;
    sd.SysMemPitch = m_Width * 4 * sizeof(unsigned char); // *4 because there are 4 values-> RGBA all 8bit
    sd.SysMemSlicePitch = m_Width * m_Height * 4 * sizeof(unsigned char);

    pTexture = {};
    HRESULT hr = Graphics::GetDevice()->CreateTexture2D(&desc, &sd, &pTexture);
    assert(SUCCEEDED(hr));

    stbi_image_free(data);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;           //-1 = all the mipmap levels

    hr = Graphics::GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);
    assert(SUCCEEDED(hr));

    D3D11_SAMPLER_DESC SamplerDesc = {};
    SamplerDesc.Filter = linearFilter ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.MipLODBias = 0.0f;
    SamplerDesc.MinLOD = 0.0f;
    SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


    hr = Graphics::GetDevice()->CreateSamplerState(&SamplerDesc, &pSampler);
    assert(SUCCEEDED(hr));
}

Texture::~Texture()
{
    SafeRelease(&pTexture);
    SafeRelease(&pTextureView);
    SafeRelease(&pSampler);
}

void Texture::Bind(unsigned int slot)
{
    Graphics::GetDeviceContext()->PSSetShaderResources(slot, 1, &pTextureView);
    Graphics::GetDeviceContext()->PSSetSamplers(slot, 1, &pSampler);
}

TextureArray::TextureArray(const char* name[], const int arraySize, bool linearFilter, bool flipTex) :
    name(*name)
{
    int nrChannels;
    stbi_set_flip_vertically_on_load(flipTex);

    D3D11_SUBRESOURCE_DATA* sd = new D3D11_SUBRESOURCE_DATA[arraySize];
    for (size_t i = 0; i < arraySize; i++)
    {
        std::string path = name[i];

        data = stbi_load(path.c_str(), &m_Width, &m_Height, &nrChannels, 0);
        sd[i].pSysMem = data;
        sd[i].SysMemPitch = m_Width * 4 * sizeof(unsigned char);
        sd[i].SysMemSlicePitch = m_Width * m_Height * 4 * sizeof(unsigned char);
    }

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_Width;
    desc.Height = m_Height;
    desc.MipLevels = 1;         //0 = all mip levels
    desc.ArraySize = arraySize;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    pTexture = {};
    HRESULT hr = Graphics::GetDevice()->CreateTexture2D(&desc, sd, &pTexture);
    assert(SUCCEEDED(hr));

    stbi_image_free(data);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;           //-1 = all the mipmap levels
    srvDesc.Texture2DArray.ArraySize = arraySize;
    srvDesc.Texture2DArray.MipLevels = 1;

    hr = Graphics::GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);
    assert(SUCCEEDED(hr));

    D3D11_SAMPLER_DESC SamplerDesc = {};
    SamplerDesc.Filter = linearFilter ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamplerDesc.MipLODBias = 0.0f;
    SamplerDesc.MinLOD = 0.0f;
    SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


    hr = Graphics::GetDevice()->CreateSamplerState(&SamplerDesc, &pSampler);
    assert(SUCCEEDED(hr));
}

TextureArray::~TextureArray()
{
    SafeRelease(&pTexture);
    SafeRelease(&pTextureView);
    SafeRelease(&pSampler);
}

void TextureArray::Bind(unsigned int slot)
{
    Graphics::GetDeviceContext()->PSSetShaderResources(slot, 1, &pTextureView);
    Graphics::GetDeviceContext()->PSSetSamplers(slot, 1, &pSampler);
}