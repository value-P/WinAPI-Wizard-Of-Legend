#pragma once
#include "Obj.h"
class CTile :
    public CObj
{
public:
    CTile();
    virtual ~CTile();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Anim_Update() override;

    void Just_Draw(HDC hDC);

public:
    virtual void Debug_Render(HDC hDC) override;
    void    SummonMonster();

public:
    int     Get_DrawTileX() { return m_iDrawTileX; }
    void    Set_DrawTileX(int _tileXIdx) { m_iDrawTileX = _tileXIdx; }
   
    int     Get_DrawTileY() { return m_iDrawTileY; }
    void    Set_DrawTileY(int _tileYIdx) { m_iDrawTileY = _tileYIdx; }

    TILE_ID Get_TileType() { return m_eTileTpye; }
    void    Set_TileType(TILE_ID _tileID) { m_eTileTpye = _tileID; }

private:
    bool         m_bDetected;
    int          m_iDrawTileX;
    int          m_iDrawTileY;
    TILE_ID      m_eTileTpye;
    MONSTER_TYPE m_eSummonType;

};

