#include "stdafx.h"
#include "M_Shark.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "CollsionMgr.h"

CM_Shark::CM_Shark()
	:m_fTime(0.f)
{
}

CM_Shark::~CM_Shark()
{
	Release();
}

void CM_Shark::Initialize()
{
	m_eRenderID = EFFECT;
	m_eObjID = OBJ_SKILL;


	m_tInfo.fX = m_pTarget->Get_Pos().x;
	m_tInfo.fY = m_pTarget->Get_Pos().y - 30.f;

	m_fAttack = m_pOwner->Get_Attack();

	m_eSkillType = SKILL_SHARK;
	m_pFrameKey = L"M_SharkCircle";
	m_tFrame.SetInfo(0, 25, 0, 10);
	m_tInfo.fCX = 256.f;
	m_tInfo.fCY = 256.f;
}

int CM_Shark::Update()
{
	if (m_bDead) return OBJ_DEAD;

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CM_Shark::Late_Update()
{

	int iResult = __super::Move_Frame();

	if (iResult == ANIM_END)
	{
		if (m_pFrameKey == L"M_SharkCircle")
		{
			m_pFrameKey = L"Shark";
			m_tFrame.SetInfo(0, 16, 0, 100);
			m_tInfo.fCX = 360.f;
			m_tInfo.fCY = 360.f;
			m_tColInfo.fCX = 360.f;
			m_tColInfo.fCY = 360.f;
			GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_EFFECT_3);
			GET_SINGLE(CSoundMgr)->Play_Sound(L"SharkUp.mp3", SOUND_EFFECT_3, 0.6f);
		}
		else
		{
			Set_Dead();
		}
	}
	else
	{
		if (m_pFrameKey == L"Shark")
		{
			if (m_tFrame.Get_CurFrame() == 7)
			{
				float fx, fy;
				if (CCollsionMgr::Check_Rect(m_tInfo, m_pTarget->Get_ColInfo(), &fx, &fy))
				{
					m_pTarget->Get_Damage(m_fAttack, this);
				}
			}
		}
	}
}

void CM_Shark::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

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

void CM_Shark::Release()
{
}

void CM_Shark::Debug_Render(HDC hDC)
{
	int	fScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	fScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

	// 일반 콜라이더
	MoveToEx(hDC, (int)m_tColRect.left + fScrollX, (int)m_tColRect.top + fScrollY, nullptr);
	LineTo(hDC, (int)m_tColRect.right + fScrollX, (int)m_tColRect.top + fScrollY);
	LineTo(hDC, (int)m_tColRect.right + fScrollX, (int)m_tColRect.bottom + fScrollY);
	LineTo(hDC, (int)m_tColRect.left + fScrollX, (int)m_tColRect.bottom + fScrollY);
	LineTo(hDC, (int)m_tColRect.left + fScrollX, (int)m_tColRect.top + fScrollY);

	SelectObject(hDC, hOldPen); // 기존에 사용하던것(Newpen)을 리턴함
	DeleteObject(hNewPen);
}

void CM_Shark::Anim_Update()
{
}

SKILL_TYPE CM_Shark::Get_SkillType()
{
	return ST_RANGE;
}

