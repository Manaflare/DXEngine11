#include "Graphics.h"



Graphics::Graphics()
{
	m_dxManager = nullptr;
}


Graphics::~Graphics()
{
	SafeDelete(m_dxManager);
}
bool Graphics::Initialize()
{
	// Setup the projection matrix.
	// Create the projection matrix for 3D rendering.
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_projMatrix, proj);

	// Initialize the world matrix to the identity matrix.
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());

	// Create an orthographic projection matrix for 2D rendering.
	XMStoreFloat4x4(&m_orthoMatrix, XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 1000.f));

	return true;
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
