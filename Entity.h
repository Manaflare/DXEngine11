#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
class Shader;
class Mesh;

using namespace DirectX;

class Entity
{
public:
	enum E_ENTITY_TYPE
	{
		ENTTIY_TYPE_NONE = 0,
		ENTITY_TYPE_MESH,
		ENTITY_TYPE_SPRITE,
		ENTITY_TYPE_SKY,
	};
public:
	Entity();
	~Entity();

	virtual void Initialize(ID3D11Device * device, Shader * shader) = 0; //for being interface
	virtual void Update();
	virtual void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);

	void SetPosition(float x = 0.f, float y = 0.f, float z = 0.f);
	void SetVelocity(float x = 0.f, float y = 0.f, float z = 0.f);
	void SetRotation(float rotation) { m_fRotation = rotation; }
	void SetScale(XMFLOAT3 scale) { m_scale = scale; }

	E_ENTITY_TYPE GetEntityType() const { return m_type;  }
protected:
	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT3 m_position;
	XMFLOAT3 m_velocity;
	XMFLOAT3 m_scale;
	float m_fRotation;

	E_ENTITY_TYPE m_type;
};

