#pragma once

// External Includes...
#include <d3d11.h>
#ifdef DEBUG
#include <dxgidebug.h>
#endif

// Link libraries ...
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

// Includes...
#include "Singleton.h"

/*
 * D3DSystem
 *   Singleton class to encompass interactions with the DirectX API
 */
class D3DSystem : public Singleton<D3DSystem> {
public:
    static void BeginScene();
    static void EndScene();

    static ID3D11Device* GetDevice() { return (s_pxThis) ? s_pxThis->m_pxDevice : nullptr; }
    static ID3D11DeviceContext* GetDeviceContext() { return (s_pxThis) ? s_pxThis->m_pxDeviceContext : nullptr; }

protected:
    virtual bool Init() override;
    virtual void Shutdown() override;

private:
    D3DSystem();
    ~D3DSystem() {}

    ID3D11Device* m_pxDevice;
    ID3D11DeviceContext* m_pxDeviceContext;
    ID3D11RenderTargetView* m_pxRenderTargetView;
    ID3D11DepthStencilView* m_pxDepthStencilView;
    IDXGISwapChain* m_pxSwapChain;

    // Maybe not needed...
    ID3D11Texture2D* m_pxDepthStencilBuffer;

    friend Singleton<D3DSystem>;
};
