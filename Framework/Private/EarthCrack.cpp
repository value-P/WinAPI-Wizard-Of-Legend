#include "stdafx.h"
#include "EarthCrack.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"

CEarthCrack::CEarthCrack()
	:m_fTime(0.f)
{
}

CEarthCrack::~CEarthCrack()
{
	Release();
}

void CEarthCrack::Initialize()
{
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 75.f;

	m_pFrameKey = L"BOTTOM_HOLE";
}

int CEarthCrack::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	float deltaTime = CTimeMgr::Get_Instance()->Get_fDT();

	m_fTime += deltaTime;
	if (m_fTime > 10.f)
		Set_Dead();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CEarthCrack::Late_Update()
{

	__super::Move_Frame();
}

void CEarthCrack::Render(HDC hDC)
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

void CEarthCrack::Release()
{
}

void CEarthCrack::Debug_Render(HDC hDC)
{
}

void CEarthCrack::Anim_Update()
{
}
