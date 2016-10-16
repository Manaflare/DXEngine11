#include "GameScene.h"
#include "Player.h"
#include "TextureShader.h "
#include "ResourceManager.h"
#include "MeshShader.h"
#include "EntityManager.h"
#include "Entity.h"
#include "EntitySky.h"

GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

bool GameScene::Initialize()
{
/*	TextureShader* playerShader = (TextureShader*)ResMgr->GetShaderByName("texture");
	if (playerShader == nullptr)
		return false;
	*/	
	MeshShader* playerShader = (MeshShader*)ResMgr->GetShaderByName("basic");
	if (playerShader == nullptr)
		return false;
		
	Shader* SkyShader = (MeshShader*)ResMgr->GetShaderByName("Sky");
	if (SkyShader == nullptr)
		return false;

	ID3D11Device* device = Engine::GetEngine()->GetGraphics()->GetDevice();
	ID3D11DeviceContext* deviceContext = Engine::GetEngine()->GetGraphics()->GetDeviceContext();

	m_player = new Player();
	m_player->Initialize(device, deviceContext, playerShader);

	m_sky = EntMgr->CreateEntity(Entity::ENTITY_TYPE_SKY);
	if (m_sky)
		m_sky->Initialize(device, SkyShader);

	return true;
}

void GameScene::Update()
{
	if (m_player)
		m_player->Update();

}

void GameScene::Render(ID3D11DeviceContext * deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{

}
