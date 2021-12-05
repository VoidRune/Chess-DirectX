#include "RenderTarget.h"

ID3D11ShaderResourceView* RenderTarget::pTextureView = nullptr;
ID3D11RenderTargetView* RenderTarget:: pTargetView = nullptr;

RenderTarget::RenderTarget(int width, int height)
{
	ID3D11Texture2D* pTexture;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	Graphics::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture);

	//Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	Graphics::GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);

	//Target View
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	Graphics::GetDevice()->CreateRenderTargetView(pTexture, &rtvDesc, &pTargetView);
	
	SafeRelease(&pTexture);
}

RenderTarget::~RenderTarget()
{
	SafeRelease(&pTargetView);
	SafeRelease(&pTextureView);
}

void RenderTarget::BindAsTexture(int slot)
{
	Graphics::GetDeviceContext()->PSSetShaderResources(slot, 1, &pTextureView);
}

void RenderTarget::BindAsTarget()
{
	float color[4] = { 0.2f, 0.8f, 0.8f, 1.0f };
	Graphics::GetDeviceContext()->ClearRenderTargetView(pTargetView, color);
	Graphics::GetDeviceContext()->OMSetRenderTargets(1, &pTargetView, Graphics::depthStencilView);
}

void RenderTarget::Resize(int width, int height)
{
	if (Graphics::GetDevice())
	{
		SafeRelease(&pTargetView);
		SafeRelease(&pTextureView);

		ID3D11Texture2D* pTexture;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		Graphics::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture);

		//Resource View
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		Graphics::GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);

		//Target View
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
		Graphics::GetDevice()->CreateRenderTargetView(pTexture, &rtvDesc, &pTargetView);

		SafeRelease(&pTexture);
	}
}