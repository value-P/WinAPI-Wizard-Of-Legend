#pragma once
#include "Obj.h"
class CPotion :
    public CObj
{
public:
    CPotion();
    virtual ~CPotion();

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
    wstring     m_wsBtnFrameKey;
    bool        m_bOnPlayer;

};

