#pragma once
#include "MyPng.h"

class CPngMgr
{
private:
	CPngMgr();
	~CPngMgr();

public:
	static CPngMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CPngMgr;
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
	void	Insert_Img(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC		Find_Img(const TCHAR* pImgKey);
	void	Release();

private:
	static CPngMgr* m_pInstance;
	map<const TCHAR*, CMyPng*>	m_mapPng;
};

