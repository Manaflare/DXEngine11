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
	void SwitchFireFrame();

	DXManager* GetDXManager();
	HWND GetHwnd();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void SetHwnd(HWND hwnd) { m_hwnd = hwnd;  }

private:
	DXManager* m_dxManager;
	HWND m_hwnd;
};

