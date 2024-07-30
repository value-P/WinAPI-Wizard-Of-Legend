#include "stdafx.h"
#include "GiaArmor.h"
#include "TimeMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "EarthCrack.h"

CGiaArmor::CGiaArmor()
	:m_bEndFloat(false), m_fStartOffsetY(0.f), m_fTime(0.f)
{
}

CGiaArmor::~CGiaArmor()
{
	Release();
}

void CGiaArmor::Initialize()
{
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 53.f;

	m_fSpeed = 100.f;

	m_eRenderID = EFFECT;
	m_eObjID = OBJ_SKILL;

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 80;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 11;
	m_tFrame.iMotion = 0;

	m_fOffset = 70.f;
	m_fCoolTime = 10.f;
	m_eSkillType = SKILL_EARTH;
	m_pFrameKey = L"GAIA_ARMOR";

	m_fStartOffsetY = 20.f;

	m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(m_fAngle * DEG2RAD);
	m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(m_fAngle * DEG2RAD) + m_fStartOffsetY;
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 53.f;
	m_tColInfo.fCX = 50.f;
	m_tColInfo.fCY = 53.f;

	UpdateFrame(m_fAngle);

	// 바닥 부서지는 이펙트 생성
	CObj* pEffect = CAbstractFactory<CEarthCrack>::Create_Obj(m_tInfo.fX, m_tInfo.fY - 10.f);
	pEffect->Set_RenderID(BACKGROUND);
	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
}

int CGiaArmor::Update()
{
	if (m_bSkillEnded || m_bDead
		) return OBJ_DEAD;

	float delTa = CTimeMgr::Get_Instance()->Get_fDT();

	if (!m_bEndFloat)
	{
		m_fStartOffsetY -= 20.f * delTa;
		m_fStartOffsetY = max(m_fStartOffsetY, 0.f);
		if (m_fStartOffsetY == 0.f )
			m_bEndFloat = true;

		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(m_fAngle * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(m_fAngle * DEG2RAD) + m_fStartOffsetY;
	}
	else
	{
		float tempAngle = m_fAngle - m_fSpeed * delTa;
		m_fAngle = tempAngle < 0 ? tempAngle + 360.f : tempAngle;
		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(m_fAngle * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(m_fAngle * DEG2RAD) + m_fStartOffsetY;

		m_fTime += delTa;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGiaArmor::Late_Update()
{
	if (!m_bEndFloat) return;

	list<CObj*> hitList;
	if (GET_SINGLE(CObjMgr)->CheckCollision(this, OBJ_SKILL, &hitList))
	{
		for (auto& iter : hitList)
		{
			if (static_cast<CSkill*>(iter)->Get_SkillType() == ST_THROW)
			{
				iter->Set_Dead();
				this->Set_Dead();
			}
		}
	}

	if (m_fTime > 7.f)
		m_bSkillEnded = true;

	UpdateFrame(m_fAngle);
}

void CGiaArmor::Render(HDC hDC)
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

void CGiaArmor::Release()
{
}

void CGiaArmor::Debug_Render(HDC hDC)
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

void CGiaArmor::Anim_Update()
{
}

void CGiaArmor::UpdateFrame(float _angle)
{
	int num = (_angle + 15) / 30;

	switch (num)
	{
	case 0:
		m_tFrame.iFrameStart = 3;
		break;
	case 1:
		m_tFrame.iFrameStart = 4;
		break;
	case 2:
		m_tFrame.iFrameStart = 5;
		break;
	case 3:
		m_tFrame.iFrameStart = 6;
		break;
	case 4:
		m_tFrame.iFrameStart = 7;
		break;
	case 5:
		m_tFrame.iFrameStart = 8;
		break;
	case 6:
		m_tFrame.iFrameStart = 9;
		break;
	case 7:
		m_tFrame.iFrameStart = 10;
		break;
	case 8:
		m_tFrame.iFrameStart = 11;
		break;
	case 9:
		m_tFrame.iFrameStart = 0;
		break;
	case 10:
		m_tFrame.iFrameStart = 1;
		break;
	case 11:
		m_tFrame.iFrameStart = 2;
		break;
	}
}

SKILL_TYPE CGiaArmor::Get_SkillType()
{
	return ST_RANGE;
}
