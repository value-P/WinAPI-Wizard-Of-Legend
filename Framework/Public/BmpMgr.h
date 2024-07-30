#pragma once
#include "MyBitmap.h"

class CBmpMgr
{
private:
	CBmpMgr();
	~CBmpMgr();

public:
	static CBmpMgr*	Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBmpMgr;
		}

		return m_pInstance;
	}
	static void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void	Insert_Bmp(wstring pFilePath, wstring pImgKey);
	HDC		Find_Img(wstring pImgKey);
	void	Release();

private:
	static CBmpMgr* m_pInstance;
	map<wstring, CMyBitmap*>	m_mapBit;
};

