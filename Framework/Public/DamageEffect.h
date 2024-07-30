#pragma once
#include "Effect.h"
class CDamageEffect :
    public CEffect
{
public:
    CDamageEffect();
    virtual ~CDamageEffect();

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;


private:
    float   m_fTime;
    float   m_fDamage;
};

