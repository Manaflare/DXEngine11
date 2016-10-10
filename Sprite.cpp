#include "Sprite.h"
#include "SystemDefs.h"
#include "ResourceManager.h"

Sprite::Sprite(float size)
{
	m_vertexBuffer = nullptr;
	m_texture = nullptr;

	m_size = size;
}


Sprite::~Sprite()
{
	SafeDelete(m_vertexBuffer);
}

void Sprite::Initialize(ID3D11Device * device, Shader * shader, LPCSTR textureFileName, bool isWritable)
{
	m_shader = shader;

	m_vertexBuffer = new VertexBuffer();
	m_vertexBuffer->InitializeSprite(device, shader, m_size, isWritable);

	if (textureFileName != NULL)
	{
		m_texture = ResMgr->GetTextureByName((char*)textureFileName);
	}
}

void Sprite::Update()
{
}

void Sprite::Render(ID3D11DeviceContext * deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	if (m_texture)
	{
		m_shader->SetShaderParameter(deviceContext, m_texture->GetTexture());
		m_shader->SetShaderParameter(deviceContext, worldMatrix, viewMatrix, projMatrix);

		m_vertexBuffer->Render(deviceContext);
	}
}
