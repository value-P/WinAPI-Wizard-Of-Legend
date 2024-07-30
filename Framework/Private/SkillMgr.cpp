#include "stdafx.h"
#include "SkillMgr.h"
#include "Normal.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "Crystal.h"
#include "GiaArmor.h"
#include "SoundMgr.h"
#include "Shark.h"
#include "VoidStorm.h"
#include "BmpMgr.h"

CSkillMgr* CSkillMgr::m_pInstance = nullptr;

CSkillMgr::CSkillMgr()
	:m_bAwakened(false), m_fCutSceneTime(0.f)
{
}

CSkillMgr::~CSkillMgr()
{
	Release();
}

void CSkillMgr::Initialize()
{
	for (int i = SKILL_NORMAL; i < SKILL_END; ++i)
	{
		switch (i)
		{
		case SKILL_NORMAL:
			m_iMaxComboArr[i] = 3;
			break;
		case SKILL_DASH: 
			m_iMaxComboArr[i] = 0;
			break;
		case SKILL_SHARK:
			m_iMaxComboArr[i] = 8;
			break;
		case SKILL_EARTH:
			m_iMaxComboArr[i] = 0;
			break;
		case SKILL_VOIDSTORM:
			m_iMaxComboArr[i] = 0;
			break;
		case SKILL_ICE_ORBIT:
			m_iMaxComboArr[i] = 0;
			break;
		}
	}
}

void CSkillMgr::Update()
{
	CoolMaxUpdate();
	CoolUpdate();

	float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();
	if (m_bAwakened)
	{
		m_fCutSceneTime += deltaTime;
		if (m_fCutSceneTime > 3.f)
		{
			m_bAwakened = false;
			m_fCutSceneTime = 0.f;
		}
	}
}

void CSkillMgr::Render(HDC hDC)
{
	if (m_bAwakened)
	{
		HDC hCutDC = CBmpMgr::Get_Instance()->Find_Img(L"CutScene");

		GdiTransparentBlt(hDC,
			0, 300,
			372, 279,
			hCutDC,
			0, 0,
			372, 279,
			RGB(0, 0, 0));
	}

}

void CSkillMgr::Release()
{
}

bool CSkillMgr::Skill(PlayerSkill _skillType, CObj* _Owner)
{
	if (_Owner == nullptr) return false;

	CSkill* pSkill = nullptr;

	if (m_fSkillCoolDown[_skillType] > 0.f) return false;

	switch (_skillType)
	{
	case SKILL_DASH:

		break;
	case SKILL_SHARK:
	{
		int	fScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int	fScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
		POINT pt{};
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		pSkill = new CShark;
		pSkill->Set_Pos(pt.x - fScrollX, pt.y - fScrollY);
		pSkill->Set_Owner(_Owner);
		pSkill->Initialize();
		
		CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pSkill);
	}
	break;
	case SKILL_EARTH:
		CSoundMgr::Get_Instance()->Play_Sound(L"GAIA_ARMOR_START.mp3",SOUND_EFFECT_3,1.f);
		for (int i = 0; i < 8; ++i)
		{
			pSkill = new CGiaArmor;
			pSkill->Set_Angle(i * 45);
			pSkill->Set_Owner(_Owner);
			pSkill->Set_Pos(_Owner->Get_Pos().x, _Owner->Get_Pos().y);
			pSkill->Initialize();

			CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pSkill);
		}
		break;
	case SKILL_ICE_ORBIT:
	{
		CObj* pPlayer = GET_SINGLE(CObjMgr)->Get_Player();
		if (pPlayer->Get_ManaRate() == 1.f)
		{
			CSoundMgr::Get_Instance()->Stop_Sound(SOUND_EFFECT_2);
			CSoundMgr::Get_Instance()->Play_Sound(L"Awaken.wav", SOUND_EFFECT_2, 1.f);
			pPlayer->Lose_Mana(100.f);
			m_bAwakened = true;

			for (int i = 0; i < 6; ++i)
			{
				pSkill = new CCrystal;
				pSkill->Set_Angle(i * 60);
				pSkill->Set_Owner(_Owner);
				pSkill->Initialize();

				CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pSkill);
			}
		}
		else
		{
			CSoundMgr::Get_Instance()->Stop_Sound(SOUND_EFFECT_2);
			CSoundMgr::Get_Instance()->Play_Sound(L"ICE_KRYSTAL_START.mp3", SOUND_EFFECT_2, 1.f);

			for (int i = 0; i < 3; ++i)
			{
				pSkill = new CCrystal;
				pSkill->Set_Angle(i * 120);
				pSkill->Set_Owner(_Owner);
				pSkill->Initialize();

				CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pSkill);
			}
		}
	}
		break;
	default:
		return false;
	}

	m_fSkillCoolDown[_skillType] = pSkill->Get_CoolTime();

	return true;
}

bool CSkillMgr::Skill(PlayerSkill _skillType, CObj* _Owner, float _angle)
{
	if (_Owner == nullptr) return false;

	if (m_fSkillCoolDown[_skillType] > 0.f) return false;

	CSkill* pSkill = nullptr;

	switch (_skillType)
	{
	case SKILL_NORMAL:
	{
		pSkill = new CNormal;
		pSkill->Set_Owner(_Owner);
		pSkill->Initialize();
		pSkill->Set_Angle(_angle);
		pSkill->InitFrameOption();
		pSkill->Set_Combo(m_iSkillStack[SKILL_NORMAL]);
		CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pSkill);
		m_iSkillStack[_skillType] = (m_iSkillStack[_skillType] + 1) % m_iMaxComboArr[_skillType];
	}
		break;
	case SKILL_VOIDSTORM:
			for (int i = -1; i < 2; ++i)
			{
				float angleDeg = _angle *  RAD2DEG;
				pSkill = new CVoidStorm;
				pSkill->Set_Owner(_Owner);
				pSkill->Set_Angle(angleDeg + (20 * i));
				pSkill->Initialize();
				CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pSkill);
			}
		break;

	default:
		return false;
	}

	m_fSkillCoolDown[_skillType] = pSkill->Get_CoolTime();



	return true;
}

float CSkillMgr::Get_CoolTimeRate(PlayerSkill _skillType)
{
	float fTemp = m_fSkillCoolDown[_skillType] <= 0 ? 0 : m_fSkillCoolDown[_skillType] / m_fSkiilCoolMax[_skillType];

	return fTemp;
}

void CSkillMgr::CoolUpdate()
{
	for (auto& iter : m_fSkillCoolDown)
	{
		if (iter > 0.f)
		{
			float temp = iter - CTimeMgr::Get_Instance()->Get_fDT();
			temp = max(0.f, temp);
			iter = temp;
		}
	}
}

void CSkillMgr::CoolMaxUpdate()
{
	for (int i = 0; i < SKILL_END; ++i)
	{
		switch (i)
		{
		case SKILL_NORMAL:
			m_fSkiilCoolMax[i] = 0.5f;
			break;
		case SKILL_DASH:
			m_fSkiilCoolMax[i] = 1.f;
			break;
		case SKILL_SHARK:
			m_fSkiilCoolMax[i] = 10.f;
			break;
		case SKILL_EARTH:
			m_fSkiilCoolMax[i] = 10.f;
			break;
		case SKILL_VOIDSTORM:
			m_fSkiilCoolMax[i] = 2.f;
			break;
		case SKILL_ICE_ORBIT:
			m_fSkiilCoolMax[i] = 10.f;
			break;
		}
	}
}
