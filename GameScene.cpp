#include "GameScene.h"
#include "Player.h"
#include "TextureShader.h "
#include "ResourceManager.h"

GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

bool GameScene::Initialize()
{
	TextureShader* textureShader = (TextureShader*)ResMgr->GetShaderByName("texture");
	if (textureShader == nullptr)
		return false;

	m_player = new Player();
	m_player->Initialize(Engine::GetEngine()->GetGraphics()->GetDevice(), Engine::GetEngine()->GetGraphics()->GetDeviceContext(), textureShader);

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
