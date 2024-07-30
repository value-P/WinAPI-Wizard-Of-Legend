#include "stdafx.h"
#include "M_VoidStorm.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "CollsionMgr.h"
#include "SoundMgr.h"

CM_VoidStorm::CM_VoidStorm()
    :m_bStormEnd(false), m_fAttackedTime(0.f), m_fStormEndTime(0.f)
{
}

CM_VoidStorm::~CM_VoidStorm()
{
    Release();
}

void CM_VoidStorm::Initialize()
{
    m_eRenderID = EFFECT;
    m_eObjID = OBJ_SKILL;
    m_eSkillType = SKILL_VOIDSTORM;

    m_tInfo.fX = m_pOwner->Get_Pos().x;
    m_tInfo.fY = m_pOwner->Get_Pos().y;
    m_tInfo.fCX = 140.f;
    m_tInfo.fCY = 140.f;
    m_tColInfo.fCX = 140.f;
    m_tColInfo.fCY = 140.f;
    m_fCoolTime = 2.f;

    m_pFrameKey = L"VoidStorm_Start";
    m_tFrame.SetInfo(0, 3, 0, 200);

    m_fSpeed = 400.f;
    m_fAttack = m_pOwner->Get_Attack();


}

int CM_VoidStorm::Update()
{
    float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    if (m_bDead) return OBJ_DEAD;

    float delTa = CTimeMgr::Get_Instance()->Get_fDT();

    m_fStormEndTime += delTa;
    if (m_fStormEndTime > 6.f)
        m_bStormEnd = true;

    if (m_pFrameKey == L"VoidStorm_ING")
    {
        m_tInfo.fX += m_fSpeed * delTa * cosf(m_fAngle * DEG2RAD);
        m_tInfo.fY -= m_fSpeed * delTa * sinf(m_fAngle * DEG2RAD);

        // 0.1초의 텀을 두고 데미지를 주도록
        m_fAttackedTime += delTa;
        if (m_fAttackedTime > 0.5f)
        {
            float fx, fy;
            if (CCollsionMgr::Check_Rect(m_tInfo, m_pTarget->Get_ColInfo(), &fx, &fy))
            {
                m_pTarget->Get_Damage(m_fAttack, this);
            }

            m_fAttackedTime = 0.f;
        }

        if (GET_SINGLE(CObjMgr)->CheckStructCol(this))
        {
            m_pFrameKey = L"VoidStorm_End";
        }
        else if (m_tRect.right < -fScrollX ||
            m_tRect.left > WINCX - fScrollX ||
            m_tRect.top > WINCY - fScrollY ||
            m_tRect.bottom < -fScrollY)
        {
            Set_Dead();
        }
    }

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CM_VoidStorm::Late_Update()
{
    int iResult = __super::Move_Frame();

    if (iResult == ANIM_END)
    {
        if (m_pFrameKey == L"VoidStorm_Start")
        {
            GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_EFFECT_2);
            GET_SINGLE(CSoundMgr)->Play_Sound(L"ChaosStormEnd.wav", SOUND_EFFECT_2, 1.f);

            m_pFrameKey = L"VoidStorm_ING";
        }
        else if (m_bStormEnd && m_pFrameKey == L"VoidStorm_ING")
            m_pFrameKey = L"VoidStorm_End";
        else if (m_pFrameKey == L"VoidStorm_End")
            Set_Dead();
    }
}

void CM_VoidStorm::Render(HDC hDC)
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

void CM_VoidStorm::Release()
{
}

void CM_VoidStorm::Debug_Render(HDC hDC)
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

void CM_VoidStorm::Anim_Update()
{
}

SKILL_TYPE CM_VoidStorm::Get_SkillType()
{
    return ST_THROW;
}
