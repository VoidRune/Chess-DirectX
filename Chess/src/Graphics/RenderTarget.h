#pragma once
#include "Graphics.h"

class RenderTarget
{
public:
	RenderTarget(int width, int height);
	~RenderTarget();
	void BindAsTexture(int slot);
	void BindAsTarget();
	static void Resize(int width, int height);
private:
	static ID3D11ShaderResourceView* pTextureView;
	static ID3D11RenderTargetView* pTargetView;
};

