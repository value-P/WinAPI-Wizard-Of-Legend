#pragma once
class CMyBitmap
{
public:
	CMyBitmap();
	virtual ~CMyBitmap();

public:
	void	Load_Bmp(wstring pFilePath);
	void	Release();

public:
	HDC		Get_MemDC() { return m_hMemDC; }

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;
};

