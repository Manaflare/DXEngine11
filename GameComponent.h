#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class GameComponent
{
public :
	GameComponent() {}
	virtual ~GameComponent() {}

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix) = 0;
};