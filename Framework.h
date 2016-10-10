#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Engine.h"

class GameComponent;

class Framework
{
public:
	Framework();
	~Framework();

	bool Initialize(GameComponent* gameComponent);
	void Run();

private:
	bool CreateDXWindow(char* windowTitle, int x, int y, int widht, int height);

	char* m_applicationName;
	HINSTANCE m_hInstance;

};

