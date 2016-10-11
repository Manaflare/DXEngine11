#pragma once

#include "ResourceShader.h"
#include "ResourceTexture.h"

#include <iostream>
#include <vector>

class ResourceManager
{
public:
	~ResourceManager();

	void LoadShaderResource(ID3D11Device* device, HWND hwnd, char* shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	void LoadShaderResource(Shader* shader);
	void LoadTextureResource(ID3D11Device* device, const char* textureFileName);

	Shader* GetShaderByName(char* shaderName);
	Texture* GetTextureByName(char* textureName);
	Texture* GetTextureByIndex(int index);
	
	int		GetTextureIndexByName(const char* textureName);

	static ResourceManager* GetInstance()
	{
		if (m_instance == nullptr)
			m_instance = new ResourceManager();

		return m_instance;
	}

private:
	ResourceManager();

	vector<ResourceTexture*> m_vecTexture;
	vector<ResourceShader*> m_vecShader;

	static ResourceManager* m_instance;
};

#define ResMgr ResourceManager::GetInstance() 