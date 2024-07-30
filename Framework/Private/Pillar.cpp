#include "stdafx.h"
#include "Pillar.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Skill.h"
#include "ObjMgr.h"
#include "CollsionMgr.h"
#include "SoilEffect.h"

CPillar::CPillar()
	:m_bStartUpdate(false), m_bAttacked(false)
{
}

CPillar::~CPillar()
{
    Release();
}

void CPillar::Initialize()
{
    m_tFrame.dwTime = GetTickCount();
    m_tFrame.SetInfo(0, 2, 0, 200);

    m_tInfo.fCX = 40.f;
    m_tInfo.fCY = 128.f;

	m_vColOffset = Vector2D(0.f, -48.f);
	m_tColInfo.fCX = 40.f;
	m_tColInfo.fCY = 32.f;
}

int CPillar::Update()
{
	if (m_bDead)
	{
		CObj* effect = new CSoilEffect;
		effect->Set_Pos(m_tInfo.fX, m_tInfo.fY);
		effect->Set_RenderID(BACKGROUND);
		effect->Initialize();
		GET_SINGLE(CObjMgr)->Add_Object(OBJ_EFFECT, effect);
		return OBJ_DEAD;
	}
    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CPillar::Late_Update()
{
	if (!m_bStartUpdate) return;

   int Result = __super::Move_Frame();
   if (Result == ANIM_END)
   {
	   m_tFrame.Stop_Frame();
	   m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
   }
   else
   {
	   switch (Result)
	   {
	   case 1:
		   if (!m_bAttacked)
		   {
			   CObj* player = CObjMgr::Get_Instance()->Get_Player();
			   float fx, fy;
			   if (CCollsionMgr::Check_Rect(m_tColInfo, player->Get_TileColInfo(), &fx, &fy))
			   {
				   player->Get_Damage(m_fAttack, this);
				   m_bAttacked = true;
			   }
			   break;
		   }
	   }
   }
}

void CPillar::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_tRect.right < fScrollX * -1 ||
		m_tRect.left > fScrollX * -1 + WINCX ||
		m_tRect.top > fScrollY * -1 + WINCY ||
		m_tRect.bottom < fScrollY * -1)
		return;

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

void CPillar::Release()
{
}

void CPillar::Debug_Render(HDC hDC)
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

void CPillar::Anim_Update()
{
}

SKILL_TYPE CPillar::Get_SkillType()
{
	return ST_WALL;
}
