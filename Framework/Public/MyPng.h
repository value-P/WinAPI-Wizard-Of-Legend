#pragma once
class CMyPng
{
public:
	CMyPng();
	~CMyPng();

public:	
	void	Load_Png(const TCHAR* pFilePath);
	void	Release();
	HDC		Get_MemDC() { return m_hMemDC; }
	Image* Get_pImage() { return m_pImage; }

public:
	HDC		m_hMemDC;
	Image*	m_pImage;
};

