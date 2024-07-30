#include "stdafx.h"
#include "SoilEffect.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CSoilEffect::CSoilEffect()
	:m_fTime(0.f)
{
}

CSoilEffect::~CSoilEffect()
{
}

void CSoilEffect::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 120.f;

	m_tFrame.SetInfo(0, 3, 0, 200);

	m_pFrameKey = L"SOIL_EFFECT";
}

int CSoilEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSoilEffect::Late_Update()
{
	int iResult = __super::Move_Frame();

	if (iResult == ANIM_END)
	{
		Set_Dead();
	}
}

void CSoilEffect::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left + (long)fScrollX,
		m_tRect.top + (long)fScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CSoilEffect::Release()
{
}

void CSoilEffect::Debug_Render(HDC hDC)
{
}

void CSoilEffect::Anim_Update()
{
}
