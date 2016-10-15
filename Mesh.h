#pragma once
#include <vector>
#include <DirectXMath.h>
#include <string>
#include <d3d11.h>
#include "VertexBuffer.h"

using namespace DirectX;
using namespace std;

class Texture;
class Shader;
class VertexBuffer;

// Create material structure
struct SurfaceMaterial
{
	std::string MatName;   // So we can match the subset with it's material

							// Surface's colors
	XMFLOAT4 Diffuse;       // Transparency (Alpha) stored in 4th component
	XMFLOAT3 Ambient;
	XMFLOAT4 Specular;      // Specular power stored in 4th component

							// Texture ID's to look up texture in SRV array
	int DiffuseTextureID;
	int AmbientTextureID;
	int SpecularTextureID;
	int AlphaTextureID;
	int NormMapTextureID;

	// Booleans so we don't implement techniques we don't need
	bool HasDiffTexture;
	bool HasAmbientTexture;
	bool HasSpecularTexture;
	bool HasAlphaTexture;
	bool HasNormMap;
	bool IsTransparent;
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	//Define LoadObjModel function after we create surfaceMaterial structure
	bool LoadObjModel(ID3D11Device* device,     // Pointer to the device
		std::string Filename,                      // obj model filename (model.obj)
		bool IsRHCoordSys,                          // True if model was created in right hand coord system
		bool ComputeNormals);                       // True to compute the normals, false to use the files normals
													///////////////**************new**************////////////////////

	void Initialize(ID3D11Device* device, Shader* shader, LPCSTR objFileName, bool isWritable = false);
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);


private:

	void CreateAABB();

	VertexBuffer* m_vertexBuffer;
	Texture* m_texture;
	Shader* m_shader;

	int Subsets;                        // Number of subsets in obj model
	std::vector<XMFLOAT3> vertexPosition;     // Models vertex positions list
	std::vector<VertexBuffer::VertexType> vertices;
	std::vector<unsigned long> Indices;         // Models index list
	std::vector<int> SubsetIndexStart;  // Subset's index offset
	std::vector<int> SubsetMaterialID;  // Lookup ID for subsets surface material
	std::vector<XMFLOAT3> AABB;			// Stores models AABB (min vertex, max vertex, and center)
										// Where AABB[0] is the min Vertex, and AABB[1] is the max vertex
	XMFLOAT3 Center;					// True center of the model
	float BoundingSphere;				// Model's bounding sphere

	int m_meshTriangles;
	int m_triangleCount;  // Total Triangles
	int m_totalVerts;

	std::vector<SurfaceMaterial> m_material;
	std::string m_meshName;
};




