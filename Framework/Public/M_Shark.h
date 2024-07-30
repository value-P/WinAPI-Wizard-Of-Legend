#pragma once
#include "Skill.h"
class CM_Shark :
    public CSkill
{
public:
    CM_Shark();
    virtual ~CM_Shark();

public:
    // CSkill을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;

    // CSkill을(를) 통해 상속됨
    SKILL_TYPE Get_SkillType() override;
private:
    float   m_fTime;       // 시간 흐름 체크

};

