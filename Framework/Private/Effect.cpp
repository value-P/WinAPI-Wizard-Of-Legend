#include "stdafx.h"
#include "Effect.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CEffect::CEffect()
{
}

CEffect::~CEffect()
{
	Release();
}

void CEffect::Initialize()
{
}

int CEffect::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CEffect::Late_Update()
{
	int iResult = __super::Move_Frame();

	if (iResult == ANIM_END)
		Set_Dead();
}

void CEffect::Render(HDC hDC)
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

void CEffect::Release()
{
}

void CEffect::Debug_Render(HDC hDC)
{
}

void CEffect::Anim_Update()
{
}
