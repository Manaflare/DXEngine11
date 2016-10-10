#pragma once

#include "Texture.h"
class ResourceTexture
{
public:
	ResourceTexture();
	~ResourceTexture();

	bool Load(ID3D11Device* device, const char* textureFileName);

	Texture* GetTexture() { return m_texture; }
	string GetName() { return m_texture->GetName(); }


private:
	Texture* m_texture;
};

