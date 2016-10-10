#include "Shader.h"
#include "SystemDefs.h"

Shader::Shader(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;

	m_bInitialized = Initialize(device, hwnd, shaderFileName, vertexFuncName, pixelFuncName);
}

Shader::~Shader()
{

	SafeRelease(m_matrixBuffer);
	SafeRelease(m_layout);
	SafeRelease(m_pixelShader);
	SafeRelease(m_vertexShader);

	m_name.clear();
}

void Shader::Begin(ID3D11DeviceContext * deviceContext, int indexCount)
{
	//set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	//set the vertex and pixel 
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//render
	deviceContext->DrawIndexed(indexCount, 0, 0);

}

void Shader::End(ID3D11DeviceContext * deviceContext)
{

	deviceContext->IASetInputLayout(NULL);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
}

bool Shader::SetShaderParameter(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

bool Shader::SetShaderParameter(ID3D11DeviceContext * deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	//transpose all matrices
	XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix));
	XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));
	XMMatrixTranspose(XMLoadFloat4x4(&projMatrix));

	//lock buffer
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->worldMatrix = worldMatrix;
	dataPtr->viewMatrix = viewMatrix;
	dataPtr->ProjMatrix = projMatrix;

	//unlock buffer
	deviceContext->Unmap(m_matrixBuffer, 0);

	//update values in the shader
	deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);



	return true;
}

bool Shader::Initialize(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	string shaderFilePath = shaderFileName;
	m_name = shaderFilePath;
	
	int pos = m_name.find_last_of("/");
	if (pos >= 0)
	{
		m_name = m_name.substr(pos + 1, m_name.length());
	}

	string vsFileName = shaderFileName;
	vsFileName += ".vs";

	string psFileName = shaderFileName;
	psFileName += ".ps";

	//initialize the vertex and pixel shader
	return InitializeShader(device, hwnd, vsFileName.c_str(), psFileName.c_str(), vertexFuncName, pixelFuncName);

}

bool Shader::InitializeShader(ID3D11Device * device, HWND hwnd, LPCSTR vsFileName, LPCSTR psFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

		
	//compile vertex shader
//	result = D3DCompile(vsFileName, NULL, NULL, vertexFuncName, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);

	std::string str(vsFileName);
	std::wstring wsTmp(str.begin(), str.end());

	std::wstring wvsFileName = wsTmp;

	result = D3DCompileFromFile(wvsFileName.c_str(), NULL,NULL,vertexFuncName, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		else
		{
			MessageBox(hwnd, vsFileName, "Error in Shader File", MB_OK);
		}

		return false;
	}

	str = psFileName;
	std::wstring wpsTmp(str.begin(), str.end());
	std::wstring wpsFileName = wpsTmp;

    result = D3DCompileFromFile(wpsFileName.c_str(), NULL, NULL, pixelFuncName, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFileName);
		}
		else
		{
			MessageBox(hwnd, psFileName, "Error in Shader File", MB_OK);
		}

		return false;
	}

	//create vertex shader buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//create pixel shader buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}
	
	//setup layout of the data that goes into the shader
	
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	SafeRelease(vertexShaderBuffer);
	SafeRelease(pixelShaderBuffer);

	//setup the matrix buffer description
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create the constant buffer pointer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Shader::OutputShaderErrorMessage(ID3DBlob * errorMessage, HWND hwnd, LPCSTR shaderFileName)
{
	char* compileError = (char*)errorMessage->GetBufferPointer();
	unsigned long bufferSize = errorMessage->GetBufferSize();

	ofstream fout;

	//open file to write
	fout.open("shader-error.txt");

	for (unsigned int i = 0; i < bufferSize; ++i)
	{
		fout << compileError[i];
	}

	fout.close();

	//release
	SafeRelease(errorMessage);

	MessageBox(hwnd, "error compiling shader. check shader-error.txt", shaderFileName, MB_OK);

}