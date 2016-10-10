#include "Framework.h"
#include "SystemDefs.h"
#include "Input.h"
#include "GameComponent.h"
#include <iostream>

using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

Framework::Framework()
{
}


Framework::~Framework()
{
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	Engine::GetEngine()->Release();


	UnregisterClass(m_applicationName, m_hInstance);
	m_hInstance = nullptr;
}

bool Framework::Initialize(GameComponent* gameComponent)
{
	if (!CreateDXWindow("DxEngine", WINDOWS_POSX, WINDOWS_POSY, SCREEN_WIDTH, SCREEN_HEIGHT))
		return false;

	Engine::GetEngine()->SetGameComponent(gameComponent);

	if (!Engine::GetEngine()->Initialize(m_hInstance, Engine::GetEngine()->GetGraphics()->GetHwnd()))
	{
		return false;
	}


	return true;
}

void Framework::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//update loop
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//update & render function
			Engine::GetEngine()->Run();
		}
	}

}


bool Framework::CreateDXWindow(char* windowTitle, int x, int y, int widht, int height)
{
	HWND hwnd;
	WNDCLASSEX wc;
	m_applicationName = windowTitle;

	m_hInstance = GetModuleHandle(NULL);

	//setup the window class with default setting;
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszClassName	= m_applicationName;
	wc.lpszMenuName		= NULL;
	wc.cbSize			= sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "RegisterClass is failed", "Error", MB_OK);
		return false;
	}

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		DEVMODE dmScreenSettings;
		ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		screenWidth = widht;
		screenHeight = height;
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, windowTitle, windowTitle, nStyle, x, y, widht, height, NULL, NULL, m_hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "CreateWindowEx failed", "Error", MB_OK);
		Engine::GetEngine()->Release();
		PostQuitMessage(0);
		return false;
	}

	if (!Engine::GetEngine()->InitializeGraphics(hwnd))
	{
		MessageBox(NULL, "Could not initialize DX 11", "Error", MB_OK);
		Engine::GetEngine()->Release();
		PostQuitMessage(0);
		UnregisterClass(m_applicationName, m_hInstance);
		m_hInstance = nullptr;
		DestroyWindow(hwnd);
		return false;
	}

	Engine::GetEngine()->GetGraphics()->SetHwnd(hwnd);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	Input* input = Engine::GetEngine()->GetInput();
	if(input && input->IsKeyDown(DIK_ESCAPE))
	{
		PostQuitMessage(0);
		DestroyWindow(hwnd);
		return 0;
	}

	switch (message)
	{
		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
				DestroyWindow(hwnd);
			}
		}
		break;
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}
		break;
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			DestroyWindow(hwnd);
		}
		break;
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

	}

	return 0;
}