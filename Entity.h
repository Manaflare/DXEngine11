#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
class Shader;
class Sprite;
class Mesh;

using namespace DirectX;

class Entity
{
public:
	Entity();
	~Entity();

	void Initialize(ID3D11Device * device, Shader * shader, LPCSTR objFileName);
	void InitializeSprite(ID3D11Device* device, Shader* shader, LPCSTR textureFileName, float fSpriteSize);

	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);

	void SetPosition(float x = 0.f, float y = 0.f, float z = 0.f);
	void SetVelocity(float x = 0.f, float y = 0.f, float z = 0.f);

private:
	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT3 m_position;
	XMFLOAT3 m_velocity;

	Sprite* m_sprite;
	Mesh* m_mesh;

};

