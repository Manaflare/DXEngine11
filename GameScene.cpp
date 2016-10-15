#include "GameScene.h"
#include "Player.h"
#include "TextureShader.h "
#include "ResourceManager.h"
#include "MeshShader.h"

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
		
	m_player = new Player();
	m_player->Initialize(Engine::GetEngine()->GetGraphics()->GetDevice(), Engine::GetEngine()->GetGraphics()->GetDeviceContext(), playerShader);

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
