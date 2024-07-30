#include "stdafx.h"
#include "Wave.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "CollsionMgr.h"

CWave::CWave()
	:m_bAttacked(false), m_bStartAnim(false)
{
}

CWave::~CWave()
{
	Release();
}

void CWave::Initialize()
{
	m_pFrameKey = L"WAVEROCK";

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.SetInfo(0, 5, 0, 300);

	m_tInfo.fCX = 96.f;
	m_tInfo.fCY = 96.f;

	m_vColOffset = Vector2D(0.f, -24.f);
	m_tColInfo.fCX = 96.f;
	m_tColInfo.fCY = 48.f;

	m_fAttack = m_pOwner->Get_Attack();

	m_fDuration = 1.5f;

	__super::Update_Rect();

	float fx, fy;
	if (CCollsionMgr::Check_Rect(m_tColInfo, m_pTarget->Get_TileColInfo(), &fx, &fy))
	{
		m_pTarget->Get_Damage(m_fAttack, this);
	}

}

int CWave::Update()
{
	float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

	if (m_bDead)
		return OBJ_DEAD;

	m_fDuration -= deltaTime;
	if (m_fDuration <= 0.f)
		m_bStartAnim = true;

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CWave::Late_Update()
{
	if (!m_bStartAnim) return;

	int Result = __super::Move_Frame();
	if (Result == ANIM_END)
	{
		m_tFrame.Stop_Frame();
		m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
		Set_Dead();
	}
	else
	{
		switch (Result)
		{
		case 1:
			if (!m_bAttacked)
			{
				float fx, fy;
				if (CCollsionMgr::Check_Rect(m_tColInfo, m_pTarget->Get_TileColInfo(), &fx, &fy))
				{
					m_pTarget->Get_Damage(m_fAttack, this);
					m_bAttacked = true;
				}
				break;
			}
		}
	}

}

void CWave::Render(HDC hDC)
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

void CWave::Release()
{
}

void CWave::Debug_Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

	// 일반 콜라이더
	MoveToEx(hDC, m_tColRect.left + fScrollX, m_tColRect.top + fScrollY, nullptr);
	LineTo(hDC, m_tColRect.right + fScrollX, m_tColRect.top + fScrollY);
	LineTo(hDC, m_tColRect.right + fScrollX, m_tColRect.bottom + fScrollY);
	LineTo(hDC, m_tColRect.left + fScrollX, m_tColRect.bottom + fScrollY);
	LineTo(hDC, m_tColRect.left + fScrollX, m_tColRect.top + fScrollY);

	SelectObject(hDC, hOldPen); // 기존에 사용하던것(Newpen)을 리턴함
	DeleteObject(hNewPen);
}

void CWave::Anim_Update()
{
}

SKILL_TYPE CWave::Get_SkillType()
{
	return ST_RANGE;
}
