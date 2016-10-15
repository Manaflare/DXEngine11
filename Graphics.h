#pragma once

#include "DXManager.h"
#include "SystemDefs.h"
#include <DirectXMath.h>

using namespace DirectX;

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool InitializeDX(HWND hwnd);
	bool Initialize();
	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

	DXManager* GetDXManager();
	HWND GetHwnd();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void SetHwnd(HWND hwnd) { m_hwnd = hwnd;  }

	void GetWorldMatrix(XMFLOAT4X4& pOut) { pOut = m_worldMatrix; }
	void GetProjMatrix(XMFLOAT4X4& pOut) { pOut = m_projMatrix; }
	void GetOrthoMatrix(XMFLOAT4X4& pOut) { pOut = m_orthoMatrix; }

private:
	DXManager* m_dxManager;
	HWND m_hwnd;

	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT4X4 m_projMatrix;
	XMFLOAT4X4 m_orthoMatrix;

};

