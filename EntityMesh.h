#pragma once
#include "Entity.h"
#include "VertexBuffer.h"
#include <string>
#include <vector>
#include <d3d11.h>

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

class EntityMesh :
	public Entity
{
public:
	EntityMesh();
	virtual ~EntityMesh();


	void Initialize(ID3D11Device * device, Shader * shader);
	void Update();
	void Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix);


public:
	void SetObjFileName(const char* objFileName) { m_meshName = objFileName; }// to do list
	bool LoadObjModel(ID3D11Device * device, std::string Filename, bool IsRHCoordSys, bool ComputeNormals);
	void CreateAABB();

private:
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

