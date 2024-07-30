#include "stdafx.h"
#include "Money.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "CollsionMgr.h"

CMoney::CMoney()
{
}

CMoney::~CMoney()
{
	Release();
}

void CMoney::Initialize()
{
    m_tInfo.fCX = 12.f;
    m_tInfo.fCY = 12.f;

    m_tFrame.SetInfo(0, 1, 0, 200);

    m_pFrameKey = L"MONEY";
}

int CMoney::Update()
{
    if (m_bDead) return OBJ_DEAD;

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CMoney::Late_Update()
{
	CObj* player = GET_SINGLE(CObjMgr)->Get_Player();
	float fx, fy;
	if (CCollsionMgr::Check_Rect(player->Get_ColInfo(), Get_Info(), &fx, &fy))
	{
		this->Set_Dead();

		player->Take_Money(20.f);
	}

    __super::Move_Frame();
}

void CMoney::Render(HDC hDC)
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

void CMoney::Release()
{
}

void CMoney::Debug_Render(HDC hDC)
{
}

void CMoney::Anim_Update()
{
}
