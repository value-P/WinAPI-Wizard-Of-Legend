#pragma once
#include "Obj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    virtual ~CEffect();
    
public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;
};

