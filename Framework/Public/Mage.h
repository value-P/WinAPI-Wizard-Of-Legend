#pragma once
#include "Obj.h"

class CFlame;

enum MAGE_STATE { MG_IDLE, MG_RUN, MG_ATTACK, MG_HIT, MG_DEAD };

class CMage :
    public CObj
{
public:
    CMage();
    virtual ~CMage();

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;

public:
    virtual void Get_Damage(float _dmg, CObj* _pSrc) override;

private:
    void    Shot();

private:
    void    SetFrameByState();
    void    SummonBall();
private:
    MAGE_STATE    m_eCurState_Anim;
    MAGE_STATE    m_ePrevState_Anim;
    MONSTER_STATE   m_eCurState;

    CFlame*     m_pFlame;

    float       m_fTime;


};

