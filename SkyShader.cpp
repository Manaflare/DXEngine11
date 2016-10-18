#include "PreCompiled.h"
#include "SkyShader.h"



SkyShader::SkyShader(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
	:Shader(device, hwnd, shaderFileName, vertexFuncName, pixelFuncName)
{
}


SkyShader::~SkyShader()
{
}

void SkyShader::Begin(ID3D11DeviceContext* deviceContext, int indexCount, int startIndexLocation /*= 0*/)
{

}

void SkyShader::End(ID3D11DeviceContext* deviceContext)
{

}
