#include "PreCompiled.h"
#include "Entity.h"
#include "SystemDefs.h"
#include "VertexBuffer.h"

Entity::Entity()
{
	m_position = m_velocity = XMFLOAT3(0.f, 0.f, 0.f);
	m_scale = XMFLOAT3(1.f, 1.f, 1.f);
	m_fRotation = 0.f;
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());
	m_type = ENTTIY_TYPE_NONE;
	m_shader = nullptr;
	m_vertexBuffer = new VertexBuffer();
}

Entity::~Entity()
{
	SafeDelete(m_vertexBuffer);
}

void Entity::Update()
{
	//reset world matrix
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());

	m_position.x += m_velocity.x * 0.000001f;
	m_position.y += m_velocity.y * 0.000001f;
	//m_position.z += m_velocity.z * 0.01f;
	m_position.z += m_velocity.y * 0.000001f;


//	cout << "X : " << m_position.x << " Y : " << m_position.y << " Z : " << m_position.z << endl;
//	m_velocity.x *= 0.96f;
//	m_velocity.y *= 0.96f;
	//m_velocity.z *= 0.96f;
//	m_velocity.z = 0.f;

	//apply rotation & scale & translation
	XMMATRIX Rotation = XMMatrixRotationY(m_fRotation);
	XMMATRIX Scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX Translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	
	XMMATRIX worldMatrix = Rotation * Scale * Translation;
	XMStoreFloat4x4(&m_worldMatrix, worldMatrix);
}

void Entity::Render(ID3D11DeviceContext * deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	
}

void Entity::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Entity::SetPosition(XMFLOAT3 pos)
{
	m_position = pos;
}

void Entity::SetVelocity(float x, float y, float z)
{
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}
