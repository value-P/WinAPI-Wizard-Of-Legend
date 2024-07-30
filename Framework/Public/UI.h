#pragma once
#include "Obj.h"
class CUI :
    public CObj
{
public:
    CUI();
    virtual ~CUI();

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
    void    Set_Type(UI_ID _type) { m_eType = _type; }
    void    Set_Owner(CObj* _Obj) { m_pOwner = _Obj; }
    // 퍼센티지만큼(0~1) UI크기를 줄여주는 함수(right만 줄여줌)
    void    UI_Slider(float _fPercentage);
    // 인벤토리같은 UI의 경우 토글해주기 위해
    void    Toggle();

    bool    IsToggled() { return m_bToggle; }

protected:
    CObj*   m_pOwner;
    UI_ID   m_eType;

    // 인벤토리 UI용 변수
    bool    m_bToggle;

    // 슬라이더 UI용 변수
    float   m_fSliderMultiflier;
};

