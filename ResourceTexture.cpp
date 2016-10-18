#include "PreCompiled.h"
#include "ResourceTexture.h"
#include "SystemDefs.h"


ResourceTexture::ResourceTexture()
{
	m_texture = nullptr;
}


ResourceTexture::~ResourceTexture()
{
	SafeDelete(m_texture);
}

bool ResourceTexture::Load(ID3D11Device * device, const char * textureFileName)
{
	m_texture = new Texture();
	if (!m_texture->Initialize(device, textureFileName))
	{
		SafeDelete(m_texture);
		return false;
	}

	return true;
}
