#include "stdafx.h"
#include "SpawnPlayer.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CSpawnPlayer::CSpawnPlayer()
{
}

CSpawnPlayer::~CSpawnPlayer()
{
	Release();
}

void CSpawnPlayer::Initialize()
{
	m_pFrameKey = L"SUMMON";

	m_tInfo.fCX = 65.25f;
	m_tInfo.fCY = 184.f;

	m_eRenderID = EFFECT;

	m_tFrame.SetInfo(0, 7, 0, 50);

	__super::Update_Rect();
}

int CSpawnPlayer::Update()
{
	if (m_bDead) return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CSpawnPlayer::Late_Update()
{
	int iResult = __super::Move_Frame();

	if (iResult == ANIM_END)
	{
		Set_Dead();
	}
}

void CSpawnPlayer::Render(HDC hDC)
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
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CSpawnPlayer::Release()
{
}
