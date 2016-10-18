#include "PreCompiled.h"
#include "DXManager.h"
#include "SystemDefs.h"

DXManager::DXManager() : m_wireframe(false)
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTagetview = nullptr;
	m_depthstencilBuffer = nullptr;
	m_depthstencilState = nullptr;
	m_depthstencilView = nullptr;
	m_rasterizerState = nullptr;
	m_alphaEnabledBlendingState = nullptr;
	m_alphaDisabledBlendingState = nullptr;
	m_depthDisabledstencilState = nullptr;
}


DXManager::~DXManager()
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}

	if (m_alphaDisabledBlendingState)
	{
		m_alphaDisabledBlendingState->Release();
		m_alphaDisabledBlendingState = nullptr;
	}

	if (m_alphaEnabledBlendingState)
	{
		m_alphaEnabledBlendingState->Release();
		m_alphaEnabledBlendingState = nullptr;
	}

	if (m_depthstencilView)
	{
		m_depthstencilView->Release();
		m_depthstencilView = nullptr;
	}

	if (m_depthstencilState)
	{
		m_depthstencilState->Release();
		m_depthstencilState = nullptr;
	}

	if (m_depthstencilBuffer)
	{
		m_depthstencilBuffer->Release();
		m_depthstencilBuffer = nullptr;
	}

	if (m_renderTagetview)
	{
		m_renderTagetview->Release();
		m_renderTagetview = nullptr;
	}

	if (m_rasterizerState)
	{
		m_rasterizerState->Release();
		m_rasterizerState = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}

bool DXManager::Initialize(int screenWidth, int screenHeight, bool vshync, HWND hwnd, bool fullscreen)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;

	unsigned int numModes, numerator = 0, denominator = 0, stringLength;

	DXGI_MODE_DESC* displayModelist;
	DXGI_ADAPTER_DESC adapterdesc;
	int error;
	ID3D11Texture2D* backBufferPtr;

	m_vsync = vshync;

	//create directx interface factory
	result = CreateDXGIFactory(_uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	//use the factory to create an adapter for primary graphics interface
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output
	result = adapter->EnumOutputs(0, &output);
	if (FAILED(result))
	{
		return false;
	}

	//get the number of modes that fit the DXGI_ENUM_MODES_INTERLACED for the adapter output 
	result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		return false;
	}


	//create a list to hold all the possible modes for this monitor/video card combination
	displayModelist = new DXGI_MODE_DESC[numModes];

	//Fill the display mode list
	result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModelist);
	if (FAILED(result))
	{
		return false;
	}

	for (unsigned int i = 0; i < numModes; ++i)
	{
		if (displayModelist[i].Width == (unsigned int)screenWidth)
		{
			if (displayModelist[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModelist[i].RefreshRate.Numerator;
				denominator = displayModelist[i].RefreshRate.Denominator;
			}
		}
	}

	if (numerator == 0 && denominator == 0)
		return false;

	//get the adapter description
	result = adapter->GetDesc(&adapterdesc);
	if (FAILED(result))
	{
		return false;
	}

	//store the video card memory in mbs
	m_videocardMemory = (int)(adapterdesc.DedicatedVideoMemory / 1024 / 1024);

	//convert the name of the video card to a character array
	error = wcstombs_s(&stringLength, m_videocareDescription ,128, adapterdesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	//release memory
	delete[] displayModelist;
	displayModelist = nullptr;

	SafeRelease(output);
	SafeRelease(adapter);
	SafeRelease(factory);


	if (!InitializeSwapChain(hwnd, fullscreen, screenWidth, screenHeight, numerator, denominator))
	{
		return false;
	}

	//Get the pointer of back buffer
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	//create render target view
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTagetview);
	if (FAILED(result))
	{
		return false;
	}

	//release back buffer pointer
	SafeRelease(backBufferPtr);

	if (!InitlalizeDepthBuffer(screenWidth, screenHeight))
	{
		return false;
	}

	if (!InitializeDepthStencilBuffer())
	{
		return false;
	}

	if (!InitializeStencilView())
	{
		return false;
	}

	//bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_renderTagetview, m_depthstencilView);

	if (!InitializeRasterizeState())
	{
		return false;
	}

	if (!InitializeViewport(screenWidth, screenHeight))
	{
		return false;
	}

	if (!InitializeAlphaBlending())
	{
		return false;
	}

	if (!InitializeZBuffer())
	{
		return false;
	}

	return true;
}

void DXManager::BeginScene(float r, float g, float b, float a)
{
	float color[4];

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	//clear back buffer
	m_deviceContext->ClearRenderTargetView(m_renderTagetview, color);

	//clear depth buffer
	m_deviceContext->ClearDepthStencilView(m_depthstencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);


}

void DXManager::EndScene()
{

	if (m_vsync)
	{
		//lock to screen refresh rate
		m_swapChain->Present(1, 0);
	}
	else
	{
		//present as fast as possible
		m_swapChain->Present(0, 0);
	}
}

