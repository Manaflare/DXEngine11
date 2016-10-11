#include "TextureShader.h"
#include "SystemDefs.h"

TextureShader::TextureShader(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName) :
	Shader(device, hwnd, shaderFileName, vertexFuncName, pixelFuncName)
{
	//Initializer Shader(Create Layout, vertex, index constant buffer)
	m_bInitialized = Initialize(device);

}

TextureShader::~TextureShader()
{
	SafeRelease(m_samplerState);
}


bool TextureShader::Initialize(ID3D11Device * device)
{
	if (!this->InitializeConstant(device))
	{
		return false;
	}

	if (!this->InitializeSamplerState(device))
	{
		return false;
	}

	return true;
}

bool TextureShader::InitializeSamplerState(ID3D11Device * device)
{
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT result;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//create sampler state
	result = device->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TextureShader::InitializeConstant(ID3D11Device * device)
{
	return true;
}

void TextureShader::Begin(ID3D11DeviceContext * deviceContext, int indexCount, int startIndexLocation)
{
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	Shader::Begin(deviceContext, indexCount, startIndexLocation);
}

void TextureShader::End(ID3D11DeviceContext * deviceContext)
{
	deviceContext->PSSetSamplers(0, 0, nullptr);
	Shader::End(deviceContext);
}

