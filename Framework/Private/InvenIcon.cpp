#include "stdafx.h"
#include "InvenIcon.h"
#include "SkillMgr.h"
#include "BmpMgr.h"
#include "Player.h"

CInvenIcon::CInvenIcon()
	:m_eIconType(SKILL_NONE), m_bSelected(false)
{
}

CInvenIcon::~CInvenIcon()
{
	Release();
}

void CInvenIcon::Initialize()
{
	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;
	m_eIconType = static_cast<CPlayer*>(m_pOwner)->Get_SkillTypeInKey(m_eBtnType);
}

int CInvenIcon::Update()
{
	m_eIconType = static_cast<CPlayer*>(m_pOwner)->Get_SkillTypeInKey(m_eBtnType);

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CInvenIcon::Late_Update()
{
}

void CInvenIcon::Render(HDC hDC)
{
	HDC hMemDC = nullptr;

	if (m_eIconType == SKILL_NONE) return;

	switch (m_eIconType)
	{
	case SKILL_NORMAL:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"NORMAL_ATTACK_ICON");
		break;
	case SKILL_DASH:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"DASH_ICON");
		break;
	case SKILL_SHARK:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"SHARK_ICON");
		break;
	case SKILL_EARTH:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"GAIA_ARMOR_ICON");
		break;
	case SKILL_VOIDSTORM:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"VOID_STORM_ICON");
		break;
	case SKILL_ICE_ORBIT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"ICE_KRYSTAL_ICON");
		break;
	}

	BitBlt(hDC, m_tRect.left, m_tRect.top,
		m_tInfo.fCX, m_tInfo.fCY,
		hMemDC, 0, 0, SRCCOPY);

	if (!m_bSelected) 
		return;

	HDC hCoolDC = CBmpMgr::Get_Instance()->Find_Img(L"SELECT_ICON");

	GdiTransparentBlt(hDC,
		m_tRect.left,
		m_tRect.top,
		32.f,
		32.f,
		hCoolDC,
		0, 0,
		32.f, 32.f,
		RGB(255, 0, 255));

}

void CInvenIcon::Release()
{
}

void CInvenIcon::Debug_Render(HDC hDC)
{
}

void CInvenIcon::Anim_Update()
{
}
