// Includes...
#include "D3DSystem.h"
#include "Core/Assert.h"

// Externs...
extern HWND gHWND;
extern const bool gbFULLSCREEN;
extern u_int guSCREENWIDTH;
extern u_int guSCREENHEIGHT;

// Global Variables...
const bool gbVSYNC = false;

// Static Variables...
D3DSystem* Singleton<D3DSystem>::s_pxThis = nullptr;

D3DSystem::D3DSystem()
: m_pxDevice(nullptr)
, m_pxDeviceContext(nullptr)
, m_pxRenderTargetView(nullptr)
, m_pxDepthStencilView(nullptr)
, m_pxSwapChain(nullptr)
, m_pxDepthStencilBuffer(nullptr)
{
}

/*
 * BeginScene : Clear the render targets to start a fresh frame
 */
void
D3DSystem::BeginScene()
{
    if (!s_pxThis || !s_pxThis->m_pxDeviceContext) { return; }

    float color[4];
    // Clear the buffer to black
    color[0] = 0.f;
    color[1] = 0.f;
    color[2] = 0.f;
    color[3] = 1.f;

    // Clear the back buffer.
    s_pxThis->m_pxDeviceContext->ClearRenderTargetView(s_pxThis->m_pxRenderTargetView, color);

    // Clear the depth buffer.
    s_pxThis->m_pxDeviceContext->ClearDepthStencilView(s_pxThis->m_pxDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/*
 * EndScene : Present the render target to the screen
 */
void
D3DSystem::EndScene()
{
    if (!s_pxThis) { return; }
    // Present the back buffer to the screen since rendering is complete.
    if (gbVSYNC) {
        // Lock to screen refresh rate.
        s_pxThis->m_pxSwapChain->Present(1, 0);
    } else {
        // Present as fast as possible.
        s_pxThis->m_pxSwapChain->Present(0, 0);
    }

    return;
}

/*
 * Init : Initialize the instance of the graphics system
 */
bool
D3DSystem::Init()
{
    u_int uNumModes;
    u_int uRefreshRateNumerator;
    u_int uRefreshRateDenominator;
    u_int uMultiSamplingCount = 1;
    u_int uMultiSamplingQuality = 0;
    u_int uScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    u_int uScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    HRESULT hResult;
    D3D_FEATURE_LEVEL xFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    ID3D11Texture2D* pxBackBuffer = nullptr;
    IDXGIFactory* pxFactory = nullptr;
    IDXGIAdapter* pxAdapter;
    IDXGIOutput* pxAdapterOutput;
    DXGI_MODE_DESC* pxDisplayModeList;

    DXGI_SWAP_CHAIN_DESC xSwapChainDesc;
    ZeroMemory(&xSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    D3D11_DEPTH_STENCIL_VIEW_DESC xDepthStencilViewDesc;
    ZeroMemory(&xDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

    D3D11_TEXTURE2D_DESC xDepthBufferDesc;
    ZeroMemory(&xDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

    D3D11_VIEWPORT xViewport;
    ZeroMemory(&xViewport, sizeof(D3D11_VIEWPORT));

    // Create the D3D11 device and device context
    hResult = D3D11CreateDevice(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_DEBUG,
        &xFeatureLevel,
        1,
        D3D11_SDK_VERSION,
        &m_pxDevice,
        NULL,
        &m_pxDeviceContext);
    ASSERT(!FAILED(hResult), "Failed to create the D3D11 device.");
    if (FAILED(hResult)) { return false; }

    // Create the DirectX graphics interface factory object
    hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pxFactory);
    ASSERT(!FAILED(hResult), "Failed to create the DirectX graphics interface factory.");
    if (FAILED(hResult)) { return false; }

    // Get an adapter for the video card from the factory
    hResult = pxFactory->EnumAdapters(0, &pxAdapter);
    ASSERT(!FAILED(hResult), "Failed to get the video card adapter from the factory.");
    if (FAILED(hResult)) { return false; }

    // Enumerate the primary adapter output
    hResult = pxAdapter->EnumOutputs(0, &pxAdapterOutput);
    ASSERT(!FAILED(hResult), "Failed to enumerate the primary adapter output.");
    if (FAILED(hResult)) { return false; }

    // Get the list of display modes for the primary adapter output
    hResult = pxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_ENUM_MODES_INTERLACED,
        &uNumModes,
        NULL);
    ASSERT(!FAILED(hResult), "Failed to get the display mode list for the primary adapter output.");
    if (FAILED(hResult)) { return false; }
    pxDisplayModeList = new DXGI_MODE_DESC[uNumModes];
    ASSERT(pxDisplayModeList, "");
    if (!pxDisplayModeList) { return false; }
    hResult = pxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_ENUM_MODES_INTERLACED,
        &uNumModes,
        pxDisplayModeList);
    ASSERT(!FAILED(hResult), "Failed to get the display mode list for the primary adapter output.");
    if (FAILED(hResult)) { return false; }

    // Find the Numerator and Denominator of the display mode matching the screen width & height
    for (u_int u = 0; u < uNumModes; u++) {
        if (pxDisplayModeList[u].Width == uScreenWidth
            && pxDisplayModeList[u].Height == uScreenHeight) {
            uRefreshRateNumerator = pxDisplayModeList[u].RefreshRate.Numerator;
            uRefreshRateDenominator = pxDisplayModeList[u].RefreshRate.Denominator;
        }
    }

    // Initialize the swap chain description
    {
        xSwapChainDesc.BufferCount = 1;
        xSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        xSwapChainDesc.OutputWindow = gHWND;
        xSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        xSwapChainDesc.Flags = 0;
        xSwapChainDesc.BufferDesc.Width = uScreenWidth;
        xSwapChainDesc.BufferDesc.Height = uScreenHeight;
        xSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        xSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        xSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        if (gbVSYNC) {
            xSwapChainDesc.BufferDesc.RefreshRate.Numerator = uRefreshRateNumerator;
            xSwapChainDesc.BufferDesc.RefreshRate.Denominator = uRefreshRateDenominator;
        } else {
            xSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
            xSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        }
        xSwapChainDesc.Windowed = !gbFULLSCREEN;
        // Turn multisampling on if available.
        for (int i = 1; i <= 16; i *= 2) {
            u_int uQuality = -1;
            m_pxDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &uQuality);
            if (uQuality > 0) {
                uMultiSamplingCount = i;
                uMultiSamplingQuality = uQuality - 1;
            }
        }
        xSwapChainDesc.SampleDesc.Count = uMultiSamplingCount;
        xSwapChainDesc.SampleDesc.Quality = uMultiSamplingQuality;
    }

    // Create the swap chain
    hResult = pxFactory->CreateSwapChain(m_pxDevice, &xSwapChainDesc, &m_pxSwapChain);
    ASSERT(!FAILED(hResult), "Failed to create the D3D11 swap chain.");
    if (FAILED(hResult)) { return false; }

    // Get the pointer to the back buffer.
    hResult = m_pxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pxBackBuffer);
    ASSERT(!FAILED(hResult), "Failed to create the D3D11 back buffer.");
    if (FAILED(hResult)) { return false; }

    // Create the render target view with the back buffer pointer
    hResult = m_pxDevice->CreateRenderTargetView(pxBackBuffer, NULL, &m_pxRenderTargetView);
    ASSERT(!FAILED(hResult), "Failed to create the D3D11 render target view.");
    if (FAILED(hResult)) { return false; }

    // Setup the depth buffer description
    {
        xDepthBufferDesc.Width = uScreenWidth;
        xDepthBufferDesc.Height = uScreenHeight;
        xDepthBufferDesc.MipLevels = 1;
        xDepthBufferDesc.ArraySize = 1;
        xDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        xDepthBufferDesc.SampleDesc.Count = uMultiSamplingCount;
        xDepthBufferDesc.SampleDesc.Quality = uMultiSamplingQuality;
        xDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        xDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    }

    // Create the texture for the depth buffer
    hResult = m_pxDevice->CreateTexture2D(&xDepthBufferDesc, NULL, &m_pxDepthStencilBuffer);
    ASSERT(!FAILED(hResult), "Failed to create the D3D11 depth stencil buffer.");
    if (FAILED(hResult)) { return false; }

    // Initialize the depth stencil view description
    {
        xDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        xDepthStencilViewDesc.ViewDimension = (uMultiSamplingCount == 1) ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DMS;
        xDepthStencilViewDesc.Texture2D.MipSlice = 0;
    }

    // Create the depth stencil view
    hResult = m_pxDevice->CreateDepthStencilView(m_pxDepthStencilBuffer, &xDepthStencilViewDesc, &m_pxDepthStencilView);
    ASSERT(!FAILED(hResult), "Failed to create the D3D11 depth stencil view.");
    if (FAILED(hResult)) { return false; }

    // Bind the render target view and depth stencil view
    m_pxDeviceContext->OMSetRenderTargets(1, &m_pxRenderTargetView, m_pxDepthStencilView);

    // Setup the viewport for rendering
    xViewport.Width = (float)uScreenWidth;
    xViewport.Height = (float)uScreenHeight;
    xViewport.MinDepth = 0.f;
    xViewport.MaxDepth = 1.f;
    xViewport.TopLeftX = 0.f;
    xViewport.TopLeftY = 0.f;
    m_pxDeviceContext->RSSetViewports(1, &xViewport);

    // Release the back buffer
    pxBackBuffer->Release();
    pxBackBuffer = nullptr;

    return true;
}

/*
 * Shutdown : Shutdown the instance of the graphics system
 */
void
D3DSystem::Shutdown()
{
    if (m_pxSwapChain) {
        m_pxSwapChain->SetFullscreenState(false, NULL);
    }
    if (m_pxDepthStencilView) {
        m_pxDepthStencilView->Release();
        m_pxDepthStencilView = nullptr;
    }
    if (m_pxDepthStencilBuffer) {
        m_pxDepthStencilBuffer->Release();
        m_pxDepthStencilBuffer = nullptr;
    }
    if (m_pxRenderTargetView) {
        m_pxRenderTargetView->Release();
        m_pxRenderTargetView = nullptr;
    }
    if (m_pxDeviceContext) {
        m_pxDeviceContext->Release();
        m_pxDeviceContext = nullptr;
    }
    if (m_pxDevice) {
        m_pxDevice->Release();
        m_pxDevice = nullptr;
    }
    if (m_pxSwapChain) {
        m_pxSwapChain->Release();
        m_pxSwapChain = nullptr;
    }
}
