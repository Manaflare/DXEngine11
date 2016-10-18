#include "PreCompiled.h"
#include "MeshShader.h"
#include "SystemDefs.h"


MeshShader::MeshShader(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName) :
Shader(device, hwnd, shaderFileName, vertexFuncName, pixelFuncName)
{
	m_objColorBuffer = nullptr;
	m_LightBuffer = nullptr;
	InitializeSamplerState(device);
	m_bInitialized = InitializeConstant(device);
}


MeshShader::~MeshShader()
{
	SafeRelease(m_objColorBuffer);
	SafeRelease(m_LightBuffer);
}

bool MeshShader::SetShaderParameter(ID3D11DeviceContext * deviceContext, XMFLOAT4 vecValue)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//obj Color
	result = deviceContext->Map(m_objColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	CB_PS_PER_OBJECT* pPSPerObject = (CB_PS_PER_OBJECT*)mappedResource.pData;
	pPSPerObject->m_vObjectColor = XMFLOAT4(1, 1, 1, 1);
	
	deviceContext->Unmap(m_objColorBuffer, 0);

	deviceContext->PSSetConstantBuffers(0, 1, &m_objColorBuffer);
	
	
	//Light Dir
	result = deviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	CB_PS_PER_FRAME* pPSFrameObject = (CB_PS_PER_FRAME*)mappedResource.pData;
	pPSFrameObject->m_vLightDirAmbient = XMFLOAT3(0.f, 1.f, 1.f);
	pPSFrameObject->m_fAmbient = 1.f;

	deviceContext->Unmap(m_LightBuffer, 0);

	deviceContext->PSSetConstantBuffers(1, 1, &m_LightBuffer);

	return true;
}

bool MeshShader::Initialize(ID3D11Device * device)
{
	return true;
}

bool MeshShader::InitializeSamplerState(ID3D11Device * device)
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
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
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

bool MeshShader::InitializeConstant(ID3D11Device * device)
{
	HRESULT result;
	// Setup constant buffers
	D3D11_BUFFER_DESC Desc;
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;
	Desc.ByteWidth = sizeof(CB_PS_PER_OBJECT);
	result = device->CreateBuffer(&Desc, NULL, &m_objColorBuffer);
	if (FAILED(result))
	{
		return false;
	}

	Desc.ByteWidth = sizeof(CB_PS_PER_FRAME);
	result = device->CreateBuffer(&Desc, NULL, &m_LightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void MeshShader::Begin(ID3D11DeviceContext * deviceContext, int indexCount, int startIndexLocation)
{
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	Shader::Begin(deviceContext, indexCount, startIndexLocation);
}

void MeshShader::End(ID3D11DeviceContext * deviceContext)
{
	deviceContext->PSSetSamplers(0, 0, nullptr);
	Shader::End(deviceContext);
}