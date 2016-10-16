#pragma once
#include "GameComponent.h"

class Player;
class Entity;

class GameScene :
	public GameComponent
{
public:
	GameScene();
	virtual ~GameScene();

	bool Initialize();
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);

private:
	Player* m_player;
	Entity* m_sky;

};

