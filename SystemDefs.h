#pragma once

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 768
#define WINDOWS_POSX 1000
#define WINDOWS_POSY 85

const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLE = true;

const float PI = 3.14159265358979f;

template <class T>
void SafeRelease(T ptr)
{
	ptr->Release();
	ptr = nullptr;
}


template <class T>
void SafeDelete(T ptr)
{
	delete ptr;
	ptr = nullptr;
}

template <class T>
void SafeDeleteArray(T ptr)
{
	delete [] ptr;
	ptr = nullptr;
}