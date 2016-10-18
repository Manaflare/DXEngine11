#include "PreCompiled.h"
#include "EntityMesh.h"
#include "VertexBuffer.h"
#include "ResourceManager.h"
#include "MeshShader.h"
#include <fstream>
#include <iostream>
#include <sstream>

EntityMesh::EntityMesh()
{

	Subsets = 0;

	m_triangleCount = 0;  // Total Triangles
	m_totalVerts = 0;
	m_meshTriangles = 0;

	m_type = E_ENTITY_TYPE::ENTITY_TYPE_MESH;
}


EntityMesh::~EntityMesh()
{
}

void EntityMesh::Initialize(ID3D11Device * device, Shader * shader)
{
	m_shader = shader;
	
	timer.StartTimer();
	if (!this->LoadObjModel(device, m_meshName.c_str(), true, true))
	{
		cout << "Couldn't LoadObjModel" << m_meshName.c_str() << endl;
		return;
	}
	cout << "Load Model Succesfully : " << m_meshName.c_str() << " Time is : " << timer.EndTimer() << " seconds" << endl;
	m_meshName = m_meshName.substr(0, m_meshName.find_last_of("."));

	m_vertexBuffer->InitializeMesh(device, shader, &vertices[0], &Indices[0], m_meshTriangles, m_totalVerts);

	//if (textureFileName != NULL)
	//{
	//	m_texture = ResMgr->GetTextureByName((char*)textureFileName);
	//}
}

