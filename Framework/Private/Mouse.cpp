#include "stdafx.h"
#include "Mouse.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MOUSE.bmp", L"MOUSE");
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;
}

int CMouse::Update()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	m_tInfo.fX = (float)ptMouse.x;
	m_tInfo.fY = (float)ptMouse.y;
	

	__super::Update_Rect();

	ShowCursor(false);

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{
}

void CMouse::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"MOUSE");

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX ,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CMouse::Release()
{
}

void CMouse::Debug_Render(HDC hDC)
{
}

void CMouse::Anim_Update()
{
}
