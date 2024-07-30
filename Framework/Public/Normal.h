#pragma once
#include "Skill.h"

enum NORMAL_DIR {NORMAL_RIGHT,NORMAL_RU,NORMAL_UP,NORMAL_LU,NORMAL_LEFT,NORMAL_LD,NORMAL_DOWN,NORMAL_RD};

class CNormal :
    public CSkill
{
public:
    CNormal();
    virtual ~CNormal();

public:
    // CSkill을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

    // CSkill을(를) 통해 상속됨
    SKILL_TYPE Get_SkillType() override;

protected:
    virtual void    InitFrameOption() override;

};

