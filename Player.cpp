#include "PreCompiled.h"
#include "Player.h"
#include "EntityManager.h"
#include "Entity.h"
#include "EntityMesh.h"
#include "Input.h"
#include "Shader.h"

Player::Player()
{
	m_entity[0] = nullptr;
	m_entity[1] = nullptr;
}


Player::~Player()
{
}

void Player::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, Shader * shader)
{
	m_entity[0] = EntMgr->CreateEntity(Entity::ENTITY_TYPE_MESH);
	EntityMesh* entityMesh = dynamic_cast<EntityMesh*>(m_entity[0]);
	entityMesh->SetObjFileName("Model/ground.obj");
	m_entity[0]->Initialize(device, shader);
	//m_entity->InitializeSprite(device, shader, "test.png", 500.f);

#ifndef _DEBUG
	m_entity[1] = EntMgr->CreateEntity(Entity::ENTITY_TYPE_MESH);
	EntityMesh* entityMesh2 = dynamic_cast<EntityMesh*>(m_entity[1]);
	entityMesh2->SetObjFileName("Model/Sylvanas.obj");
	m_entity[1]->Initialize(device, shader);
	//m_entity->InitializeSprite(device, shader, "test.png", 500.f);
#endif
}

void Player::Update()
{
	UpdateInput();
}

void Player::UpdateInput()
{
	m_entity[0]->SetVelocity(0, 0);

	Input* input = Engine::GetEngine()->GetInput();
	if (input == nullptr)
		return;

	//for free cam
	

}
