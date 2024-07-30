#include "stdafx.h"
#include "DamageEffect.h"

CDamageEffect::CDamageEffect()
	:m_fTime(0.f), m_fDamage(0.f)
{
}

CDamageEffect::~CDamageEffect()
{
	Release();
}

void CDamageEffect::Initialize()
{
}

int CDamageEffect::Update()
{
	return 0;
}

void CDamageEffect::Late_Update()
{
}

void CDamageEffect::Render(HDC hDC)
{
	SetBkMode(hDC, 1);
	SetTextColor(hDC, RGB(255, 255, 255));
	TCHAR str[50];
	wsprintf(str, L"%d", (int)m_fDamage);
	TextOut(hDC, m_tInfo.fX - m_tInfo.fCX * 0.5f, m_tInfo.fY - m_tInfo.fCY * 0.5f, str, _tcslen(str));
}

void CDamageEffect::Release()
{
}

void CDamageEffect::Debug_Render(HDC hDC)
{
}

void CDamageEffect::Anim_Update()
{
}
