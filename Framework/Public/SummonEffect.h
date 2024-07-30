#pragma once
#include "Effect.h"
class CSummonEffect :
    public CEffect
{
public:
    CSummonEffect();
    virtual ~CSummonEffect();

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

    void    Set_MonsterType(MONSTER_TYPE _monsterType) { m_eMonsterType = _monsterType; }
 
protected:
    MONSTER_TYPE m_eMonsterType;

};

