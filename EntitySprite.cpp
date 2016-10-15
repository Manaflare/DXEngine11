#include "EntitySprite.h"
#include "SystemDefs.h"
#include "ResourceManager.h"


EntitySprite::EntitySprite()
{

	m_vertexBuffer = nullptr;
	m_texture = nullptr;

}


EntitySprite::~EntitySprite()
{
	SafeDelete(m_vertexBuffer);
}

void EntitySprite::Initialize(ID3D11Device * device, Shader * shader)
{
	m_shader = shader;

	m_vertexBuffer = new VertexBuffer();
	m_vertexBuffer->InitializeSprite(device, shader, m_size, m_isWritable);

	const char* textureFileName = m_spriteName.c_str();
	if (textureFileName != NULL)
	{
		m_texture = ResMgr->GetTextureByName((char*)textureFileName);
	}
}

void EntitySprite::Update()
{
}

void EntitySprite::Render(ID3D11DeviceContext * deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	if (m_texture)
	{
		m_shader->SetShaderParameter(deviceContext, m_texture->GetTexture());
		m_shader->SetShaderParameter(deviceContext, m_worldMatrix, viewMatrix, projMatrix);

		m_vertexBuffer->Render(deviceContext);
	}
}
