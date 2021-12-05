#pragma once
#include "Graphics.h"

class Texture
{
public:
	Texture(const char* name, bool linearFilter = false, bool flipTex = true);
	~Texture();
	void Bind(unsigned int slot);
private:
	const char* name;
	unsigned char* data;
	int m_Width = 64;
	int m_Height = 64;

	ID3D11Texture2D* pTexture;
	ID3D11SamplerState* pSampler;
	ID3D11ShaderResourceView* pTextureView;

};

class TextureArray
{
public:
	TextureArray(const char* name[], const int arraySize = 1, bool linearFilter = false, bool flipTex = true);
	~TextureArray();
	void Bind(unsigned int slot);
private:
	const char* name;
	unsigned char* data;
	int m_Width = 64;
	int m_Height = 64;

	ID3D11Texture2D* pTexture;
	ID3D11SamplerState* pSampler;
	ID3D11ShaderResourceView* pTextureView;

};
