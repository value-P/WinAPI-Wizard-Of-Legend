#pragma once
#include "UI.h"
class CBossHpBar :
    public CUI
{
public:
    CBossHpBar();
    virtual ~CBossHpBar();

public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

    void    Set_FrameName(wstring _wstring) { m_wsFrameKey = _wstring; }

private:
    wstring m_wsFrameKey;
    RECT    m_FrameRect;
};

