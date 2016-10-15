#include "EntityManager.h"
#include "Entity.h"
#include "EntityMesh.h"
#include "EntitySky.h"
#include "EntitySprite.h"

enum E_ENTITY_TYPE : int;

EntityManager* EntityManager::m_instance = nullptr;

EntityManager::EntityManager()
{

}


EntityManager::~EntityManager()
{
	RemoveAllEntities();
}

void EntityManager::Update()
{
	for (Entity* entity : m_entities)
	{
		if (entity)
		{
			entity->Update();
		}
	}
}

void EntityManager::Render(ID3D11DeviceContext * deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	for (Entity* entity : m_entities)
	{
		if (entity)
		{
			entity->Render(deviceContext, viewMatrix, projMatrix);
		}
	}
}

Entity * EntityManager::CreateEntity(Entity::E_ENTITY_TYPE type)
{
	Entity* entity;
	
	switch (type)
	{
		case Entity::ENTITY_TYPE_MESH:
		{
			entity = new EntityMesh();
		}
			break;
		case Entity::ENTITY_TYPE_SPRITE:
		{
			entity = new EntitySprite();
		}
		break;
		case Entity::ENTITY_TYPE_SKY:
		{
			entity = new EntitySky();
		}
		break;
	}
	
	
	m_entities.push_back(entity);
	 
	return entity;
}

void EntityManager::RemoveEntity(Entity * entity)
{
	if (entity == nullptr)
		return;

	int index = -1;
	if (ContainsEntity(entity, index))
	{
		m_entities.erase(m_entities.begin() + index);
		delete entity;
		entity = nullptr;
	}
}

void EntityManager::RemoveAllEntities()
{
	while (!m_entities.empty())
	{
		Entity* entity = m_entities.at(0);
		m_entities.erase(m_entities.begin());
		delete entity;
		entity = nullptr;
	}
}

bool EntityManager::ContainsEntity(Entity * entity)
{
	for (Entity* entityIter: m_entities)
	{
		if (entityIter == entity)
			return true;
	}

	return false;
}

bool EntityManager::ContainsEntity(Entity * entity, int & index)
{
	for(size_t i = 0; i< m_entities.size(); ++i)
	{
		if (m_entities[i] == entity)
		{
			index = i;
			return true;
		}
			
	}

	return false;
}
