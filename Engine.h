#pragma once

#include "Graphics.h"
#include "Sprite.h"


class ResourceManager;
class EntityManager;
class Input;
class GameComponent;
class TextureShader;

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
	static Engine* GetEngine();


	void SetGameComponent(GameComponent* gameComponent);

private:
	Engine();

	void Update();
	void Render();

	Graphics* m_graphics;
	static Engine* m_instance;


	Input* m_input;


	ResourceManager* m_resourceManager;
	EntityManager*   m_entityManager;
	GameComponent* m_gameComponent;

};

