#pragma once
#include "Obj.h"
class CSceneChangePort :
    public CObj
{
public:
    CSceneChangePort();
    virtual ~CSceneChangePort();

public:
    // CObj��(��) ���� ��ӵ�
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
