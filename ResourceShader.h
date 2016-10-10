#pragma once

#include "Shader.h"

class ResourceShader
{
public:
	ResourceShader();
	~ResourceShader();

	bool Load(ID3D11Device* device, HWND hwnd, const char* shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	bool Load(Shader* shader);

	Shader* GetShader() { return m_shader; }
	string GetName() { return m_shader->GetName(); }

private:
	Shader* m_shader;
};

