#include "Engine.h"
#include "SystemDefs.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "Input.h"
#include "GameComponent.h"
#include "TextureShader.h"
#include "MeshShader.h"
#include "Camera.h"

Engine* Engine::m_instance = nullptr;

Engine::Engine()
{
	m_fps = m_frameCount = 0;
	m_graphics = nullptr;
	m_resourceManager = nullptr;
	m_entityManager = nullptr;
	m_input = nullptr;
	m_camera = nullptr;
	m_gameComponent = nullptr;
}

Engine::~Engine()
{
	SafeDelete(m_graphics);

	SafeDelete(m_resourceManager);

	SafeDelete(m_entityManager);

	SafeDelete(m_input);

	SafeDelete(m_camera);

	SafeDelete(m_gameComponent);
}


bool Engine::InitializeGraphics(HWND hwnd)
{
	m_graphics = new Graphics();

	return m_graphics->InitializeDX(hwnd);
}

bool Engine::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	m_resourceManager = ResMgr;
	m_resourceManager->LoadTextureResource(m_graphics->GetDevice(), "Texture/grassnormal.jpg");
	m_resourceManager->LoadShaderResource(new TextureShader(m_graphics->GetDevice(), hwnd, "Shader/texture", "TextureVertexShader", "TexturePixelShader"));
	m_resourceManager->LoadShaderResource(new MeshShader(m_graphics->GetDevice(), hwnd, "Shader/basic", "VSMain", "PSMain"));
	m_resourceManager->LoadShaderResource(new Shader(m_graphics->GetDevice(), hwnd, "Shader/Sky", "SkyVSMain", "SkyPSMain"));

	if (m_graphics)
		m_graphics->Initialize(); 

	// Create the camera object.
	m_camera = new Camera();
	if (!m_camera)
	{
		return false;
	}

	if (m_camera)
		m_camera->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_input = new Input();
	if (!m_input->Initialize(hInstance, hwnd, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return false;
	}

	m_input->Update();

	m_entityManager = EntMgr;
	//entity initialize

	if (m_gameComponent)
	{
		if (!m_gameComponent->Initialize())
		{
			cout << "Couldn't GameComponent Initialize " << endl;
			return false;
		}
			
	}
	else
	{
		cout << "No Game Component" << endl;
	}
	
	return true;
}

void Engine::Run()
{
	Update();
	Render();
}

void Engine::Update()
{
	//calculate frame
	CalculateFrame();

	//cout << "Frame Time : " << m_frameTime << " FPS : " << m_fps << endl;
	//entity update
	if(m_gameComponent)
		m_gameComponent->Update();

	m_input->Update();
	m_camera->Update(m_frameTime);


	m_entityManager->Update();
	
}

void Engine::Render()
{
	if (m_graphics)
	{
		m_graphics->BeginScene(0.f, 0.f, 1.f, 1.f);

		//render stuff goes here

		XMFLOAT4X4 viewMatrix, projMatrix;
		m_camera->GetViewMatrix(viewMatrix);
		m_camera->GetProjMatrix(projMatrix);

		if (m_gameComponent)
			m_gameComponent->Render(m_graphics->GetDeviceContext(), viewMatrix, projMatrix);

		//entity render
		m_entityManager->Render(m_graphics->GetDeviceContext(), viewMatrix, projMatrix);


		//render Sky

		//render UI

		//render Text
		m_graphics->EndScene();
	}
}

void Engine::CalculateFrame()
{
	//calculate FPS
	m_frameCount++;
	if (m_Timer.EndTimer() > 1.0)
	{
		m_fps = m_frameCount;
		m_frameCount = 0;
		m_Timer.StartTimer();
	}

	double currentTime = m_Timer.GetTime();
	m_frameTime = currentTime - m_frameTimeOld;
	m_frameTimeOld = currentTime;
}

void Engine::Release()
{
	SafeDelete(m_instance);
}

Engine * Engine::GetEngine()
{
	if (m_instance == nullptr)
		m_instance = new Engine();

	return m_instance;
}

void Engine::SetGameComponent(GameComponent * gameComponent)
{
	m_gameComponent = gameComponent;
}