void DXManager::EnableAlphaBlending(bool bEnable)
{
	float blendFactor[4];
	blendFactor[0] = 0.f;
	blendFactor[1] = 0.f;
	blendFactor[2] = 0.f;
	blendFactor[3] = 0.f;

	if (bEnable)
	{
		m_deviceContext->OMSetBlendState(m_alphaEnabledBlendingState, blendFactor, 0xffffffff);
	}
	else
	{
		m_deviceContext->OMSetBlendState(m_alphaDisabledBlendingState, blendFactor, 0xffffffff);
	}
}

void DXManager::EnableZBuffer(bool bEnable)
{
	if (bEnable)
	{
		m_deviceContext->OMSetDepthStencilState(m_depthstencilState,1);
	}
	else
	{
		m_deviceContext->OMSetDepthStencilState(m_depthDisabledstencilState,1);
	}
}

void DXManager::SwitchFireFrame()
{
	m_wireframe = !m_wireframe;
	if (m_wireframe)
	{
		m_deviceContext->RSSetState(m_wireFramerasterizerState);
	}
	else
	{
		m_deviceContext->RSSetState(m_rasterizerState);
	}
}

ID3D11Device * DXManager::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext * DXManager::GetDeviceContext()
{
	return m_deviceContext;
}

bool DXManager::InitializeSwapChain(HWND hwnd, bool fullscreen, int screenWidth, int screenHeight, unsigned int numerator, unsigned int denominator)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT result;

	//intialize swap chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//set to a single back buffer;
	swapChainDesc.BufferCount = 1;

	//set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	//set regular 32-bit back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//set the refresh rate of back buffer
	if (m_vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//set the usage of back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//set the handle of back buffer
	swapChainDesc.OutputWindow = hwnd;

	//turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	
	//set the fullscreen or window mode
	swapChainDesc.Windowed = !fullscreen;

	//set the scan line ordering and scailing to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

	//set the feature level to dx11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device,NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitlalizeDepthBuffer(int screenWidth, int screenHeight)
{
	D3D11_TEXTURE2D_DESC depthBufferdesc;
	HRESULT result;

	//initialize depth buffer desc
	ZeroMemory(&depthBufferdesc, sizeof(depthBufferdesc));

	//setup the depth buffer desc
	depthBufferdesc.Width = screenWidth;
	depthBufferdesc.Height = screenHeight;
	depthBufferdesc.MipLevels = 1;
	depthBufferdesc.ArraySize = 1;
	depthBufferdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferdesc.SampleDesc.Count = 1;
	depthBufferdesc.SampleDesc.Quality = 0;
	depthBufferdesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferdesc.CPUAccessFlags = 0;
	depthBufferdesc.MiscFlags = 0;

	//create the texture for the depth buffer
	result = m_device->CreateTexture2D(&depthBufferdesc, NULL, &m_depthstencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitializeDepthStencilBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthstencilDesc;
	HRESULT result;

	//initialize depth stencil description
	ZeroMemory(&depthstencilDesc, sizeof(depthstencilDesc));

	//setup the description
	depthstencilDesc.DepthEnable = true;
	depthstencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthstencilDesc.StencilEnable = true;
	depthstencilDesc.StencilReadMask = 0xFF;
	depthstencilDesc.StencilWriteMask = 0xFF;

	//stencil operation if pixel if front-facing
	depthstencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthstencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//stencil operation if pixel if back-facing
	depthstencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthstencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthstencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthstencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create the depth stencil state
	result = m_device->CreateDepthStencilState(&depthstencilDesc, &m_depthstencilState);
	if (FAILED(result))
	{
		return false;
	}

	//set the depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthstencilState, 1);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitializeStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthstencilViewDesc;
	HRESULT result;

	ZeroMemory(&depthstencilViewDesc, sizeof(depthstencilViewDesc));

	//setup the description
	depthstencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthstencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthstencilViewDesc.Texture2D.MipSlice = 0;

	//create depth stencil view
	result = m_device->CreateDepthStencilView(m_depthstencilBuffer, &depthstencilViewDesc, &m_depthstencilView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DXManager::InitializeRasterizeState()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT result;

	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.f;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.f;

	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterizerState);
	if (FAILED(result))
	{
		return false;
	}

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	result = m_device->CreateRasterizerState(&rasterDesc, &m_wireFramerasterizerState);
	if (FAILED(result))
	{
		return false;
	}

	//set the rs state
	m_deviceContext->RSSetState(m_rasterizerState);

	return true;
}

bool DXManager::InitializeViewport(int screenWidth, int screenHeight)
{
	D3D11_VIEWPORT viewport;

	//setup
	viewport.Width = (FLOAT)screenWidth;
	viewport.Height = (FLOAT)screenHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	m_deviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool DXManager::InitializeAlphaBlending()
{
	D3D11_BLEND_DESC blendDesc;
	HRESULT result;

	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	result = m_device->CreateBlendState(&blendDesc, &m_alphaEnabledBlendingState);
	if (FAILED(result))
	{
		return false;
	}


	//Modify to create disabled alpha blend state
	blendDesc.RenderTarget[0].BlendEnable = false;
	m_device->CreateBlendState(&blendDesc, &m_alphaDisabledBlendingState);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}

bool DXManager::InitializeZBuffer()
{
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	HRESULT result;

	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	//setup the description
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	//stencil operation if pixel if front-facing
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//stencil operation if pixel if back-facing
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//create the depth stencil state
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledstencilState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
