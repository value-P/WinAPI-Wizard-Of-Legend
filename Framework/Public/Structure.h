#pragma once
#include "Obj.h"
class CStructure :
    public CObj
{
public:
    CStructure();
    virtual ~CStructure();

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
    void    Set_STID(STRUCTURE_ID _type)    { m_eStructureType = _type; }
    STRUCTURE_ID    Get_STID()              { return m_eStructureType; }
private:
    STRUCTURE_ID    m_eStructureType;
};

