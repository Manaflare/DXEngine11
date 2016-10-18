#pragma once

class IFW1Factory;
class IFW1FontWrapper;

class FontManager
{
public:

	struct FontType
	{
		int id;
		string text;
		int posX, posY;
		float size;
		XMFLOAT4 color;

		FontType()
			: text("")
			, id(0)
			, posX(0)
			, posY(0)
			, size(32.f)	//default size
			, color(XMFLOAT4(1.f, 1.f, 1.f, 1.f))	//default color
		{

		}
	};

	FontManager();
	~FontManager();


	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const wchar_t* FontName);
	void Release();

	int AddText(FontType fontType);
	void RemoveText(string strText);
	void RemoveText(int index);
	void RemoveAllText();

	bool CheckText(FontType newFont);

	void Render();

	static FontManager* GetInstance()
	{
		if (m_instance == nullptr)
			m_instance = new FontManager();

		return m_instance;
	}

private:
	static FontManager* m_instance;

	IFW1Factory  *m_fontFactory;
	IFW1FontWrapper* m_fontWrapper;
	ID3D11DeviceContext* m_deviceContext;

	vector<FontType> m_vecText;
};

#define FontMgr FontManager::GetInstance()

