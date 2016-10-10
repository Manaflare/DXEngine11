#include "Texture.h"
#include "SystemDefs.h"

Texture::Texture()
{
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
}


Texture::~Texture()
{
	SafeRelease(m_texture);
	m_name.clear();
}

bool Texture::Initialize(ID3D11Device * device, LPCSTR fileName)
{
	HRESULT result;

	m_name = fileName;
	int pos = m_name.find_last_of("/");
	if (pos >= 0)
	{
		m_name = m_name.substr(pos + 1, m_name.length());
	}

	m_name = m_name.substr(0, m_name.find_last_of("."));

	string str(fileName);
	std::wstring wsTmp(str.begin(), str.end());

	std::wstring ws = wsTmp;
	//load	the texture
	DirectX::ScratchImage image;
	WCHAR ext[_MAX_EXT];
	_wsplitpath_s(ws.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);

	HRESULT hr;
	if (_wcsicmp(ext, L".dds") == 0)
	{
		hr = DirectX::LoadFromDDSFile(ws.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else if (_wcsicmp(ext, L".tga") == 0)
	{
		hr = DirectX::LoadFromTGAFile(ws.c_str(), nullptr, image);
	}
	else
	{
		hr = DirectX::LoadFromWICFile(ws.c_str( ), DirectX::WIC_FLAGS_NONE, nullptr, image);
	}

	DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &m_texture);
	//result = D3DX11CreateShaderResourceViewFromFile(device, fileName, NULL, NULL, &m_texture, NULL);

	ID3D11Resource* resource = nullptr;
	m_texture->GetResource(&resource);

	ID3D11Texture2D* texture2D = nullptr;
	result = resource->QueryInterface(&texture2D);
	if (SUCCEEDED(result))
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		texture2D->GetDesc(&textureDesc);

		m_width = textureDesc.Width;
		m_height = textureDesc.Height;
	}

	texture2D->Release();
	resource->Release();
	
	return true;
}
