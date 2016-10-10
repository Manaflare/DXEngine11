#pragma once

#include <D3D11.h>
#include <DirectXMath.h>
#include <D3Dcompiler.h>
#include <fstream>
#include <string>

using namespace std;
using namespace DirectX;

class Shader
{

private:
	struct MatrixBufferType
	{
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 ProjMatrix;
	};

public:
	Shader(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	virtual ~Shader();

	virtual void Begin(ID3D11DeviceContext* deviceContext, int indexCount);
	virtual void End(ID3D11DeviceContext* deviceContext);

	bool SetShaderParameter(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
	bool SetShaderParameter(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);

	string GetName() { return m_name;  }
	bool IsInitialized() { return m_bInitialized;  }

protected:
	virtual bool Initialize(ID3D11Device* device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);

	bool m_bInitialized;
private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCSTR vsFileName, LPCSTR psFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, LPCSTR shaderFileName);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	
	string m_name;
};

