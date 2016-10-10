#pragma once

#include "DirectXTex.h"
#include <D3D11.h>
#include <string>

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	bool Initialize(ID3D11Device* device, LPCSTR fileName);

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

