#pragma once
#include "UI.h"
#include "InvenIcon.h"

class CInven :
    public CUI
{
public:
    CInven();
    virtual ~CInven();

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
    void    KeyInput();
    void    ChangeExKey();

private:
    CInvenIcon  m_skillIconArr[SKILL_END];
    int         m_iSelectedIdx;
    int         m_iPrecSelectedIdx;
    bool        m_bStartChange;
    wstring     m_wsExFrameKey;
};

