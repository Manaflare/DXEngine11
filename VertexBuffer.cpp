#include "VertexBuffer.h"
#include "SystemDefs.h"


VertexBuffer::VertexBuffer()
{
	m_vertices = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_shader = nullptr;
}


VertexBuffer::~VertexBuffer()
{
	SafeDeleteArray(m_vertices);
	SafeRelease(m_vertexBuffer);
	SafeRelease(m_indexBuffer);
}

bool VertexBuffer::InitializeMesh(ID3D11Device * device, Shader * shader, VertexType* vertices, unsigned long* Indices, int meshTriangles, int totalVerts)
{
	HRESULT result;
	//Create index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));


	m_vertexCount = totalVerts;
	m_indexCount = meshTriangles * 3;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	//iinitData.pSysMem = &Indices[0];
	iinitData.pSysMem = Indices;
	device->CreateBuffer(&indexBufferDesc, &iinitData, &m_indexBuffer);

	//Create Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * totalVerts;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	//vertexBufferData.pSysMem = &vertices[0];
	vertexBufferData.pSysMem = vertices;
	result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	return true;
}

bool VertexBuffer::InitializeSprite(ID3D11Device * device, Shader * shader, float size, bool writeable)
{
	m_shader = shader;

	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));


	m_vertexCount = 4;
	m_indexCount = 6;

	m_vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	float halfsize = size * 0.5f;

	//load vertex array
	m_vertices[0].position = XMFLOAT3(-halfsize, -halfsize, 0.f);
	m_vertices[0].texCoord = XMFLOAT2(0.f, 1.f);

	m_vertices[1].position = XMFLOAT3(-halfsize, halfsize, 0.f);
	m_vertices[1].texCoord = XMFLOAT2(0.f, 0.f);

	m_vertices[2].position = XMFLOAT3(halfsize, halfsize, 0.f);
	m_vertices[2].texCoord = XMFLOAT2(1.f, 0.f);

	m_vertices[3].position = XMFLOAT3(halfsize, -halfsize, 0.f);
	m_vertices[3].texCoord = XMFLOAT2(1.f, 1.f);

	//load indices
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	//setup vertex buffer description
	vertexBufferDesc.Usage = (writeable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = (writeable) ? D3D11_CPU_ACCESS_WRITE : 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	//setup vertex data
	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//create vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//set up index buffer description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//create vertex buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//release
	SafeDeleteArray(indices);

	return true;
}

void VertexBuffer::Render(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	m_shader->Begin(deviceContext, m_indexCount);

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
