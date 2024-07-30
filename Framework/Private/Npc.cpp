#include "stdafx.h"
#include "Npc.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CNpc::CNpc()
{
}

CNpc::~CNpc()
{
    Release();
}

void CNpc::Initialize()
{
    m_tInfo.fCX = 64.f;
    m_tInfo.fCY = 99.f;

    m_pFrameKey = L"ITEMSHOP_NPC";

    __super::Update_Rect();
}

int CNpc::Update()
{
    return 0;
}

void CNpc::Late_Update()
{
}

void CNpc::Render(HDC hDC)
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

void CNpc::Release()
{
}

void CNpc::Debug_Render(HDC hDC)
{
}

void CNpc::Anim_Update()
{
}
