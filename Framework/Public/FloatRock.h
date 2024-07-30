#pragma once
#include "Skill.h"
class CFloatRock :
    public CSkill
{
public:
    CFloatRock();
    virtual ~CFloatRock();

public:
    // CSkill을(를) 통해 상속됨
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Release() override;
    void    Debug_Render(HDC hDC) override;
    void    Anim_Update() override;
    void    Start_Throw() { m_bStartThrow = true; }
    // CSkill을(를) 통해 상속됨
    SKILL_TYPE Get_SkillType() override;

private:
    bool Wall_Collisioned(float fX, float fY);

    bool        m_bStartThrow;
    Vector2D    m_vStartPos;

};

