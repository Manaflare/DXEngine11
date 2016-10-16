#pragma once
#include "Shader.h"
class SkyShader :
	public Shader
{
public:
	SkyShader(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	virtual ~SkyShader();

	void Begin(ID3D11DeviceContext* deviceContext, int indexCount, int startIndexLocation = 0);
	void End(ID3D11DeviceContext* deviceContext);
};

