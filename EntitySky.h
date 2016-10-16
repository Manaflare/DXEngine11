#pragma once
#include "Entity.h"
class EntitySky :
	public Entity
{
public:
	EntitySky();
	virtual ~EntitySky();


	void Initialize(ID3D11Device * device, Shader * shader);
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);

private:
	void CreateSphere(ID3D11Device * device, Shader * shader,int LatLines, int LongLines);

	int m_numSphereFaces;
};

