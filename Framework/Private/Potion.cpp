#include "stdafx.h"
#include "Potion.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "CollsionMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"

CPotion::CPotion()
    :m_bOnPlayer(false)
{
}

CPotion::~CPotion()
{
    Release();
}

void CPotion::Initialize()
{
    m_pFrameKey = L"POTION";
    m_wsBtnFrameKey = L"KeyboardKeys";

    m_tInfo.fCX = 53.f;
    m_tInfo.fCY = 73.f;

    m_tColInfo.fCX = 53.f;
    m_tColInfo.fCY = 100.f;
}

int CPotion::Update()
{
    if (m_bDead) return OBJ_DEAD;

    CObj* player = GET_SINGLE(CObjMgr)->Get_Player();
    float fx, fy;
    if (CCollsionMgr::Check_Rect(player->Get_ColInfo(), Get_ColInfo(), &fx, &fy))
    {
        m_bOnPlayer = true;
    }
    else
        m_bOnPlayer = false;

    if (m_bOnPlayer)
    {
        if (GET_SINGLE(CKeyMgr)->Key_Pressing('F'))
        {
            GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_EFFECT_2);
            GET_SINGLE(CSoundMgr)->Play_Sound(L"ULT_ON.mp3", SOUND_EFFECT_2,1.f);
            CObj* pPlayer = GET_SINGLE(CObjMgr)->Get_Player();
            pPlayer->Lose_Money(100);
            pPlayer->Plus_Hp(1000.f);
            Set_Dead();
        }
    }

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CPotion::Late_Update()
{
}

void CPotion::Render(HDC hDC)
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

    // 버튼 활성화
    if (m_bOnPlayer)
    {
        HDC hKeyMemDC = CBmpMgr::Get_Instance()->Find_Img(m_wsBtnFrameKey);

        GdiTransparentBlt(hDC,
            m_tInfo.fX - 12.5f + (long)fScrollX,
            m_tInfo.fY - 102.5f + (long)fScrollY,
            25.f,
            25.f,
            hKeyMemDC,
            0,
            0,
            25.f,
            25.f,
            RGB(255, 0, 255));
    }
}

void CPotion::Release()
{
}

void CPotion::Debug_Render(HDC hDC)
{
}

void CPotion::Anim_Update()
{
}
