#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

bool LoadOBJ(const char* path, std::vector<XMFLOAT3>& outVecVerticies, std::vector<XMFLOAT3>& outVecUvs, std::vector<XMFLOAT3>& outVecNormal);