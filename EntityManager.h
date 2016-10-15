#pragma once

#include <vector>
#include <DirectXMath.h>
#include "Entity.h"
using namespace std;
using namespace DirectX;

class Entity;
struct ID3D11DeviceContext;


class EntityManager
{
public:
	EntityManager();
	~EntityManager();


	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix );

	Entity* CreateEntity(Entity::E_ENTITY_TYPE type);
	void RemoveEntity(Entity* entity);
	void RemoveAllEntities();

	bool ContainsEntity(Entity* entity);
	bool ContainsEntity(Entity* entity, int& index);


	static EntityManager* GetInstance()
	{
		if (m_instance == nullptr)
			m_instance = new EntityManager();

		return m_instance;
	}

private:
	static EntityManager* m_instance;
	vector<Entity*> m_entities;
};

#define EntMgr EntityManager::GetInstance()

