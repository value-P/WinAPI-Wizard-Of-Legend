#pragma once
#include "UI.h"
class CIcon :
    public CUI
{
public:
    CIcon();
    virtual ~CIcon();

public:
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

public:
    void    Set_IconType(PlayerSkill _skillType) { m_eIconType = _skillType; }
    void    Set_BtnType(PlayerSkillBtn _btnType) { m_eBtnType = _btnType; }

private:
    PlayerSkillBtn  m_eBtnType;
    PlayerSkill     m_eIconType;
    INFO            m_tCoolInfo;
    RECT            m_tCoolRect;
};

