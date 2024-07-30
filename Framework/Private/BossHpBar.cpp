#include "stdafx.h"
#include "BossHpBar.h"
#include "BmpMgr.h"

CBossHpBar::CBossHpBar()
{
	ZeroMemory(&m_FrameRect, sizeof(RECT));
}

CBossHpBar::~CBossHpBar()
{
    Release();
}

void CBossHpBar::Initialize()
{
	m_pFrameKey = L"BOSS_HPBAR";

	m_tInfo.fCX = 178.f;
	m_tInfo.fCY = 14.f;

	m_FrameRect.left = m_tInfo.fX - 114.f;
	m_FrameRect.top = m_tInfo.fY - 22.5f;
	m_FrameRect.right= m_tInfo.fX + 114.f;
	m_FrameRect.bottom = m_tInfo.fY + 22.5f;
}

int CBossHpBar::Update()
{
    if (m_bDead) return OBJ_DEAD;

	__super::Update_Rect();
    return OBJ_NOEVENT;
}

void CBossHpBar::Late_Update()
{
	UI_Slider(m_pOwner->Get_HpRate());

	if (m_fSliderMultiflier <= 0.f)
		Set_Dead();
}

void CBossHpBar::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	int drawCX = m_tInfo.fCX * m_fSliderMultiflier;

	GdiTransparentBlt(hDC,
			m_tRect.left, m_tRect.top,
			drawCX, m_tInfo.fCY,
			hMemDC,
			0, 0,
			drawCX, m_tInfo.fCY,
			RGB(255, 0, 255));

	HDC hFrameDC = CBmpMgr::Get_Instance()->Find_Img(m_wsFrameKey);

	GdiTransparentBlt(hDC,
		m_FrameRect.left, m_FrameRect.top - 4.f,
		228, 45,
		hFrameDC,
		0, 0,
		228, 45,
		RGB(255, 0, 255));

}

void CBossHpBar::Release()
{
}
