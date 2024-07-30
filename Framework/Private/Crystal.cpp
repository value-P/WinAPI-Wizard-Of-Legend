#include "stdafx.h"
#include "Crystal.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"

CCrystal::CCrystal()
	:m_fMaxSpeed(0.f), m_fCurSpeed(0.f), m_bOnMaxDst(false),
	m_fCurDst(0.f), m_fTime(0.f), m_fAttackedTime(0.f)
{	 
}

CCrystal::~CCrystal()
{
}

void CCrystal::Initialize()
{
	m_fMaxSpeed = 20.f;

	m_eRenderID = EFFECT;
	m_eObjID = OBJ_SKILL;

	m_tFrame.dwTime = GetTickCount();
	m_tFrame.dwSpeed = 40;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 17;

	m_fOffset = 150.f;
	m_fCoolTime = 10.f;
	m_eSkillType = SKILL_ICE_ORBIT;
	m_pFrameKey = L"ICE_CRYSTAL";
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;
	m_tColInfo.fCX = 100.f;
	m_tColInfo.fCY = 100.f;
}

int CCrystal::Update()
{
	float delTa = CTimeMgr::Get_Instance()->Get_fDT();

	if (m_bSkillEnded) return OBJ_DEAD;

	if (m_bOnMaxDst)
	{
		m_fTime += delTa;
		// 5초간 최대 거리에서 회전 후 점점 줄어들도록
		if (m_fTime > 5.f)
		{
			m_fCurDst -= 150.f * delTa;
			m_fCurDst = max(m_fCurDst, 0.f);
			m_fCurSpeed -= 5.f * delTa;
			m_fCurSpeed = max(m_fCurSpeed, 1.f);
		}

		// 0.1초의 텀을 두고 데미지를 주도록
		m_fAttackedTime += delTa;
		if (m_fAttackedTime > 0.4f)
		{
			m_AttackedObjList.clear();
			m_fAttackedTime = 0.f;
			
			CObjMgr::Get_Instance()->CheckCollision(this, OBJ_MONSTER, &m_AttackedObjList);
			for (auto& iter : m_AttackedObjList)
				iter->Get_Damage(m_pOwner->Get_Attack() * 2.f, this);
		}

		float tempAngle = m_fAngle + m_fCurSpeed;
		m_fAngle = tempAngle >= 360 ? tempAngle - 360.f : tempAngle;
		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fCurDst * cosf(m_fAngle * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y + m_fCurDst * sinf(m_fAngle * DEG2RAD);

		if (m_fCurDst <= 0)
			m_bSkillEnded = true;

	}
	else
	{
		m_fCurDst += 100.f * delTa;
		m_fCurDst = min(m_fCurDst, m_fOffset);
		m_fCurSpeed += 5.f * delTa;
		m_fCurSpeed = min(m_fCurSpeed, m_fMaxSpeed);
		float tempAngle = m_fAngle + m_fCurSpeed;
		m_fAngle = tempAngle >= 360 ? tempAngle - 360.f : tempAngle;
		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fCurDst * cosf(m_fAngle * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y + m_fCurDst * sinf(m_fAngle * DEG2RAD);

		if (m_fCurDst >= m_fOffset)
		{
			m_bOnMaxDst = true;
		}

	}

	__super::Update_Rect();

}

void CCrystal::Late_Update()
{

}

void CCrystal::Render(HDC hDC)
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

void CCrystal::Release()
{
}

void CCrystal::Debug_Render(HDC hDC)
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

void CCrystal::Anim_Update()
{
}

void CCrystal::InitFrameOption()
{
}

SKILL_TYPE CCrystal::Get_SkillType()
{
	return ST_RANGE;
}
