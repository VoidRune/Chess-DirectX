#include "Graphics.h"

ID3D11Device* Graphics::device = nullptr;
ID3D11DeviceContext* Graphics::context = nullptr;
IDXGISwapChain* Graphics::swapChain = nullptr;
ID3D11RenderTargetView* Graphics::renderTargetView = nullptr;
ID3D11DepthStencilView* Graphics::depthStencilView = nullptr;
ID3D11RasterizerState* Graphics::rasterizerState = nullptr;
ID3D11BlendState* Graphics::blendState = nullptr;

Graphics::Graphics(HWND hWnd)
{

    DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
    swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
    swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swap_chain_descr.SampleDesc.Count = 1;
    swap_chain_descr.SampleDesc.Quality = 0;
    swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_descr.BufferCount = 1;
    swap_chain_descr.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_descr.OutputWindow = hWnd;
    swap_chain_descr.Windowed = !Window::isFullscreen;

    D3D_FEATURE_LEVEL feature_level;
    UINT flags = NULL;// D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        flags,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &swap_chain_descr,
        &swapChain,
        &device,
        &feature_level,
        &context);
    assert(S_OK == hr && swapChain && device && context);


    ID3D11Texture2D* framebuffer;
    hr = swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&framebuffer);
    assert(SUCCEEDED(hr));

    hr = device->CreateRenderTargetView(
        framebuffer, 0, &renderTargetView);
    assert(SUCCEEDED(hr));
    SafeRelease(&framebuffer);

    //DEPTH STENCIL BUFFER

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    ID3D11DepthStencilState* pDSState;
    device->CreateDepthStencilState(&dsDesc, &pDSState);

    //bind
    context->OMSetDepthStencilState(pDSState, 1);

    ID3D11Texture2D* pDepthStencil = NULL;
    D3D11_TEXTURE2D_DESC descDepth = {};
    //same width/height as swapchain
    descDepth.Width = Window::m_Width;
    descDepth.Height = Window::m_Height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    device->CreateDepthStencilView(pDepthStencil, &descDSV, &depthStencilView);

    SafeRelease(&pDepthStencil);
    context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    //-------------------------------------------
    RECT winRect;
    GetClientRect(hWnd, &winRect);
    D3D11_VIEWPORT viewport = {
      0.0f,
      0.0f,
      (FLOAT)(winRect.right - winRect.left),
      (FLOAT)(winRect.bottom - winRect.top),
      0.0f,
      1.0f };
    context->RSSetViewports(1, &viewport);


    D3D11_RASTERIZER_DESC rasterizerStateDesc;
    rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
    rasterizerStateDesc.FrontCounterClockwise = false;
    rasterizerStateDesc.DepthBias = false;
    rasterizerStateDesc.SlopeScaledDepthBias = 0;
    rasterizerStateDesc.DepthBiasClamp = 0;
    rasterizerStateDesc.DepthClipEnable = true;
    rasterizerStateDesc.ScissorEnable = false;
    rasterizerStateDesc.MultisampleEnable = true;
    rasterizerStateDesc.AntialiasedLineEnable = true;
    device->CreateRasterizerState(&rasterizerStateDesc, &rasterizerState);
    context->RSSetState(rasterizerState);

    D3D11_BLEND_DESC BlendStateDesc;
    ZeroMemory(&BlendStateDesc, sizeof(D3D11_BLEND_DESC));
    BlendStateDesc.RenderTarget[0].BlendEnable = TRUE;
    BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    device->CreateBlendState(&BlendStateDesc, &blendState);

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    UINT sampleMask = 0xffffffff;
    context->OMSetBlendState(blendState, blendFactor, sampleMask);

    InitPipeline();
}

Graphics::~Graphics()
{
    SafeRelease(&device);
    SafeRelease(&context);
    SafeRelease(&swapChain);
    SafeRelease(&renderTargetView);
    SafeRelease(&depthStencilView);
}

void Graphics::SwapBuffer(bool vSync)
{
    swapChain->Present(vSync, 0);
}
void Graphics::ClearBuffer(float* color)
{
    context->ClearRenderTargetView(renderTargetView, color);
    context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Graphics::InitPipeline()
{

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif

    ID3DBlob* vs_blob_ptr = NULL, * error_blob = NULL;

    // COMPILE VERTEX SHADER
    HRESULT hr = D3DCompileFromFile(
        L"res/Shaders/shaders.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "vs_main",
        "vs_5_0",
        flags,
        0,
        &vs_blob_ptr,
        &error_blob);
    if (FAILED(hr)) {
        if (error_blob) {
            OutputDebugStringA((char*)error_blob->GetBufferPointer());
            error_blob->Release();
        }
        if (vs_blob_ptr) { vs_blob_ptr->Release(); }
        assert(false);
    }

    ID3D11InputLayout* input_layout_ptr = NULL;
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
      { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      //{ "Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }     
      //{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TextureCoord", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

    };
    hr = device->CreateInputLayout(
        inputElementDesc,
        ARRAYSIZE(inputElementDesc),
        vs_blob_ptr->GetBufferPointer(),
        vs_blob_ptr->GetBufferSize(),
        &input_layout_ptr);
    assert(SUCCEEDED(hr));
    context->IASetInputLayout(input_layout_ptr);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SafeRelease(&vs_blob_ptr);
    SafeRelease(&error_blob);
    SafeRelease(&input_layout_ptr);

}

void Graphics::Resize(int width, int height)
{
    if (swapChain)
    {
        context->OMSetRenderTargets(0, 0, 0);

        // Release all outstanding references to the swap chain's buffers.
        SafeRelease(&renderTargetView);

        HRESULT hr;
        // Preserve the existing buffer count and format.
        // Automatically choose the width and height to match the client rect for HWNDs.
        hr = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        assert(SUCCEEDED(hr));


        // Get buffer and create a render-target-view.
        ID3D11Texture2D* pBuffer;
        hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
            (void**)&pBuffer);
        assert(SUCCEEDED(hr));

        hr = device->CreateRenderTargetView(pBuffer, NULL,
            &renderTargetView);
        assert(SUCCEEDED(hr));

        SafeRelease(&pBuffer);

        ID3D11Texture2D* pDepthStencil = NULL;
        D3D11_TEXTURE2D_DESC descDepth = {};
        //same width/height as swapchain
        descDepth.Width = width;
        descDepth.Height = height;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D32_FLOAT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
        descDSV.Format = DXGI_FORMAT_D32_FLOAT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        device->CreateDepthStencilView(pDepthStencil, &descDSV, &depthStencilView);

        context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
        SafeRelease(&pDepthStencil);
        // Set up the viewport.
        D3D11_VIEWPORT vp;
        vp.Width = width;
        vp.Height = height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        context->RSSetViewports(1, &vp);
    }
}

void Graphics::SetRenderTarget()
{
    context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
}