#include "Graphics.h"



Graphics::Graphics()
{
	m_dxManager = nullptr;
}


Graphics::~Graphics()
{
	SafeDelete(m_dxManager);
}

bool Graphics::InitializeDX(HWND hwnd)
{
	m_dxManager = new DXManager();
	if (!m_dxManager->Initialize((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, VSYNC_ENABLE, hwnd, FULL_SCREEN))
	{
		return false;
	}
	return true;
}

void Graphics::Initialize()
{
}

void Graphics::BeginScene(float r, float g, float b, float a)
{
	m_dxManager->BeginScene(r, g, b, a);
}

void Graphics::EndScene()
{
	m_dxManager->EndScene();
}

void Graphics::EnableAlphaBlending(bool enable)
{
	m_dxManager->EnableAlphaBlending(enable);
}

void Graphics::EnableZBuffer(bool enable)
{
	m_dxManager->EnableZBuffer(enable);
}

DXManager * Graphics::GetDXManager()
{
	return m_dxManager;
}

HWND Graphics::GetHwnd()
{
	return m_hwnd;
}

ID3D11Device * Graphics::GetDevice()
{
	if (m_dxManager)
		return m_dxManager->GetDevice();
	
	return nullptr;
}

ID3D11DeviceContext * Graphics::GetDeviceContext()
{
	if (m_dxManager)
		return m_dxManager->GetDeviceContext();

	return nullptr;
}
