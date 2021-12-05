#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Window/Window.h"
#include "Utility/Utility.h"

#include <vector>
#include <assert.h>

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dcompiler.lib" ) // shader compiler

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();
	static void SwapBuffer(bool vSync = true);
	static void ClearBuffer(float* color = new float[4]{ 0.0f, 0.5f, 0.8f, 1.0f });
	static ID3D11Device* GetDevice() { return device; }
	static ID3D11DeviceContext* GetDeviceContext() { return context; }
	static IDXGISwapChain* GetSwapChain() { return swapChain; }
	static ID3D11RenderTargetView* GetRenderTargetView() { return renderTargetView; }
	static ID3D11DepthStencilView* GetDepthStencilView() { return depthStencilView; }
	static void Resize(int width, int height);
	static void SetRenderTarget();
//private:
	static ID3D11Device* device;
	static ID3D11DeviceContext* context;
	static IDXGISwapChain* swapChain;
	static ID3D11RenderTargetView* renderTargetView;
	static ID3D11DepthStencilView* depthStencilView;
	static ID3D11RasterizerState* rasterizerState;
	static ID3D11BlendState* blendState;

	void InitPipeline();
};