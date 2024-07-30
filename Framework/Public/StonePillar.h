#pragma once
#include "Skill.h"
class CStonePillar :
    public CSkill
{
public:
    CStonePillar();
    virtual ~CStonePillar();

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

private:
    list<CObj*> m_pPillarList;
    bool        m_bWallCollisioned; // 기둥이 벽까지 닿으면 그만 생성시키고 기둥들의 애니메이션을 재생하도록
    float       m_fTime;
    Vector2D    m_vStartPos;

};

