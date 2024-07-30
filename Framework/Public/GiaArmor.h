#pragma once
#include "Skill.h"
class CGiaArmor :
    public CSkill
{
public:
    CGiaArmor();
    virtual ~CGiaArmor();

public:
    // CSkill을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;


private:
    void    UpdateFrame(float _angle);

private:
    bool    m_bEndFloat;   // 공중에 다 뜰때까지
    float   m_fStartOffsetY;  // 초기 위치에서 해당 위치까지 올라가면 부유 완료
    float   m_fTime;
    // CSkill을(를) 통해 상속됨
    SKILL_TYPE Get_SkillType() override;
    // 시간 흐름 체크
};

