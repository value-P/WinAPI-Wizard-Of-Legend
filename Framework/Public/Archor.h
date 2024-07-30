#pragma once
#include "Obj.h"

enum ARCHOR_STATE { AR_IDLE, AR_RUN, AR_ATTACK, AR_HIT, AR_DEAD };

class CBow;

class CArchor :
    public CObj
{
public:
    CArchor();
    virtual ~CArchor();

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

private:
    ARCHOR_STATE    m_eCurState_Anim;
    ARCHOR_STATE    m_ePrevState_Anim;
    MONSTER_STATE   m_eCurState;

    CBow*       m_pBow;

    float       m_fTime;
    float       m_fTime2;
};

