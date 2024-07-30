#include "stdafx.h"
#include "SwordManSkill.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

SwordManSkill::SwordManSkill()
{
}

SwordManSkill::~SwordManSkill()
{
}

void SwordManSkill::Initialize()
{
	m_eRenderID = EFFECT;
	m_eObjID = OBJ_SKILL;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 40;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 4;
	m_fOffset = 120.f;
	m_fCoolTime = 0.5f;
	m_pFrameKey = L"SWORDMAN_ATTACK";
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;
	m_tColInfo.fCX = 100.f;
	m_tColInfo.fCY = 100.f;
}

int SwordManSkill::Update()
{
	if (m_bSkillEnded) return OBJ_DEAD;


	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void SwordManSkill::Late_Update()
{
	int animEnd = __super::Move_Frame();
	if (animEnd == ANIM_END)
	{
		m_bSkillEnded = true;
	}
	else
	{
		switch (animEnd)
		{
		case 3:
			// 데미지를 5프레임에서 주도록
			if (CObjMgr::Get_Instance()->CheckCollision(this, OBJ_PLAYER, &m_TargetList))
			{
				m_pOwner->Get_Mana(5.f);

				for (auto& iter : m_TargetList)
					iter->Get_Damage(m_pOwner->Get_Attack(), this);

				m_TargetList.clear();
			}
			break;
		}
	}
}

void SwordManSkill::Render(HDC hDC)
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

void SwordManSkill::Release()
{
}

void SwordManSkill::Debug_Render(HDC hDC)
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

void SwordManSkill::Anim_Update() {
}

void SwordManSkill::InitFrameOption()
{

	if (m_fAngle >=  7 * PI / 4 || m_fAngle < PI / 4)
	{
		m_tFrame.iMotion = 1;
		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(0.f * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(0.f * DEG2RAD);
	}
	else if (m_fAngle >= PI / 4 && m_fAngle < 3 * PI / 4)
	{
		m_tFrame.iMotion = 2;
		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(90.f * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(90.f * DEG2RAD);
	}
	else if (m_fAngle >= 3 * PI / 4 && m_fAngle < 5 * PI / 4)
	{
		m_tFrame.iMotion = 0;
		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(180.f * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(180.f * DEG2RAD);
	}
	else if (m_fAngle >= 5 * PI / 4 && m_fAngle < 7 * PI / 4)
	{
		m_tFrame.iMotion = 4;
		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(270.f * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(270.f * DEG2RAD);
	}
}

SKILL_TYPE SwordManSkill::Get_SkillType()
{
	return ST_RANGE;
}
