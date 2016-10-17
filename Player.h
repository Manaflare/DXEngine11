#pragma once

#include "Engine.h"
class Entity;
class Shader;

class Player
{
public:
	Player();
	~Player();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Shader* shader);
	void Update();
	void UpdateInput();

private:
	Entity* m_entity[2];

};

