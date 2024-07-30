#include "stdafx.h"
#include "SceneChangePort.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "CollsionMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CSceneChangePort::CSceneChangePort()
    :m_bOnPlayer(false)
{
}

CSceneChangePort::~CSceneChangePort()
{
    Release();
}

void CSceneChangePort::Initialize()
{
    m_tInfo.fCX = 156.f;
    m_tInfo.fCY = 149.f;

    m_pFrameKey = L"TELEPORT";

    m_wsBtnFrameKey = L"KeyboardKeys";

    __super::Update_Rect();
}

int CSceneChangePort::Update()
{
    CObj* player = GET_SINGLE(CObjMgr)->Get_Player();
    float fx, fy;
    if (CCollsionMgr::Check_Rect(player->Get_ColInfo(), Get_Info(), &fx, &fy))
    {
        m_bOnPlayer = true;
    }
    else
        m_bOnPlayer = false;


    return OBJ_NOEVENT;
}

void CSceneChangePort::Late_Update()
{
    if (m_bOnPlayer)
    {
        if (GET_SINGLE(CKeyMgr)->Key_Pressing('F'))
        {
            GET_SINGLE(CSceneMgr)->Change_Scene(SC_STAGE_Boss);
        }
    }
}

void CSceneChangePort::Render(HDC hDC)
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

void CSceneChangePort::Release()
{
}

void CSceneChangePort::Debug_Render(HDC hDC)
{
}

void CSceneChangePort::Anim_Update()
{
}
