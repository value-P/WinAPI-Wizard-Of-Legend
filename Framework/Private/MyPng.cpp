#include "stdafx.h"
#include "MyPng.h"

CMyPng::CMyPng()
	:m_pImage(nullptr),m_hMemDC(NULL)
{
}

CMyPng::~CMyPng()
{
	Release();
}

void CMyPng::Load_Png(const TCHAR* pFilePath)
{
	HDC hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	ULONG_PTR gdiplusToken; // 초기화
	GdiplusStartupInput gidplusStartupInput;
	GdiplusStartup(&gdiplusToken,&gidplusStartupInput, NULL);


	// 이미지 불러오기
	m_pImage = Image::FromFile(pFilePath);
	
	Graphics g(m_hMemDC);
	RECT rect{ 0, 0, 16, 16 };
	g.DrawImage(m_pImage,0,0,16,16);
}

void CMyPng::Release()
{
	delete m_pImage;
	m_pImage = nullptr;
}
