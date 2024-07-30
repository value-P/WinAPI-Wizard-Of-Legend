#pragma once
#include "Obj.h"
class CTelePort :
    public CObj
{
public:
    CTelePort();
    virtual ~CTelePort();

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;

    void    Set_TeleportPos(float fx, float fy) { m_vTelePortPos = Vector2D(fx, fy); }

private:
    Vector2D    m_vTelePortPos;
    wstring     m_wsBtnFrameKey;
    bool        m_bOnPlayer;
};

