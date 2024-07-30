#pragma once
#include "Scene.h"

enum MENU_ID {READY,SINGLE,OPTION,QUIT,MENU_END};

class CLogo :
    public CScene
{
public:
    CLogo();
    virtual ~CLogo();

public:
    // CScene을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void  KeyInput();

private:
    MENU_ID     m_eMenuType; // 메뉴 타입
    // 0 : 기본창 / 1 : 싱글플레이 / 2:옵션(에딧) / 3:나가기 
};

