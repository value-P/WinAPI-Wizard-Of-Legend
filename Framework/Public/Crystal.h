#pragma once
#include "Skill.h"
class CCrystal :
    public CSkill
{
public:
    CCrystal();
    virtual ~CCrystal();

public:
    // CSkill을(를) 통해 상속됨
    virtual void Initialize()   override;
    virtual int Update()        override;
    virtual void Late_Update()  override;
    virtual void Render(HDC hDC) override;
    virtual void Release()      override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update()  override;

protected:
    virtual void    InitFrameOption() override;

    float   m_fCurDst;      // 현재 offset거리
    float   m_fMaxSpeed;
    float   m_fCurSpeed;
    bool    m_bOnMaxDst;
    float   m_fTime;

    float   m_fAttackedTime;
    list<CObj*> m_AttackedObjList;

    // CSkill을(를) 통해 상속됨
    SKILL_TYPE Get_SkillType() override;
};

