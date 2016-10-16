#include "EntitySky.h"
#include "ResourceManager.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "Engine.h"
EntitySky::EntitySky()
{
	m_numSphereFaces = 0;
	m_type = E_ENTITY_TYPE::ENTITY_TYPE_SKY;
}


EntitySky::~EntitySky()
{
}

void EntitySky::Initialize(ID3D11Device * device, Shader * shader)
{

	m_shader = shader;

	const char* skyTextureName = "Skybox3.dds";
	ResMgr->LoadTextureResource(device, skyTextureName);
	
	CreateSphere(device, shader, 10, 10);
	
}

void EntitySky::Update()
{
	Camera* camera = Engine::GetEngine()->GetCamera();
	//Make sure the sphere is always centered around camera
	SetPosition(camera->GetPosition());

	Entity::Update();
}

void EntitySky::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	m_vertexBuffer->Render(deviceContext);
	m_shader->SetShaderParameter(deviceContext, m_worldMatrix, viewMatrix, projMatrix);

	Texture* cubeTexture = ResMgr->GetTextureByName("Skybox3");
	if (cubeTexture)
		m_shader->SetShaderParameter(deviceContext, cubeTexture->GetTexture());

	//deviceContext->PSSetSamplers(0, 1, &CubesTexSamplerState);
	deviceContext->DrawIndexed(m_numSphereFaces * 3, 0, 0);
}

void EntitySky::CreateSphere(ID3D11Device * device, Shader * shader, int LatLines, int LongLines)
{
	XMMATRIX Rotationx;
	XMMATRIX Rotationz;
	XMMATRIX Rotationy;

	int numSphereVertices = ((LatLines - 2) * LongLines) + 2;
	int numSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

	m_numSphereFaces = numSphereFaces;

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<VertexBuffer::VertexType> vertices(numSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].position.x = 0.0f;
	vertices[0].position.y = 0.0f;
	vertices[0].position.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14f / (LatLines - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28f / (LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].position.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].position.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].position.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[numSphereVertices - 1].position.x = 0.0f;
	vertices[numSphereVertices - 1].position.y = 0.0f;
	vertices[numSphereVertices - 1].position.z = -1.0f;

	//create index buffer
	std::vector<DWORD> indices(numSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i*LongLines + j + 1;
			indices[k + 1] = i*LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i*LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = numSphereVertices - 1;
		indices[k + 1] = (numSphereVertices - 1) - (l + 1);
		indices[k + 2] = (numSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = numSphereVertices - 1;
	indices[k + 1] = (numSphereVertices - 1) - LongLines;
	indices[k + 2] = numSphereVertices - 2;


	m_vertexBuffer->InitializeMesh(device, shader, &vertices[0], &indices[0], numSphereFaces, numSphereVertices);
// 
// 	m_vertexBuffer->
// 
// 	D3D11_BUFFER_DESC vertexBufferDesc;
// 	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
// 
// 	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
// 	vertexBufferDesc.ByteWidth = sizeof(VertexBuffer::VertexType) * numSphereVertices;
// 	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// 	vertexBufferDesc.CPUAccessFlags = 0;
// 	vertexBufferDesc.MiscFlags = 0;
// 
// 	D3D11_SUBRESOURCE_DATA vertexBufferData;
// 
// 	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
// 	vertexBufferData.pSysMem = &vertices[0];
// 	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);
// 
// 
// 
// 	D3D11_BUFFER_DESC indexBufferDesc;
// 	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
// 
// 	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
// 	indexBufferDesc.ByteWidth = sizeof(DWORD) * numSphereFaces * 3;
// 	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
// 	indexBufferDesc.CPUAccessFlags = 0;
// 	indexBufferDesc.MiscFlags = 0;
// 
// 	D3D11_SUBRESOURCE_DATA iinitData;
// 
// 	iinitData.pSysMem = &indices[0];
// 	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &m_);
}
