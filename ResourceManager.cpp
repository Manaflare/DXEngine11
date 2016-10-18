#include "PreCompiled.h"
#include "ResourceManager.h"
#include "SystemDefs.h"

ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{

	while (!m_vecShader.empty())
	{
		SafeDelete(m_vecShader.at(0));
		m_vecShader.erase(m_vecShader.begin());
	}

	while (!m_vecTexture.empty())
	{
		SafeDelete(m_vecTexture.at(0));
		m_vecTexture.erase(m_vecTexture.begin());
	}
}

void ResourceManager::LoadAllTextureResource()
{
	//load all texture resources in the texture folder
}

void ResourceManager::LoadAllShaderResource()
{
	//load all shader resources in the texture folder
}

void ResourceManager::LoadShaderResource(ID3D11Device * device, HWND hwnd, char * shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	//to avoid file path
	string strShaderFileName = shaderFileName;
	int pos = strShaderFileName.find_last_of("/");
	if (pos >= 0)
	{
		strShaderFileName = strShaderFileName.substr(pos + 1, strShaderFileName.length());
	}

	string strFilePath = "Shader/";
	strFilePath += strShaderFileName;

	ResourceShader* resourceShader = new ResourceShader();
	if (!resourceShader->Load(device, hwnd, strFilePath.c_str(), vertexFuncName, pixelFuncName))
	{
		SafeDelete(resourceShader);

		cout << "Unable to Load Shader : " << strFilePath.c_str() << endl;
		return;
	}

	m_vecShader.push_back(resourceShader);

	cout << "Successfully loaded : " << strFilePath.c_str() << endl;

}

void ResourceManager::LoadShaderResource(Shader * shader)
{
	if (shader == nullptr)
		return;

	ResourceShader* resourceShader = new ResourceShader;
	if (!resourceShader->Load(shader))
	{
		cout << "Unable to Load Shader by Shader Pointer : " << resourceShader->GetName() << endl;
		SafeDelete(resourceShader);
		return;
	}

	m_vecShader.push_back(resourceShader);

	cout << "Successfully loaded by Shader Pointer : " << resourceShader->GetName() << endl;

}

void ResourceManager::LoadTextureResource(ID3D11Device * device, const char * textureFileName)
{
	//to avoid file path
	string strTextureName = textureFileName;
	int pos = strTextureName.find_last_of("/");
	if (pos >= 0)
	{
		strTextureName = strTextureName.substr(pos + 1, strTextureName.length());
	}

	string strFilePath = "Texture/";
	strFilePath += strTextureName;

	ResourceTexture* resourceTexture = new ResourceTexture();
	if (!resourceTexture->Load(device, strFilePath.c_str()))
	{
		SafeDelete(resourceTexture);

		cout << "Unable to Load Texture : " << strFilePath.c_str() << endl;
		return;
	}

	m_vecTexture.push_back(resourceTexture);

	cout << "Successfully loaded : " << strFilePath.c_str() << endl;
}

Shader * ResourceManager::GetShaderByName(char * shaderName)
{
	for (ResourceShader* resourceShader : m_vecShader)
	{
		string resourceShaderName = resourceShader->GetName();
		if (resourceShaderName == shaderName)
			return resourceShader->GetShader();
	}
	 
	return nullptr;
}

Texture * ResourceManager::GetTextureByName(char * textureName)
{
	for (ResourceTexture* resourceTexture : m_vecTexture)
	{
		string resourceTextureName = resourceTexture->GetName();
		if (resourceTextureName == textureName)
			return resourceTexture->GetTexture();
	}

	return nullptr;
}

Texture * ResourceManager::GetTextureByIndex(int index)
{
	if (index < 0)
		return nullptr;
	else
		return m_vecTexture.at(index)->GetTexture();
}

int ResourceManager::GetTextureIndexByName(const char * textureName)
{
	//get rid of ext
	string strTextureName = textureName;
	strTextureName = strTextureName.substr(0, strTextureName.find_last_of("."));

	for (size_t index = 0; index < m_vecTexture.size(); ++index)
	{
		string resourceTextureName = m_vecTexture.at(index)->GetName();
		if (resourceTextureName == strTextureName)
			return index;
	}

	return -1;
}
