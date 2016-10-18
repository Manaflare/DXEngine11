#include "PreCompiled.h"
#include "ResourceShader.h"
#include "SystemDefs.h"


ResourceShader::ResourceShader()
{
	m_shader = nullptr;
}


ResourceShader::~ResourceShader()
{
	SafeDelete(m_shader);
}

bool ResourceShader::Load(ID3D11Device * device, HWND hwnd, const char * shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	m_shader = new Shader(device, hwnd, shaderFileName, vertexFuncName, pixelFuncName);
	if (!m_shader->IsInitialized())
	{
		SafeDelete(m_shader);
		return false;
	}

	return true;
}

bool ResourceShader::Load(Shader * shader)
{
	if (shader == nullptr)
		return false;

	m_shader = shader;

	if (!m_shader->IsInitialized())
	{
		SafeDelete(m_shader);
		return false;
	}
		
	return true;
}
