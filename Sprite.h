#pragma once

#include "VertexBuffer.h"
#include "Texture.h"

class Sprite
{
public:
	Sprite(float size);
	virtual ~Sprite();

	virtual void Initialize(ID3D11Device* device, Shader* shader, LPCSTR textureFileName, bool isWritable = false);
	virtual void Update();
	virtual void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);

protected:
	VertexBuffer* m_vertexBuffer;
	Texture* m_texture;
	Shader* m_shader;
	float m_size;
};

