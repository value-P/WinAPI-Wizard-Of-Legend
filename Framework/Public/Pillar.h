#pragma once
#include "Skill.h"
class CPillar :
    public CSkill
{
public:
    CPillar();
    virtual ~CPillar();

public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

    // CSkill을(를) 통해 상속됨
    SKILL_TYPE Get_SkillType() override;

public:
    void    StartUpdate() { m_bStartUpdate = true; }

private:
    bool    m_bStartUpdate;
    bool    m_bAttacked;

};

