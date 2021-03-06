#pragma once
#include "Shader.h"
class MeshShader :
	public Shader
{
private:
	enum
	{
		NUM_OBJECT_BIND = 0,
		NUM_FRAME_BIND,
	};

	struct CB_PS_PER_OBJECT
	{
		XMFLOAT4 m_vObjectColor;
	};

	struct CB_PS_PER_FRAME
	{
		XMFLOAT3 m_vLightDirAmbient;
		float m_fAmbient;
	};

public:
	MeshShader(ID3D11Device * device, HWND hwnd, LPCSTR shaderFileName, LPCSTR vertexFuncName, LPCSTR pixelFuncName);
	virtual ~MeshShader();

	bool SetShaderParameter(ID3D11DeviceContext* deviceContext, XMFLOAT4 vecValue);

	bool Initialize(ID3D11Device * device);
	bool InitializeSamplerState(ID3D11Device * device);
	bool InitializeConstant(ID3D11Device* device);

	void Begin(ID3D11DeviceContext * deviceContext, int indexCount, int startIndexLocation);
	void End(ID3D11DeviceContext * deviceContext);


private:
	ID3D11Buffer* m_objColorBuffer;
	ID3D11Buffer* m_LightBuffer;
};

