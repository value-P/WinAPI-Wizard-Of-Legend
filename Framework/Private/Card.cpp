#include "stdafx.h"
#include "Card.h"
#include "ObjMgr.h"
#include "BmpMgr.h"
#include "SoundMgr.h"
#include "ScrollMgr.h"
#include "CollsionMgr.h"
#include "Player.h"

CCard::CCard()
{
}

CCard::~CCard()
{
    Release();
}

void CCard::Initialize()
{
	switch (m_eSkillType)
	{
	case SKILL_SHARK:
		m_pFrameKey = L"SHARK_CARD";
		break;
	case SKILL_EARTH:
		m_pFrameKey = L"GAIA_ARMOR_CARD";
		break;
	case SKILL_ICE_ORBIT:
		m_pFrameKey = L"ICE_KRYSTAL_CARD";
		break;
	}

    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 96.f;

    __super::Update_Rect();
}

int CCard::Update()
{
	if (m_bDead) return OBJ_DEAD;

    return OBJ_NOEVENT;
}

void CCard::Late_Update()
{
	CObj* player = GET_SINGLE(CObjMgr)->Get_Player();
	float fx, fy;
	if (CCollsionMgr::Check_Rect(player->Get_ColInfo(), Get_Info(), &fx, &fy))
	{
		this->Set_Dead();

		dynamic_cast<CPlayer*>(player)->Add_NewSkill(m_eSkillType);
	}
	
}

void CCard::Render(HDC hDC)
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

void CCard::Release()
{
}

void CCard::Debug_Render(HDC hDC)
{
}

void CCard::Anim_Update()
{
}
