#pragma once
#include "Entity.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include <string>

class EntitySprite :
	public Entity
{
public:
	EntitySprite();
	virtual ~EntitySprite();

	void Initialize(ID3D11Device * device, Shader * shader);
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);


	void SetSize(float size) { m_size = size; }
	void SetSpriteName(const char* spriteName) { m_spriteName = spriteName; }
	void SetWritable(bool bWritable) { m_isWritable = bWritable; }
private:
	VertexBuffer* m_vertexBuffer;
	Texture* m_texture;
	std::string m_spriteName;
	Shader* m_shader;
	float m_size;
	bool m_isWritable;

};

