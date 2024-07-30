#pragma once
#include "UI.h"
class CMiniMap :
    public CUI
{
public:
    CMiniMap();
    virtual ~CMiniMap();

public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

private:
    wstring wsRealMapKey;
    wstring wsPlayerIconKey;

};

