#include "stdafx.h"
#include "SummonEffect.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "SwordMan.h"
#include "ObjMgr.h"
#include "Archor.h"
#include "Mage.h"
#include "Ball.h"
#include "SoundMgr.h"

CSummonEffect::CSummonEffect()
	:m_eMonsterType(MONSTER_TYPE_NONE)
{
}

CSummonEffect::~CSummonEffect()
{
	Release();
}

void CSummonEffect::Initialize()
{
	m_tInfo.fCX = 75.f;
	m_tInfo.fCY = 115.f;

	m_eRenderID = EFFECT;

	switch (m_eMonsterType)
	{
	case MONSTER_KNIGHT:
		m_pFrameKey = L"SUMMON_CARD_SWORDMAN";
		break;
	case MONSTER_ARCHOR:
		m_pFrameKey = L"SUMMON_CARD_ARCHER";
		break;
	case MONSTER_BALL:
		m_pFrameKey = L"SUMMON_CARD_WIZARDBALL";
		break;
	case MONSTER_WIZARD:
		m_pFrameKey = L"SUMMON_CARD_WIZARD";
		break;
	}

	m_tFrame.SetInfo(0, 31, 0, 50);

	GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_EFFECT_2);
	GET_SINGLE(CSoundMgr)->Play_Sound(L"CARD_SUMMON.mp3", SOUND_EFFECT_2, 1.f);

	__super::Update_Rect();
}

int CSummonEffect::Update()
{
	if (m_bDead)
	{
		CObj* pObj = nullptr;

		// 몬스터 소환
		switch (m_eMonsterType)
		{
		case MONSTER_KNIGHT:
			pObj = AB_FACTORY(CSwordMan)::Create_Obj(m_tInfo.fX, m_tInfo.fY);
			break;
		case MONSTER_ARCHOR:
			pObj = AB_FACTORY(CArchor)::Create_Obj(m_tInfo.fX, m_tInfo.fY);
			break;
		case MONSTER_BALL:
			pObj = AB_FACTORY(CBall)::Create_Obj(m_tInfo.fX, m_tInfo.fY);
			break;
		case MONSTER_WIZARD:
			pObj = AB_FACTORY(CMage)::Create_Obj(m_tInfo.fX, m_tInfo.fY);
			break;
		}

		GET_SINGLE(CObjMgr)->Add_Object(OBJ_MONSTER, pObj);

		return OBJ_DEAD;
	}


	return OBJ_NOEVENT;
}

void CSummonEffect::Late_Update()
{
	int iResult = __super::Move_Frame();

	if (iResult == ANIM_END)
	{
		Set_Dead();
	}
}

void CSummonEffect::Render(HDC hDC)
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
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CSummonEffect::Release()
{
}
