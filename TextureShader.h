#pragma once
#include "Shader.h"

class TextureShader : public Shader
{
public:
	TextureShader(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	virtual ~TextureShader();

	void Begin(ID3D11DeviceContext* deviceContext, int indexCount, int startIndexLocation = 0);
	void End(ID3D11DeviceContext* deviceContext);

protected:
	bool Initialize(ID3D11Device* device);
	bool InitializeSamplerState(ID3D11Device * device);
	bool InitializeConstant(ID3D11Device* device);

};

