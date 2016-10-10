#include "Entity.h"
#include "SystemDefs.h"
#include "Sprite.h"
#include "Mesh.h"
Entity::Entity()
{
	m_position = XMFLOAT3(0.f, 0.f, 0.f);
	m_velocity = XMFLOAT3(0.f, 0.f, 0.f);

	XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());
	
	m_sprite = nullptr;
	m_mesh = nullptr;
}


Entity::~Entity()
{
	SafeDelete(m_sprite);
	SafeDelete(m_mesh);
}

void Entity::Initialize(ID3D11Device * device, Shader * shader, LPCSTR objFileName)
{
	m_mesh = new Mesh();
	m_mesh->Initialize(device, shader, objFileName);
}

void Entity::InitializeSprite(ID3D11Device * device, Shader * shader, LPCSTR textureFileName, float fSpriteSize)
{
	m_sprite = new Sprite(fSpriteSize);
	m_sprite->Initialize(device, shader, textureFileName);
}

void Entity::Update()
{
	m_position.x += m_velocity.x * 0.000001f;
	m_position.y += m_velocity.y * 0.000001f;
	//m_position.z += m_velocity.z * 0.01f;
	m_position.z = 0.f;

	m_velocity.x *= 0.96f;
	m_velocity.y *= 0.96f;
	//m_velocity.z *= 0.96f;
	m_velocity.z = 0.f;

	
	XMStoreFloat4x4(&m_worldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&m_worldMatrix), XMMatrixTranslation(m_position.x, m_position.y, m_position.z)));

	//Model update
	if (m_mesh)
		m_mesh->Update();

	if (m_sprite)
		m_sprite->Update();
}

void Entity::Render(ID3D11DeviceContext * deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	//Model Render
	if (m_mesh)
		m_mesh->Render(deviceContext, m_worldMatrix, viewMatrix, projMatrix);

	if (m_sprite)
		m_sprite->Render(deviceContext,m_worldMatrix, viewMatrix, projMatrix);
}

void Entity::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Entity::SetVelocity(float x, float y, float z)
{
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}
