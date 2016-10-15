#pragma once
#include "Shader.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
class VertexBuffer
{
public:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
	};

	VertexBuffer();
	~VertexBuffer();

	bool InitializeMesh(ID3D11Device * device, Shader * shader, VertexType* vertices, unsigned long* Indices, int meshTriangles, int totalVerts);
	bool InitializeSprite(ID3D11Device* device, Shader* shader, float size, bool writeable = false);
	void Render(ID3D11DeviceContext* deviceContext);

	VertexType* GetVertices() { return m_vertices;  }
	ID3D11Buffer* GetVertexBuffer() const { return m_vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() const { return m_indexBuffer; }
	int GetIndexCount() const { return m_indexCount; }
	int GetStartIndexLocation() const { return m_StartIndexLocation;  }

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	VertexType* m_vertices;
	Shader* m_shader;
	
	int m_vertexCount;
	int m_indexCount;
	int m_StartIndexLocation;
};

