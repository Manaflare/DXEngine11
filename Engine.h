#pragma once

#include "Graphics.h"
#include "NANATimer.h"
class ResourceManager;
class EntityManager;
class Input;
class GameComponent;
class TextureShader;
class Camera;

class Engine
{
public:
	~Engine();

	bool InitializeGraphics(HWND hwnd);
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	void Run();

	void Release();

	Input* GetInput() const { return m_input;  }
	Graphics* GetGraphics() const { return m_graphics; }
	Camera* GetCamera() const { return m_camera; }
	int GetFPS() const { return m_fps;  }

	static Engine* GetEngine();


	void SetGameComponent(GameComponent* gameComponent);

private:
	Engine();

	void CalculateFrame();
	void Update();
	void Render();

	Graphics* m_graphics;
	static Engine* m_instance;
	Camera* m_camera;
	NANATimer m_Timer;
	Input* m_input;
	int m_fps;
	int m_frameCount;
	double m_frameTime;
	double m_frameTimeOld;

	ResourceManager* m_resourceManager;
	EntityManager*   m_entityManager;
	GameComponent* m_gameComponent;

};

