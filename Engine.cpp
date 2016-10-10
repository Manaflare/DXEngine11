#include "Engine.h"
#include "SystemDefs.h"
#include "ResourceManager.h"
#include "EntityManager.h"
#include "Input.h"
#include "GameComponent.h"
Engine* Engine::m_instance = nullptr;

Engine::Engine()
{
	m_graphics = nullptr;
	m_resourceManager = nullptr;
	m_entityManager = nullptr;
	m_input = nullptr;

	m_gameComponent = nullptr;
}

Engine::~Engine()
{
	SafeDelete(m_graphics);

	SafeDelete(m_resourceManager);

	SafeDelete(m_entityManager);

	SafeDelete(m_input);

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
	m_resourceManager->LoadTextureResource(m_graphics->GetDevice(), "Texture/test.png");
	m_resourceManager->LoadShaderResource(new TextureShader(m_graphics->GetDevice(), hwnd, "Shader/texture", "TextureVertexShader", "TexturePixelShader"));
	m_resourceManager->LoadShaderResource(new TextureShader(m_graphics->GetDevice(), hwnd, "Shader/basic", "VSMain", "PSMain"));

	if (m_graphics)
		m_graphics->Initialize(); 

//	TextureShader* textureShader = (TextureShader*)m_resourceManager->GetShaderByName("texture");
//	m_sprite = new Sprite(32.f);
//	m_sprite->Initialize(m_graphics->GetDevice(), textureShader, "test");


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
			return false;
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
	//entity update
	if(m_gameComponent)
		m_gameComponent->Update();

	m_entityManager->Update();
	m_input->Update();
}

void Engine::Render()
{
	if (m_graphics)
	{
		static float r = 0.f;
		if (m_input && m_input->IsKeyDown(DIK_LEFT))
			r += 0.01f;
		if (m_input && m_input->IsKeyDown(DIK_RIGHT))
			r -= 0.01f;

		m_graphics->BeginScene(r, 0.f, 1.f, 1.f);

		//render stuff goes here

		XMFLOAT3 position = XMFLOAT3(0, 0, -100.f);
		XMFLOAT3 up = XMFLOAT3(0, 1, 0);
		XMFLOAT3 lookAt = XMFLOAT3(0, 0, 1);

		XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));

		XMFLOAT4X4 worldMatrix, viewMatrix, projMatrix;
		XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&projMatrix, XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 1000.f));

		if (m_gameComponent)
			m_gameComponent->Render(m_graphics->GetDeviceContext(), viewMatrix, projMatrix);

		//entity render
		m_entityManager->Render(m_graphics->GetDeviceContext(), viewMatrix, projMatrix);

		m_graphics->EndScene();
	}
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
