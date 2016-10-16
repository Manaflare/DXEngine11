#pragma once

#include "DirectXTex.h"
#include <D3D11.h>
#include <string>

using namespace std;

class Texture
{
public:

	enum E_TEXTURE_TYPE
	{
		TEXTURE_TYPE_TEXTURE1D = 0,
		TEXTURE_TYPE_TEXTURE2D,
		TEXTURE_TYPE_TEXTURE3D,
		TEXTURE_TYPE_TEXTURECUBE,
	};
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, LPCSTR fileName, E_TEXTURE_TYPE type = TEXTURE_TYPE_TEXTURE2D);

	DirectX::ScratchImage LoadTextureFromFile(LPCSTR fileName);

	ID3D11ShaderResourceView* GetTexture() { return m_texture; }
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	string GetName() { return m_name; }
private:
	ID3D11ShaderResourceView* m_texture;
	string m_name;

	int m_width;
	int m_height;

};

