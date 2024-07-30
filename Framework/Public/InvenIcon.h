#pragma once
#include "UI.h"
class CInvenIcon :
    public CUI
{
public:
    CInvenIcon();
    virtual ~CInvenIcon();

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
    void    Set_BtnType(PlayerSkillBtn _btnType) { m_eBtnType = _btnType; }
    PlayerSkillBtn Get_BtnType() { return m_eBtnType; }

    PlayerSkill    Get_IconType() { return m_eIconType; }
    
    void    Set_Selected(bool _bSelected) { m_bSelected = _bSelected; }

private:
    PlayerSkillBtn  m_eBtnType;
    PlayerSkill     m_eIconType;
    bool            m_bSelected;
};

