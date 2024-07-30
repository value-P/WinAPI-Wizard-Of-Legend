#pragma once
#include "Scene.h"
class CNormalStage :
    public CScene
{
public:
    CNormalStage();
    virtual ~CNormalStage();

public:
    // CScene을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    
private:
    void KeyInput();

private:
    int     m_iRenderMode;


};

