#include "stdafx.h"
#include "MiniMap.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"

CMiniMap::CMiniMap()
{
}

CMiniMap::~CMiniMap()
{
}

void CMiniMap::Initialize()
{
	m_tInfo.fCX = 203.f;
	m_tInfo.fCY = 113.f;

	m_pFrameKey = L"MiniMap";
	wsRealMapKey = L"Map";
	wsPlayerIconKey = L"PLAYER_MINIMAP";
}

int CMiniMap::Update()
{
	if (GET_SINGLE(CSceneMgr)->Get_CurScene() == SC_STAGE_Boss)
		wsRealMapKey = L"BossMiniMap";


	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CMiniMap::Late_Update()
{
}

void CMiniMap::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	// ¸Ê ¹è°æ
	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));

	// ½ÇÁ¦ ¸Ê
	HDC hRealMemDC = CBmpMgr::Get_Instance()->Find_Img(wsRealMapKey);

	float mapRateX = (-fScrollX) / MAPCX;
	float mapRateY = (-fScrollY) / MAPCY;

	GdiTransparentBlt(hDC,
		m_tRect.left ,
		m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hRealMemDC,
		1500 * mapRateX,
		1125 * mapRateY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));

	// ½ÇÁ¦ ¸Ê
	HDC playerMemDC = CBmpMgr::Get_Instance()->Find_Img(wsPlayerIconKey);

	CObj* pPlayer = GET_SINGLE(CObjMgr)->Get_Player();
	float playerPosX = (pPlayer->Get_Rect().left + fScrollX) / WINCX;
	float playerPosY = (pPlayer->Get_Rect().top + fScrollY) / WINCY;

	GdiTransparentBlt(hDC,
		m_tRect.left + m_tInfo.fCX * playerPosX,
		m_tRect.top + m_tInfo.fCY * playerPosY,
		23,
		23,
		playerMemDC,
		0,
		0,
		23,
		23,
		RGB(255, 0, 255));

}

void CMiniMap::Release()
{
}

void CMiniMap::Debug_Render(HDC hDC)
{
}

void CMiniMap::Anim_Update()
{
}
