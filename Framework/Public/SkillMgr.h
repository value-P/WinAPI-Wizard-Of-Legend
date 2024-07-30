#pragma once
#include "Skill.h"

class CSkillMgr
{
private:
	CSkillMgr();
	~CSkillMgr();

public:
	static CSkillMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSkillMgr;

		return m_pInstance;
	}
	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void	Initialize();
	void	Update();
	void	Render(HDC hDC);
	void	Release();

public:
	bool	Skill(PlayerSkill _skillType, CObj* _Owner);
	bool	Skill(PlayerSkill _skillType, CObj* _Owner, float _angle);

	float	Get_CoolTimeRate(PlayerSkill _skillType);

private:
	void	CoolUpdate();
	void	CoolMaxUpdate();

private:
	static CSkillMgr* m_pInstance;

private:
	// 각 스킬들의 쿨타임을 가지고 있을 변수
	float	m_fSkillCoolDown[SKILL_END] = { 0.f, };
	float	m_fSkiilCoolMax[SKILL_END] = { 0.f, };
	// 각 스킬의 콤보 최대치 저장 변수
	int		m_iMaxComboArr[SKILL_END] = { 0, };
	// 특정 스택형 스킬(불용)을 위한 변수
	int		m_iSkillStack[SKILL_END] = { 0, };

private:
	//컷신용
	float	m_fCutSceneTime;
	bool	m_bAwakened;
	bool	m_bIce;
};

