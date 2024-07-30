#pragma once
#include "Scene.h"
class CMyEdit :
    public CScene
{
public:
    CMyEdit();
    virtual ~CMyEdit();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void    Key_Input();

private:
    float   m_fCameraSpeed;

    // 편집 모드
    int     m_EditMode;
    // 타일모드 -> 범위타일,단일타일 모드
    bool    m_RangeTileMode;

    // 타일모드사용
    bool    m_bClicked;
    POINT   m_tStartPt;
    POINT   m_tEndPt;

};

