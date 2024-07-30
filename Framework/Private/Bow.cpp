#include "stdafx.h"
#include "Bow.h"
#include "TimeMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "CollsionMgr.h"

CBow::CBow()
    :m_bOnShoot(false)
{
}

CBow::~CBow()
{
    Release();
}

void CBow::Initialize()
{
	m_eRenderID = EFFECT;
	m_eObjID = OBJ_SKILL;
	m_tFrame.dwTime = GetTickCount();
	m_tFrame.SetInfo(0, 4, 0, 40);
	m_pFrameKey = L"ARCHER_BOW_RIGHT";
	m_tInfo.fCX = 90.f;
	m_tInfo.fCY = 85.f;
	m_tColInfo.fCX = 60.f;
	m_tColInfo.fCY = 60.f;
	m_fAttack = m_pOwner->Get_Attack();
    m_fOffset = 20.f;
    m_fSpeed = 400.f;
}

int CBow::Update()
{
	if (m_bDead || m_pOwner == nullptr)
	{
		return OBJ_DEAD;
	}

    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    if (m_bOnShoot)
    {
        m_tInfo.fX += m_vDir.x * deltaTime * m_fSpeed;
        m_tInfo.fY += m_vDir.y * deltaTime * m_fSpeed;
    }
    else
    {
        Vector2D vTargetDir = (m_pTarget->Get_Pos() - m_pOwner->Get_Pos()).Normalize();

        m_fAngle = vTargetDir.ToAngle_Rad() * RAD2DEG;

		m_tInfo.fX = m_pOwner->Get_Pos().x + m_fOffset * cosf(m_fAngle * DEG2RAD);
		m_tInfo.fY = m_pOwner->Get_Pos().y - m_fOffset * sinf(m_fAngle * DEG2RAD);

		UpdateFrame(m_fAngle);

		m_vDir = vTargetDir;
    }

    __super::Update_Rect();
    return 0;
}

void CBow::Late_Update()
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bOnShoot)
	{
		float fx, fy;
		if (CCollsionMgr::Check_Rect(m_tInfo, m_pTarget->Get_Info(), &fx, &fy))
		{
			m_pTarget->Get_Damage(m_fAttack, this);
			Set_Dead();
		}
		else if (GET_SINGLE(CObjMgr)->CheckStructCol(this))
		{
			Set_Dead();
			m_tFrame.Resume_Frame();
		}
		else if (m_tRect.right < -fScrollX ||
			m_tRect.left > WINCX - fScrollX ||
			m_tRect.top > WINCY - fScrollY ||
			m_tRect.bottom < -fScrollY)
		{
			Set_Dead();
			m_tFrame.Resume_Frame();
		}
	}

    int iResult = __super::Move_Frame();

    if (iResult == ANIM_END)
    {
        m_tFrame.Stop_Frame();
        m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
    }
    else
    {
        if (m_tFrame.Get_CurFrame() == 2 && !m_bOnShoot)
        {
            m_tFrame.Stop_Frame();
        }
        else if (m_tFrame.Get_CurFrame() == 2 && m_bOnShoot)
            m_tFrame.Resume_Frame();
    }
}

void CBow::Render(HDC hDC)
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

void CBow::Release()
{
}

void CBow::Debug_Render(HDC hDC)
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

void CBow::Anim_Update()
{
}

void CBow::UpdateFrame(float _angle)
{
	float angle = 0.f;

	if (_angle + 11 >= 360)
		angle = _angle + 11 - 360;
	else
		angle = _angle + 11;

	int num = angle / 22;

	switch (num)
	{
	case 0:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion= 4;
		break;
	case 1:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion= 3;
		break;
	case 2:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion= 2;
		break;
	case 3:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion= 1;
		break;
	case 4:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion= 0;
		break;
	case 5:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion= 1;
		break;
	case 6:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion= 2;
		break;
	case 7:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion= 3;
		break;
	case 8:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion= 4;
		break;
	case 9:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion = 5;
		break;
	case 10:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion = 6;
		break;
	case 11:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion = 7;
		break;
	case 12:
		m_pFrameKey = L"ARCHER_BOW_LEFT";
		m_tFrame.iMotion = 8;
		break;
	case 13:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion = 7;
		break;
	case 14:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion = 6;
		break;
	case 15:
		m_pFrameKey = L"ARCHER_BOW_RIGHT";
		m_tFrame.iMotion = 5;
		break;
	}
}

SKILL_TYPE CBow::Get_SkillType()
{
	return ST_THROW;
}
