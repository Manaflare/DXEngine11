#pragma once
#include <DirectXMath.h>

template <class T>
void SafeRelease(T ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}

}


template <class T>
void SafeDelete(T ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}

}

template <class T>
void SafeDeleteArray(T ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}

}

static UINT32 ConvertVector4ToUINT(DirectX::XMFLOAT4 color)
{
	int r = static_cast<int>(color.x * 255.f);
	int g = static_cast<int>(color.y * 255.f);
	int b = static_cast<int>(color.z * 255.f);
	int a = static_cast<int>(color.w * 255.f);

	UINT32 colorInt = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | ( b );
	return colorInt;
}

static DirectX::XMFLOAT4 ConvertUINTToVector4(UINT colorInt)
{
	DirectX::XMFLOAT4 color;
	color.w = static_cast<float>((colorInt >> 24) & 0xFF);
	color.x = static_cast<float>((colorInt >> 16) & 0xFF);
	color.y = static_cast<float>((colorInt >> 8) & 0xFF);
	color.z = static_cast<float>(colorInt & 0xFF);

	return color;
}