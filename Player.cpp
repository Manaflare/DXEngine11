#include "Player.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Input.h"

Player::Player()
{
	m_entity = nullptr;
}


Player::~Player()
{
}

void Player::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, Shader * shader)
{
	m_entity = EntMgr->AddEntity();
	m_entity->Initialize(device, shader, "Model/ground.obj");
	//m_entity->InitializeSprite(device, shader, "test.png", 500.f);
}

void Player::Update()
{
	UpdateInput();
}

void Player::UpdateInput()
{
	m_entity->SetVelocity(0, 0);

	Input* input = Engine::GetEngine()->GetInput();
	if (input == nullptr)
		return;

	//for free cam
	

}
