#include "stdafx.h"
#include "Skill.h"

CSkill::CSkill():
	m_fCoolTime(0.f),m_eSkillType(SKILL_NONE), m_bSkillEnded(false), m_fOffset(0.f), m_pOwner(nullptr),
	m_iCurCombo(0)
{
}

CSkill::~CSkill()
{
}
