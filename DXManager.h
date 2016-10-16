#pragma once

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

class DXManager
{
public:
	DXManager();
	~DXManager();

	bool Initialize(int screenWidth, int screenHeight, bool vshync, HWND hwnd, bool fullscreen);
	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	void EnableAlphaBlending(bool bEnable);
	void EnableZBuffer(bool bEnable);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	bool InitializeSwapChain(HWND hwnd, bool fullscreen, int screenWidth, int screenHeight, unsigned int numerator, unsigned int denominator);
	bool InitlalizeDepthBuffer(int screenWidth, int screenHeight);
	bool InitializeDepthStencilBuffer();
	bool InitializeStencilView();
	bool InitializeRasterizeState();
	bool InitializeViewport(int screenWidth, int screenHeight);
	bool InitializeAlphaBlending();
	bool InitializeZBuffer();

	bool m_vsync;
	int m_videocardMemory;
	char m_videocareDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTagetview;
	ID3D11Texture2D* m_depthstencilBuffer;
	ID3D11DepthStencilState* m_depthstencilState;
	ID3D11DepthStencilView* m_depthstencilView;
	ID3D11RasterizerState* m_rasterizerState;
	ID3D11BlendState* m_alphaEnabledBlendingState;
	ID3D11BlendState* m_alphaDisabledBlendingState;
	ID3D11DepthStencilState* m_depthDisabledstencilState;

};

