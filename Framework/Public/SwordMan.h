#pragma once
#include "Obj.h"

enum SWORD_STATE { SM_IDLE, SM_RUN, SM_ATTACK, SM_HIT, SM_DEAD };

class CSwordMan :
    public CObj
{
public:
    CSwordMan();
    virtual ~CSwordMan();

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
    void    Pattern();

private:
    void    SetFrameByState();

private:
    SWORD_STATE  m_eCurState_Anim;
    SWORD_STATE  m_ePrevState_Anim;
    MONSTER_STATE    m_eCurState;

    float       m_fTime;
};

