#include "stdafx.h"
#include "Icon.h"
#include "BmpMgr.h"
#include "UIMgr.h"
#include "SkillMgr.h"
#include "Player.h"

CIcon::CIcon()
    :m_eIconType(SKILL_NONE)
{
	ZeroMemory(&m_tCoolInfo, sizeof(INFO));
	ZeroMemory(&m_tCoolRect, sizeof(RECT));
}

CIcon::~CIcon()
{
    Release();
}

void CIcon::Initialize()
{
	m_tCoolInfo.fX = m_tInfo.fX;
	m_tCoolInfo.fY = m_tInfo.fY;
	m_tCoolInfo.fCX = m_tInfo.fCX;
	m_tCoolInfo.fCY = m_tInfo.fCY;
	
	m_tCoolRect.left = m_tCoolInfo.fX - m_tCoolInfo.fCX * 0.5f;
	m_tCoolRect.top = m_tCoolInfo.fY - m_tCoolInfo.fCY * 0.5f;
	m_tCoolRect.right = m_tCoolInfo.fX + m_tCoolInfo.fCY * 0.5f;
	m_tCoolRect.bottom = m_tCoolInfo.fY + m_tCoolInfo.fCX * 0.5f;

	m_eIconType = static_cast<CPlayer*>(m_pOwner)->Get_SkillTypeInKey(m_eBtnType);
}

int CIcon::Update()
{
	m_eIconType = static_cast<CPlayer*>(m_pOwner)->Get_SkillTypeInKey(m_eBtnType);

	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CIcon::Late_Update()
{
}

void CIcon::Render(HDC hDC)
{
	HDC hMemDC = nullptr;

	if (m_eIconType == SKILL_NONE) return;

	switch (m_eIconType)
	{
	case SKILL_NORMAL:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"NORMAL_ATTACK_SKILLBAR");
		break;
	case SKILL_DASH:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"DASH_SKILLBAR");
		break;
	case SKILL_SHARK:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"SHARK_SKILLBAR");
		break;
	case SKILL_EARTH:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"GAIA_ARMOR_SKILLBAR");
		break;
	case SKILL_VOIDSTORM:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"VOID_STORM_SKILLBAR");
		break;
	case SKILL_ICE_ORBIT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"ICE_KRYSTAL_SKILLBAR");
		break;
	}

	BitBlt(hDC, m_tRect.left, m_tRect.top, 
		m_tInfo.fCX, m_tInfo.fCY,
		hMemDC,	0, 0, SRCCOPY);

	HDC hCoolDC = CBmpMgr::Get_Instance()->Find_Img(L"COOLTIME_SHADOW");

	float coolTimeRate = CSkillMgr::Get_Instance()->Get_CoolTimeRate(m_eIconType);

	BitBlt(hDC, m_tCoolRect.left, m_tCoolRect.top, m_tCoolInfo.fCX, m_tCoolInfo.fCY * coolTimeRate, hCoolDC, 0, 0, SRCCOPY);
}

void CIcon::Release()
{
}

void CIcon::Debug_Render(HDC hDC)
{
}

void CIcon::Anim_Update()
{
}
