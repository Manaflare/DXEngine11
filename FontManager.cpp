#include "Precompiled.h"
#include "FontManager.h"
#include "FW1FontWrapper.h"


//this class is the class that is wrapped FW1FontClass to render text easily

FontManager* FontManager::m_instance = nullptr;

FontManager::FontManager()
{
}


FontManager::~FontManager()
{
	Release();
}

bool FontManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* FontName)
{
	m_deviceContext = deviceContext;
	HRESULT result = FW1CreateFactory(FW1_VERSION, &m_fontFactory);
	if (FAILED(result))
	{
		return false;
	}

	result = m_fontFactory->CreateFontWrapper(device, FontName, &m_fontWrapper);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void FontManager::Release()
{
	m_fontWrapper->Release();
	m_fontFactory->Release();
}

int FontManager::AddText(FontType fontType)
{
	if(CheckText(fontType) == false)
		m_vecText.push_back(fontType);

	return static_cast<int>(m_vecText.size() - 1);
}

void FontManager::RemoveText(string strText)
{
	for (size_t index = 0; index < m_vecText.size(); ++index)
	{
		if (strText == m_vecText.at(index).text)
		{
			m_vecText.erase(m_vecText.begin() + index);
			break;
		}
	}
}

void FontManager::RemoveText(int index)
{
	m_vecText.erase(m_vecText.begin() + index);
}

void FontManager::RemoveAllText()
{
	m_vecText.clear();
}

bool FontManager::CheckText(FontType newFont)
{
	int index = 0;
	for (FontType fontIter : m_vecText)
	{
		//check if they are same as position
		if (fontIter.posX == newFont.posX && fontIter.posY == newFont.posY)
		{
			//If Text is different from previous Text, change the new Text
			if (fontIter.text != newFont.text)
			{
				m_vecText.erase(m_vecText.begin() + index);
				break;
			}
		}
			return true;

		index++;
	}

	return false;
}

void FontManager::Render()
{
	for (FontType fontIter : m_vecText)
	{
		wstring wstr(fontIter.text.begin(), fontIter.text.end());
		m_fontWrapper->DrawString(m_deviceContext, wstr.c_str(), fontIter.size, fontIter.posX, fontIter.posY, ConvertVector4ToUINT(fontIter.color), FW1_RESTORESTATE);
	}

}
