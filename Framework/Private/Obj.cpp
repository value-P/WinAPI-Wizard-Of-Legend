#include "stdafx.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "Money.h"
#include "SoundMgr.h"

CObj::CObj() : m_fSpeed(0.f), m_bDead(false), m_pTarget(nullptr), m_iMoney(0),
m_fCurHp(0.f),m_fMaxHp(0.f),m_fCurMana(0.f),m_fMaxMana(0.f), m_fAngle(0.f), m_fAttack(0.f), m_fShdowOffset(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));

	ZeroMemory(&m_tColInfo, sizeof(INFO));
	ZeroMemory(&m_tColRect, sizeof(RECT));

	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_vDir, sizeof(Vector2D));
	ZeroMemory(&m_vLookDir, sizeof(Vector2D));

	ZeroMemory(&m_tTileColInfo, sizeof(INFO));
	ZeroMemory(&m_tTileColRect, sizeof(RECT));
	ZeroMemory(&m_vTileColOffset, sizeof(Vector2D));
	ZeroMemory(&m_vColOffset, sizeof(Vector2D));

	ZeroMemory(&m_tShadowInfo, sizeof(INFO));
	ZeroMemory(&m_tShadowRect, sizeof(RECT));
}


CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left	= LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top		= LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right	= LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom	= LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));

	m_tShadowInfo.fX = m_tInfo.fX;
	m_tShadowInfo.fY = m_tInfo.fY + m_fShdowOffset;
	m_tShadowInfo.fCX = 60.f;
	m_tShadowInfo.fCY = 60.f;
	m_tShadowRect.left		= LONG(m_tShadowInfo.fX - (m_tShadowInfo.fCX * 0.5f));
	m_tShadowRect.top		= LONG(m_tShadowInfo.fY - (m_tShadowInfo.fCY * 0.5f));
	m_tShadowRect.right		= LONG(m_tShadowInfo.fX + (m_tShadowInfo.fCX * 0.5f));
	m_tShadowRect.bottom	= LONG(m_tShadowInfo.fY + (m_tShadowInfo.fCY * 0.5f));

	m_tColInfo.fX = LONG(m_tInfo.fX + m_vColOffset.x);
	m_tColInfo.fY = LONG(m_tInfo.fY - m_vColOffset.y);

	m_tColRect.left = LONG(m_tColInfo.fX - (m_tColInfo.fCX * 0.5f));
	m_tColRect.top = LONG(m_tColInfo.fY - (m_tColInfo.fCY * 0.5f));
	m_tColRect.right = LONG(m_tColInfo.fX + (m_tColInfo.fCX * 0.5f));
	m_tColRect.bottom = LONG(m_tColInfo.fY + (m_tColInfo.fCY * 0.5f));
}

void CObj::Update_TileColRect()
{
	// 중심에서 오프셋만큼 떨어진 위치에 타일콜라이더 생성
	m_tTileColInfo.fX = m_tInfo.fX + m_vTileColOffset.x;
	m_tTileColInfo.fY = m_tInfo.fY - m_vTileColOffset.y;

	m_tTileColRect.left = LONG(m_tTileColInfo.fX - (m_tTileColInfo.fCX * 0.5f));
	m_tTileColRect.top = LONG(m_tTileColInfo.fY - (m_tTileColInfo.fCY * 0.5f));
	m_tTileColRect.right = LONG(m_tTileColInfo.fX + (m_tTileColInfo.fCX * 0.5f));
	m_tTileColRect.bottom = LONG(m_tTileColInfo.fY + (m_tTileColInfo.fCY * 0.5f));
}

int CObj::Move_Frame()
{
	if (m_tFrame.stopFrame) return m_tFrame.iFrameStart;

	if (m_tFrame.dwSpeed + m_tFrame.dwTime < GetTickCount())
	{
		++m_tFrame.iFrameStart;

		m_tFrame.dwTime = GetTickCount();

		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
			return ANIM_END;
		}
	}

	return m_tFrame.iFrameStart;
}


void CObj::Get_Damage(float _dmg, CObj* _pSrc)
{
	float tempHp = m_fCurHp - _dmg;
	tempHp = max(tempHp, 0.f);

	m_fCurHp = tempHp;

	if (tempHp == 0)
		Set_Dead();
}

void CObj::Get_Mana(float _mana)
{
	float temp = m_fCurMana + _mana;

	temp = min(temp, m_fMaxMana);

	m_fCurMana = temp;
}

void CObj::Take_Money(int _money)
{
	CSoundMgr::Get_Instance()->Stop_Sound(SOUND_EFFECT_2);
	CSoundMgr::Get_Instance()->Play_Sound(L"MONEY_TAKE.mp3", SOUND_EFFECT_2, 1.f);

	m_iMoney += _money;
}
void CObj::Drop_Money()
{
	GET_SINGLE(CObjMgr)->Add_Object(OBJ_STRUCT, AB_FACTORY(CMoney)::Create_Obj(m_tInfo.fX, m_tInfo.fY));
}
