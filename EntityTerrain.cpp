#include "EntityTerrain.h"



EntityTerrain::EntityTerrain()
{
}


EntityTerrain::~EntityTerrain()
{
}

void EntityTerrain::Initialize(ID3D11Device * device, Shader * shader)
{
	m_shader = shader;

}

void EntityTerrain::Update()
{

}

void EntityTerrain::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{

}
