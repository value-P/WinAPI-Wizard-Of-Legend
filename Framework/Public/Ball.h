#pragma once
#include "Obj.h"

enum BALL_STATE { BA_IDLE, BA_SHOT, BA_HIT, BA_DEAD };

class CBall :
    public CObj
{
public:
    CBall();
    virtual ~CBall();

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
    void    SetFrameByState();

private:
    BALL_STATE    m_eCurState_Anim;
    BALL_STATE    m_ePrevState_Anim;
    MONSTER_STATE   m_eCurState;

    bool        m_bTargeting;

    float       m_fTime;
    float       m_fTime2;
    float       m_fIdleTime;
    float       m_fShotTime;
    float       m_fRotateTime;
};

