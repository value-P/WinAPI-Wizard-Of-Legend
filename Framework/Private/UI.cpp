#include "stdafx.h"
#include "UI.h"
#include "BmpMgr.h"
#include "SoundMgr.h"

CUI::CUI()
	:m_pOwner(nullptr), m_bToggle(false), m_eType(UI_NORMAL), m_fSliderMultiflier(1.f)
{
}

CUI::~CUI()
{
	Release();
}

void CUI::Initialize()
{
	AddFontResource(L"../Image/Font/DungGeunMo.ttf");
}

int CUI::Update()
{
	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CUI::Late_Update()
{

	// 슬라이더 UI 따로처리
	if (m_eType == UI_SLIDER_HP && m_pOwner != nullptr)
		UI_Slider(m_pOwner->Get_HpRate());
	else if (m_eType == UI_SLIDER_MANA && m_pOwner != nullptr)
		UI_Slider(m_pOwner->Get_ManaRate());

	__super::Move_Frame();
}

void CUI::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	int drawCX = 0;

	if (m_eType == UI_SLIDER_HP || m_eType == UI_SLIDER_MANA)
		drawCX = m_tInfo.fCX * m_fSliderMultiflier;
	else
		drawCX = m_tInfo.fCX;

	if (m_eType == UI_MONEY && m_pOwner != nullptr)
	{
		SetBkMode(hDC, 1);
		SetTextColor(hDC, RGB(255, 255, 255));
		HFONT hFont, oldFont;
		TCHAR str[50];
		wsprintf(str, L"%d", m_pOwner->Get_Money());
		hFont = CreateFont(30, 0, 0, 0, 500, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("둥근모꼴"));
		oldFont = (HFONT)SelectObject(hDC, hFont);
		TextOut(hDC, m_tInfo.fX - m_tInfo.fCX * 1.5f , m_tInfo.fY - m_tInfo.fCY *0.5f, str, _tcslen(str));

		DeleteObject(hFont);
		SelectObject(hDC, oldFont);
	}
	else
	{
		GdiTransparentBlt(hDC,
			m_tRect.left, m_tRect.top,
			drawCX, m_tInfo.fCY,
			hMemDC,
			0, 0,
			drawCX, m_tInfo.fCY,
			RGB(255, 0, 255));
	}
}

void CUI::Release()
{
}

void CUI::Debug_Render(HDC hDC)
{
}

void CUI::Anim_Update()
{
}

void CUI::UI_Slider(float _fPercentage)
{
	if (_fPercentage < 0.f) _fPercentage = 0.f;
	else if (_fPercentage > 1.f) _fPercentage = 1.f;

	m_fSliderMultiflier = _fPercentage;
}

void CUI::Toggle()
{
	if (!m_bToggle)
		CSoundMgr::Get_Instance()->Play_Sound(L"CLOSE_INVENTORY.mp3", SOUND_UI_1, 1.f);
	else
		CSoundMgr::Get_Instance()->Play_Sound(L"OPEN_INVENTORY.mp3", SOUND_UI_2, 1.f);

	m_bToggle = !m_bToggle;
}