bool EntityMesh::LoadObjModel(ID3D11Device * device, std::string Filename, bool IsRHCoordSys, bool ComputeNormals)
{
	ifstream fileIn(Filename, ios::in);
	std::string meshMatLib;                    // String to hold our obj material library filename (model.mtl)

											   // Arrays to store our model's information
	std::vector<DWORD> indices;
	std::vector<XMFLOAT3> vertPos;
	std::vector<XMFLOAT3> vertNorm;
	std::vector<XMFLOAT2> vertTexCoord;
	std::vector<std::string> meshMaterials;

	// Vertex definition indices
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;

	// Make sure we have a default if no tex coords or normals are defined
	bool hasTexCoord = false;
	bool hasNorm = false;

	// Temp variables to store into vectors
	std::string meshMaterialsTemp;
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	char checkChar;      // The variable we will use to store one char from file at a time
	std::string face;      // Holds the string containing our face vertices
	int vIndex = 0;         // Keep track of our vertex index count

	if (!fileIn)
	{
		cout << "Cannot open " << Filename << endl;
		return false;
	}

	if (fileIn)
	{
		while (fileIn)
		{
			checkChar = fileIn.get();

			switch (checkChar)
			{
				// A comment. Skip rest of the line
			case '#':
				checkChar = fileIn.get();
				while (checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'v':
				checkChar = fileIn.get();
				if (checkChar == ' ')  // v - vert position
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;   // Store the next three types

					if (IsRHCoordSys)            // If model is from an RH Coord System
						vertPos.push_back(XMFLOAT3(vx, vy, vz * -1.0f));   // Invert the Z axis
					else
						vertPos.push_back(XMFLOAT3(vx, vy, vz));
				}
				if (checkChar == 't')  // vt - vert tex coords
				{
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;     // Store next two types

					if (IsRHCoordSys)            // If model is from an RH Coord System
						vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f - vtcv));  // Reverse the "v" axis
					else
						vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));

					hasTexCoord = true;         // We know the model uses texture coords
				}
				if (checkChar == 'n')          // vn - vert normal
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;    // Store next three types

					if (IsRHCoordSys)                // If model is from an RH Coord System
						vertNorm.push_back(XMFLOAT3(vnx, vny, vnz * -1.0f));  // Invert the Z axis
					else
						vertNorm.push_back(XMFLOAT3(vnx, vny, vnz));

					hasNorm = true;                 // We know the model defines normals
				}
				break;
				// New group (Subset)
			case 'g': // g - defines a group
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					SubsetIndexStart.push_back(vIndex);       // Start index for this subset
					Subsets++;
				}
				break;
			case 'f': // f - defines the faces
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					face = "";               // Holds all vertex definitions for the face (eg. "1\1\1 2\2\2 3\3\3")
					std::string VertDef;   // Holds one vertex definition at a time (eg. "1\1\1")
					m_triangleCount = 0;      // Keep track of the triangles for this face, since we will have to 
											  // "retriangulate" faces with more than 3 sides

					checkChar = fileIn.get();
					while (checkChar != '\n')
					{
						face += checkChar;          // Add the char to our face string
						checkChar = fileIn.get();   // Get the next Character
						if (checkChar == ' ')      // If its a space...
							m_triangleCount++;        // Increase our triangle count
					}

					// Check for space at the end of our face string
					if (face[face.length() - 1] == ' ')
						m_triangleCount--;    // Each space adds to our triangle count

					m_triangleCount -= 1;     // Ever vertex in the face AFTER the first two are new faces

					std::stringstream ss(face);

					if (face.length() > 0)
					{
						int firstVIndex, lastVIndex;    // Holds the first and last vertice's index

						for (int i = 0; i < 3; ++i)      // First three vertices (first triangle)
						{
							ss >> VertDef;  // Get vertex definition (vPos/vTexCoord/vNorm)

							std::string vertPart;
							int whichPart = 0;      // (vPos, vTexCoord, or vNorm)

													// Parse this string
							for (size_t j = 0; j < VertDef.length(); ++j)
							{
								if (VertDef[j] != '/') // If there is no divider "/", add a char to our vertPart
									vertPart += VertDef[j];

								// If the current char is a divider "/", or its the last character in the string
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::istringstream wstringToInt(vertPart); // Used to convert wstring to int

									if (whichPart == 0)  // If it's the vPos
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;      // subtract one since c++ arrays start with 0, and obj start with 1

																	// Check to see if the vert pos was the only thing specified
										if (j == VertDef.length() - 1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}

									else if (whichPart == 1) // If vTexCoord
									{
										if (vertPart != "")   // Check to see if there even is a tex coord
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;   // subtract one since c++ arrays start with 0, and obj start with 1
										}
										else    // If there is no tex coord, make a default
											vertTCIndexTemp = 0;

										// If the cur. char is the second to last in the string, then
										// there must be no normal, so set a default normal
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;

									}
									else if (whichPart == 2) // If vNorm
									{
										std::istringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;     // subtract one since c++ arrays start with 0, and obj start with 1
									}

									vertPart = "";   // Get ready for next vertex part
									whichPart++;    // Move on to next vertex part                  
								}
							}

							// Check to make sure there is at least one subset
							if (Subsets == 0)
							{
								SubsetIndexStart.push_back(vIndex);       // Start index for this subset
								Subsets++;
							}

							// Avoid duplicate vertices
							bool vertAlreadyExists = false;
							if (m_totalVerts >= 3) // Make sure we at least have one triangle to check
							{
								// Loop through all the vertices
								for (int iCheck = 0; iCheck < m_totalVerts; ++iCheck)
								{
									// If the vertex position and texture coordinate in memory are the same
									// As the vertex position and texture coordinate we just now got out
									// of the obj file, we will set this faces vertex index to the vertex's
									// index value in memory. This makes sure we don't create duplicate vertices
									if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											Indices.push_back(iCheck);        // Set index for this vertex
											vertAlreadyExists = true;       // If we've made it here, the vertex already exists
										}
									}
								}
							}

							// If this vertex is not already in our vertex arrays, put it there
							if (!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								m_totalVerts++;   // We created a new vertex
								Indices.push_back(m_totalVerts - 1);  // Set index for this vertex
							}

							// If this is the very first vertex in the face, we need to
							// make sure the rest of the triangles use this vertex
							if (i == 0)
							{
								firstVIndex = Indices[vIndex];    //The first vertex index of this FACE

							}

							// If this was the last vertex in the first triangle, we will make sure
							// the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
							if (i == 2)
							{
								lastVIndex = Indices[vIndex]; // The last vertex index of this TRIANGLE
							}
							vIndex++;   // Increment index count
						}

						m_meshTriangles++;    // One triangle down

											  // If there are more than three vertices in the face definition, we need to make sure
											  // we convert the face to triangles. We created our first triangle above, now we will
											  // create a new triangle for every new vertex in the face, using the very first vertex
											  // of the face, and the last vertex from the triangle before the current triangle
						for (int l = 0; l < m_triangleCount - 1; ++l)    // Loop through the next vertices to create new triangles
						{
							// First vertex of this triangle (the very first vertex of the face too)
							Indices.push_back(firstVIndex);           // Set index for this vertex
							vIndex++;

							// Second Vertex of this triangle (the last vertex used in the tri before this one)
							Indices.push_back(lastVIndex);            // Set index for this vertex
							vIndex++;

							// Get the third vertex for this triangle
							ss >> VertDef;

							std::wstring vertPart;
							int whichPart = 0;

							// Parse this string (same as above)
							for (size_t j = 0; j < VertDef.length(); ++j)
							{
								if (VertDef[j] != '/')
									vertPart += VertDef[j];
								if (VertDef[j] == '/' || j == VertDef.length() - 1)
								{
									std::wistringstream wstringToInt(vertPart);

									if (whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										// Check to see if the vert pos was the only thing specified
										if (j == VertDef.length() - 1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if (whichPart == 1)
									{
										if (vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if (j == VertDef.length() - 1)
											vertNormIndexTemp = 0;

									}
									else if (whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;
								}
							}

							// Check for duplicate vertices
							bool vertAlreadyExists = false;
							if (m_totalVerts >= 3) // Make sure we at least have one triangle to check
							{
								for (int iCheck = 0; iCheck < m_totalVerts; ++iCheck)
								{
									if (vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if (vertTCIndexTemp == vertTCIndex[iCheck])
										{
											Indices.push_back(iCheck);        // Set index for this vertex
											vertAlreadyExists = true;       // If we've made it here, the vertex already exists
										}
									}
								}
							}

							if (!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								m_totalVerts++;                       // New vertex created, add to total verts
								Indices.push_back(m_totalVerts - 1);  // Set index for this vertex
							}

							// Set the second vertex for the next triangle to the last vertex we got        
							lastVIndex = Indices[vIndex]; // The last vertex index of this TRIANGLE

							m_meshTriangles++;    // New triangle defined
							vIndex++;
						}
					}
				}
				break;
			case 'm': // mtllib - material library filename
				checkChar = fileIn.get();
				if (checkChar == 't')
				{
					checkChar = fileIn.get();
					if (checkChar == 'l')
					{
						checkChar = fileIn.get();
						if (checkChar == 'l')
						{
							checkChar = fileIn.get();
							if (checkChar == 'i')
							{
								checkChar = fileIn.get();
								if (checkChar == 'b')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										// Store the material libraries file name
										fileIn >> meshMatLib;
									}
								}
							}
						}
					}
				}

				break;

			case 'u': // usemtl - which material to use
				checkChar = fileIn.get();
				if (checkChar == 's')
				{
					checkChar = fileIn.get();
					if (checkChar == 'e')
					{
						checkChar = fileIn.get();
						if (checkChar == 'm')
						{
							checkChar = fileIn.get();
							if (checkChar == 't')
							{
								checkChar = fileIn.get();
								if (checkChar == 'l')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										meshMaterialsTemp = "";  // Make sure this is cleared

										fileIn >> meshMaterialsTemp; // Get next type (string)

										meshMaterials.push_back(meshMaterialsTemp);
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}
	else    // If we could not open the file
	{
		// SwapChain->SetFullscreenState(false, NULL); // Make sure we are out of fullscreen

		// create message
		std::string message = "Could not open obj file: ";
		message += Filename;

		MessageBox(0, message.c_str(),  // Display message
			"Error", MB_OK);

		return false;
	}

	SubsetIndexStart.push_back(vIndex); // There won't be another index start after our last subset, so set it here

										// sometimes "g" is defined at the very top of the file, then again before the first group of faces.
										// This makes sure the first subset does not conatain "0" indices.
	if (SubsetIndexStart[1] == 0)
	{
		SubsetIndexStart.erase(SubsetIndexStart.begin() + 1);
		Subsets--;
	}

	// Make sure we have a default for the tex coord and normal
	// if one or both are not specified
	if (!hasNorm)
		vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	if (!hasTexCoord)
		vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));


	// Close the obj file, and open the mtl file
	fileIn.close();

	string meshMatLibPath = "Model/";
	meshMatLibPath += meshMatLib;
	fileIn.open(meshMatLibPath.c_str());

	std::wstring lastStringRead;
	int matCount = m_material.size(); // Total materials

	if (fileIn)
	{
		while (fileIn)
		{
			checkChar = fileIn.get();   // Get next char

			switch (checkChar)
			{
				// Check for comment
			case '#':
				checkChar = fileIn.get();
				while (checkChar != '\n')
					checkChar = fileIn.get();
				break;

				// Set the colors
			case 'K':
				checkChar = fileIn.get();
				if (checkChar == 'd')  // Diffuse Color
				{
					checkChar = fileIn.get();   // Remove space

					fileIn >> m_material[matCount - 1].Diffuse.x;
					fileIn >> m_material[matCount - 1].Diffuse.y;
					fileIn >> m_material[matCount - 1].Diffuse.z;
				}

				if (checkChar == 'a')  // Ambient Color
				{
					checkChar = fileIn.get();   // Remove space

					fileIn >> m_material[matCount - 1].Ambient.x;
					fileIn >> m_material[matCount - 1].Ambient.y;
					fileIn >> m_material[matCount - 1].Ambient.z;
				}

				if (checkChar == 's')  // Ambient Color
				{
					checkChar = fileIn.get();   // Remove space

					fileIn >> m_material[matCount - 1].Specular.x;
					fileIn >> m_material[matCount - 1].Specular.y;
					fileIn >> m_material[matCount - 1].Specular.z;
				}
				break;

			case 'N':
				checkChar = fileIn.get();

				if (checkChar == 's')  // Specular Power (Coefficient)
				{
					checkChar = fileIn.get();   // Remove space

					fileIn >> m_material[matCount - 1].Specular.w;
				}

				break;

				// Check for transparency
			case 'T':
				checkChar = fileIn.get();
				if (checkChar == 'r')
				{
					checkChar = fileIn.get();   // Remove space
					float Transparency;
					fileIn >> Transparency;

					m_material[matCount - 1].Diffuse.w = Transparency;

					if (Transparency > 0.0f)
						m_material[matCount - 1].IsTransparent = true;
				}
				break;

				// Some obj files specify d for transparency
			case 'd':
				checkChar = fileIn.get();
				if (checkChar == ' ')
				{
					float Transparency;
					fileIn >> Transparency;

					// 'd' - 0 being most transparent, and 1 being opaque, opposite of Tr
					Transparency = 1.0f - Transparency;

					m_material[matCount - 1].Diffuse.w = Transparency;

					if (Transparency > 0.0f)
						m_material[matCount - 1].IsTransparent = true;
				}
				break;

				// Get the diffuse map (texture)
			case 'm':
				checkChar = fileIn.get();
				if (checkChar == 'a')
				{
					checkChar = fileIn.get();
					if (checkChar == 'p')
					{
						checkChar = fileIn.get();
						if (checkChar == '_')
						{
							// map_Kd - Diffuse map
							checkChar = fileIn.get();
							if (checkChar == 'K')
							{
								checkChar = fileIn.get();
								if (checkChar == 'd')
								{
									std::string fileNamePath;

									fileIn.get();   // Remove whitespace between map_Kd and file

													// Get the file path - We read the pathname char by char since
													// pathnames can sometimes contain spaces, so we will read until
													// we find the file extension
									bool texFilePathEnd = false;
									while (!texFilePathEnd)
									{
										checkChar = fileIn.get();

										fileNamePath += checkChar;

										if (checkChar == '.')
										{
											for (int i = 0; i < 3; ++i)
												fileNamePath += fileIn.get();

											texFilePathEnd = true;
										}
									}

									//check if this texture has already been loaded
									bool alreadyLoaded = false;

									int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
									if (index >= 0)
									{
										alreadyLoaded = true;
										m_material[matCount - 1].DiffuseTextureID = index;
										m_material[matCount - 1].HasDiffTexture = true;
									}


									//if the texture is not already loaded, load it now
									if (!alreadyLoaded)
									{
										ResMgr->LoadTextureResource(device, fileNamePath.c_str());
										int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
										if (index >= 0)
										{
											m_material[matCount - 1].DiffuseTextureID = index;
											m_material[matCount - 1].HasDiffTexture = true;
										}
									}
								}

								// Get Ambient Map (texture)
								if (checkChar == 'a')
								{
									std::string fileNamePath;

									fileIn.get();   // Remove whitespace between map_Kd and file

													// Get the file path - We read the pathname char by char since
													// pathnames can sometimes contain spaces, so we will read until
													// we find the file extension
									bool texFilePathEnd = false;
									while (!texFilePathEnd)
									{
										checkChar = fileIn.get();

										fileNamePath += checkChar;

										if (checkChar == '.')
										{
											for (int i = 0; i < 3; ++i)
												fileNamePath += fileIn.get();

											texFilePathEnd = true;
										}
									}

									//check if this texture has already been loaded
									bool alreadyLoaded = false;
									int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
									if (index >= 0)
									{
										alreadyLoaded = true;
										m_material[matCount - 1].AmbientTextureID = index;
										m_material[matCount - 1].HasAmbientTexture = true;
									}

									//if the texture is not already loaded, load it now
									if (!alreadyLoaded)
									{
										ResMgr->LoadTextureResource(device, fileNamePath.c_str());
										int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
										if (index >= 0)
										{
											m_material[matCount - 1].AmbientTextureID = index;
											m_material[matCount - 1].HasAmbientTexture = true;
										}
									}
								}

								// Get Specular Map (texture)
								if (checkChar == 's')
								{
									std::string fileNamePath;

									fileIn.get();   // Remove whitespace between map_Ks and file

													// Get the file path - We read the pathname char by char since
													// pathnames can sometimes contain spaces, so we will read until
													// we find the file extension
									bool texFilePathEnd = false;
									while (!texFilePathEnd)
									{
										checkChar = fileIn.get();

										fileNamePath += checkChar;

										if (checkChar == '.')
										{
											for (int i = 0; i < 3; ++i)
												fileNamePath += fileIn.get();

											texFilePathEnd = true;
										}
									}

									//check if this texture has already been loaded
									bool alreadyLoaded = false;
									int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
									if (index >= 0)
									{
										alreadyLoaded = true;
										m_material[matCount - 1].SpecularTextureID = index;
										m_material[matCount - 1].HasSpecularTexture = true;
									}

									//if the texture is not already loaded, load it now
									if (!alreadyLoaded)
									{
										ResMgr->LoadTextureResource(device, fileNamePath.c_str());
										int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
										if (index >= 0)
										{
											m_material[matCount - 1].SpecularTextureID = index;
											m_material[matCount - 1].HasSpecularTexture = true;
										}
									}
								}
							}

							//map_d - alpha map
							else if (checkChar == 'd')
							{
								std::string fileNamePath;

								fileIn.get();   // Remove whitespace between map_Ks and file

												// Get the file path - We read the pathname char by char since
												// pathnames can sometimes contain spaces, so we will read until
												// we find the file extension
								bool texFilePathEnd = false;
								while (!texFilePathEnd)
								{
									checkChar = fileIn.get();

									fileNamePath += checkChar;

									if (checkChar == '.')
									{
										for (int i = 0; i < 3; ++i)
											fileNamePath += fileIn.get();

										texFilePathEnd = true;
									}
								}

								//check if this texture has already been loaded
								bool alreadyLoaded = false;
								int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
								if (index >= 0)
								{
									alreadyLoaded = true;
									m_material[matCount - 1].AlphaTextureID = index;
									m_material[matCount - 1].IsTransparent = true;
								}


								//if the texture is not already loaded, load it now
								if (!alreadyLoaded)
								{
									ResMgr->LoadTextureResource(device, fileNamePath.c_str());
									int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
									if (index >= 0)
									{
										m_material[matCount - 1].AlphaTextureID = index;
										m_material[matCount - 1].IsTransparent = true;
									}
								}
							}

							// map_bump - bump map (Normal Map)
							else if (checkChar == 'b')
							{
								checkChar = fileIn.get();
								if (checkChar == 'u')
								{
									checkChar = fileIn.get();
									if (checkChar == 'm')
									{
										checkChar = fileIn.get();
										if (checkChar == 'p')
										{
											std::string fileNamePath;

											fileIn.get();   // Remove whitespace between map_bump and file

															// Get the file path - We read the pathname char by char since
															// pathnames can sometimes contain spaces, so we will read until
															// we find the file extension
											bool texFilePathEnd = false;
											while (!texFilePathEnd)
											{
												checkChar = fileIn.get();

												fileNamePath += checkChar;

												if (checkChar == '.')
												{
													for (int i = 0; i < 3; ++i)
														fileNamePath += fileIn.get();

													texFilePathEnd = true;
												}
											}

											//check if this texture has already been loaded
											bool alreadyLoaded = false;
											int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
											if (index >= 0)
											{
												alreadyLoaded = true;
												m_material[matCount - 1].NormMapTextureID = index;
												m_material[matCount - 1].HasNormMap = true;

											}

											//if the texture is not already loaded, load it now
											if (!alreadyLoaded)
											{
												ResMgr->LoadTextureResource(device, fileNamePath.c_str());
												int index = ResMgr->GetTextureIndexByName(fileNamePath.c_str());
												if (index >= 0)
												{
													m_material[matCount - 1].NormMapTextureID = index;
													m_material[matCount - 1].HasNormMap = true;
												}
											}
										}
									}
								}
							}
						}
					}
				}
				break;

			case 'n': // newmtl - Declare new material
				checkChar = fileIn.get();
				if (checkChar == 'e')
				{
					checkChar = fileIn.get();
					if (checkChar == 'w')
					{
						checkChar = fileIn.get();
						if (checkChar == 'm')
						{
							checkChar = fileIn.get();
							if (checkChar == 't')
							{
								checkChar = fileIn.get();
								if (checkChar == 'l')
								{
									checkChar = fileIn.get();
									if (checkChar == ' ')
									{
										// New material, set its defaults
										SurfaceMaterial tempMat;
										m_material.push_back(tempMat);
										fileIn >> m_material[matCount].MatName;
										m_material[matCount].IsTransparent = false;
										m_material[matCount].HasDiffTexture = false;
										m_material[matCount].HasAmbientTexture = false;
										m_material[matCount].HasSpecularTexture = false;
										m_material[matCount].HasAlphaTexture = false;
										m_material[matCount].HasNormMap = false;
										m_material[matCount].NormMapTextureID = 0;
										m_material[matCount].DiffuseTextureID = 0;
										m_material[matCount].AlphaTextureID = 0;
										m_material[matCount].SpecularTextureID = 0;
										m_material[matCount].AmbientTextureID = 0;
										m_material[matCount].Specular = XMFLOAT4(0, 0, 0, 0);
										m_material[matCount].Ambient = XMFLOAT3(0, 0, 0);
										m_material[matCount].Diffuse = XMFLOAT4(0, 0, 0, 0);
										matCount++;
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}
	else    // If we could not open the material library
	{
		// SwapChain->SetFullscreenState(false, NULL); // Make sure we are out of fullscreen


		// there might not be mtl file in the obj
		std::string message = "Could not open Mtl File: ";
		message += meshMatLib;

		MessageBox(0, message.c_str(),
			"Error", MB_OK);

		// return false;
	}


	// Set the subsets material to the index value
	// of the its material in our material array
	for (int i = 0; i < Subsets; ++i)
	{
		bool hasMat = false;
		for (size_t j = 0; j < m_material.size(); ++j)
		{
			if (meshMaterials[i] == m_material[j].MatName)
			{
				SubsetMaterialID.push_back(j);
				hasMat = true;
			}
		}
		if (!hasMat)
			SubsetMaterialID.push_back(0); // Use first material in array
	}

	VertexBuffer::VertexType tempVert;

	// Create our vertices using the information we got 
	// from the file and store them in a vector
	for (int j = 0; j < m_totalVerts; ++j)
	{
		tempVert.position = vertPos[vertPosIndex[j]];
		tempVert.normal = vertNorm[vertNormIndex[j]];
		tempVert.texCoord = vertTexCoord[vertTCIndex[j]];

		vertices.push_back(tempVert);
		vertexPosition.push_back(tempVert.position);
	}

	//If computeNormals was set to true then we will create our own
	//normals, if it was set to false we will use the obj files normals
	if (ComputeNormals)
	{
		std::vector<XMFLOAT3> tempNormal;

		//normalized and unnormalized normals
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

		//tangent stuff
		std::vector<XMFLOAT3> tempTangent;
		XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float tcU1, tcV1, tcU2, tcV2;

		//Used to get vectors (sides) from the position of the verts
		float vecX, vecY, vecZ;

		//Two edges of our triangle
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Compute face normals
		//And Tangents
		for (int i = 0; i < m_meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,2)
			vecX = vertices[Indices[(i * 3)]].position.x - vertices[Indices[(i * 3) + 2]].position.x;
			vecY = vertices[Indices[(i * 3)]].position.y - vertices[Indices[(i * 3) + 2]].position.y;
			vecZ = vertices[Indices[(i * 3)]].position.z - vertices[Indices[(i * 3) + 2]].position.z;
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our first edge

															//Get the vector describing another edge of our triangle (edge 2,1)
			vecX = vertices[Indices[(i * 3) + 2]].position.x - vertices[Indices[(i * 3) + 1]].position.x;
			vecY = vertices[Indices[(i * 3) + 2]].position.y - vertices[Indices[(i * 3) + 1]].position.y;
			vecZ = vertices[Indices[(i * 3) + 2]].position.z - vertices[Indices[(i * 3) + 1]].position.z;
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //Create our second edge

															//Cross multiply the two edge vectors to get the un-normalized face normal
			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

			tempNormal.push_back(unnormalized);

			//Find first texture coordinate edge 2d vector
			tcU1 = vertices[Indices[(i * 3)]].texCoord.x - vertices[Indices[(i * 3) + 2]].texCoord.x;
			tcV1 = vertices[Indices[(i * 3)]].texCoord.y - vertices[Indices[(i * 3) + 2]].texCoord.y;

			//Find second texture coordinate edge 2d vector
			tcU2 = vertices[Indices[(i * 3) + 2]].texCoord.x - vertices[Indices[(i * 3) + 1]].texCoord.x;
			tcV2 = vertices[Indices[(i * 3) + 2]].texCoord.y - vertices[Indices[(i * 3) + 1]].texCoord.y;

			//Find tangent using both tex coord edges and position edges
			tangent.x = (tcV1 * XMVectorGetX(edge1) - tcV2 * XMVectorGetX(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
			tangent.y = (tcV1 * XMVectorGetY(edge1) - tcV2 * XMVectorGetY(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
			tangent.z = (tcV1 * XMVectorGetZ(edge1) - tcV2 * XMVectorGetZ(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));

			tempTangent.push_back(tangent);
		}

		//Compute vertex normals (normal Averaging)
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		int facesUsing = 0;
		float tX, tY, tZ;   //temp axis variables

							//Go through each vertex
		for (int i = 0; i < m_totalVerts; ++i)
		{
			//Check which triangles use this vertex
			for (int j = 0; j < m_meshTriangles; ++j)
			{
				if (Indices[j * 3] == i ||
					Indices[(j * 3) + 1] == i ||
					Indices[(j * 3) + 2] == i)
				{
					tX = XMVectorGetX(normalSum) + tempNormal[j].x;
					tY = XMVectorGetY(normalSum) + tempNormal[j].y;
					tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

					normalSum = XMVectorSet(tX, tY, tZ, 0.0f);  //If a face is using the vertex, add the unormalized face normal to the normalSum

																//We can reuse tX, tY, tZ to sum up tangents
					tX = XMVectorGetX(tangentSum) + tempTangent[j].x;
					tY = XMVectorGetY(tangentSum) + tempTangent[j].y;
					tZ = XMVectorGetZ(tangentSum) + tempTangent[j].z;

					tangentSum = XMVectorSet(tX, tY, tZ, 0.0f); //sum up face tangents using this vertex

					facesUsing++;
				}
			}

			//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
			normalSum = normalSum / facesUsing;
			tangentSum = tangentSum / facesUsing;

			//Normalize the normalSum vector and tangent
			normalSum = XMVector3Normalize(normalSum);
			tangentSum = XMVector3Normalize(tangentSum);

			//Store the normal and tangent in our current vertex
			vertices[i].normal.x = XMVectorGetX(normalSum);
			vertices[i].normal.y = XMVectorGetY(normalSum);
			vertices[i].normal.z = XMVectorGetZ(normalSum);

			vertices[i].tangent.x = XMVectorGetX(tangentSum);
			vertices[i].tangent.y = XMVectorGetY(tangentSum);
			vertices[i].tangent.z = XMVectorGetZ(tangentSum);

			//Clear normalSum, tangentSum and facesUsing for next vertex
			normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			facesUsing = 0;

		}
	}

	CreateAABB();

	return true;
}


void EntityMesh::Update()
{
	//Entity::Update();
}

void EntityMesh::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projMatrix)
{
	//Set Vertex and Pixel Shaders
	//	m_shader->Begin(deviceContext, m_vertexBuffer->GetIndexCount(), m_vertexBuffer->GetStartIndexLocation());

	//	unsigned int stride  = sizeof(VertexBuffer::VertexType), offset = 0;

	/////Draw NON-transparent subsets/////
	for (int i = 0; i < Subsets; ++i)
	{
		if (!m_material[i].IsTransparent)
		{
			//Set the grounds index buffer
			//	deviceContext->IASetIndexBuffer(m_vertexBuffer->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
			//Set the grounds vertex buffer
			//	ID3D11Buffer* vertexBuffer = m_vertexBuffer->GetVertexBuffer();
			//		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
			//	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//Set the WVP matrix and send it to the constant buffer in effect file
			m_vertexBuffer->Render(deviceContext);
			m_shader->SetShaderParameter(deviceContext, m_worldMatrix, viewMatrix, projMatrix);
			//temp
			//MeshShader* mesh_shader = (MeshShader*)m_shader;
			//mesh_shader->SetShaderParameter(deviceContext, XMFLOAT4(1, 1, 1, 1));

			// If this subset has a diffuse texture, send it to the pixel shader
			if (m_material[SubsetMaterialID[i]].HasDiffTexture)
			{
				Texture* diffuseTexture = ResMgr->GetTextureByIndex(m_material[SubsetMaterialID[i]].DiffuseTextureID);
				if (diffuseTexture)
					m_shader->SetShaderParameter(deviceContext, diffuseTexture->GetTexture());
			}


			// If this subset has a normal (bump) map, send it to the pixel shader
			if (m_material[SubsetMaterialID[i]].HasNormMap)
			{
				Texture* diffuseTexture = ResMgr->GetTextureByIndex(m_material[SubsetMaterialID[i]].NormMapTextureID);
				if (diffuseTexture)
					m_shader->SetShaderParameter(deviceContext, diffuseTexture->GetTexture(), 1);
			}

			int indexStart = SubsetIndexStart[i];
			int indexDrawAmount = SubsetIndexStart[i + 1] - indexStart;
			deviceContext->DrawIndexed(indexDrawAmount, indexStart, 0);
		}
	}

}

void EntityMesh::CreateAABB()
{
	// Create Axis-Aligned Bounding Box (AABB)
	XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	BoundingSphere = 0;

	for (UINT i = 0; i < vertexPosition.size(); i++)
	{
		// The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
		// that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
		// covered by the bounding volume

		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, vertexPosition[i].x);	// Find smallest x value in model
		minVertex.y = min(minVertex.y, vertexPosition[i].y);	// Find smallest y value in model
		minVertex.z = min(minVertex.z, vertexPosition[i].z);	// Find smallest z value in model

																//Get the largest vertex 
		maxVertex.x = max(maxVertex.x, vertexPosition[i].x);	// Find largest x value in model
		maxVertex.y = max(maxVertex.y, vertexPosition[i].y);	// Find largest y value in model
		maxVertex.z = max(maxVertex.z, vertexPosition[i].z);	// Find largest z value in model
	}

	// Our AABB [0] is the min vertex and [1] is the max
	AABB.push_back(minVertex);
	AABB.push_back(maxVertex);

	// Get models true center
	Center.x = maxVertex.x - minVertex.x / 2.0f;
	Center.y = maxVertex.y - minVertex.y / 2.0f;
	Center.z = maxVertex.z - minVertex.z / 2.0f;

	// Now that we have the center, get the bounding sphere	
	for (UINT i = 0; i < vertexPosition.size(); i++)
	{
		float x = (Center.x - vertexPosition[i].x) * (Center.x - vertexPosition[i].x);
		float y = (Center.y - vertexPosition[i].y) * (Center.y - vertexPosition[i].y);
		float z = (Center.z - vertexPosition[i].z) * (Center.z - vertexPosition[i].z);

		// Get models bounding sphere
		BoundingSphere = max(BoundingSphere, (x + y + z));
	}

	// We didn't use the square root when finding the largest distance since it slows things down.
	// We can square root the answer from above to get the actual bounding sphere now
	BoundingSphere = sqrt(BoundingSphere);

}

