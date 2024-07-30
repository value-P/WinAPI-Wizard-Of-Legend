#pragma once
#include "Skill.h"
class CVoidStorm :
    public CSkill
{
public:
    CVoidStorm();
    virtual ~CVoidStorm();

public:
    // CSkill을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;
    SKILL_TYPE Get_SkillType() override;

private:
    bool    m_bStormEnd;
    float   m_fAttackedTime;
    float   m_fStormEndTime;
    list<CObj*> m_AttackedObjList;
};

