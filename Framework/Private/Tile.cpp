#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "SummonEffect.h"

CTile::CTile()
    :m_eTileTpye(TILE_FLOOR),m_iDrawTileX(12),m_iDrawTileY(12), m_bDetected(false), m_eSummonType(MONSTER_TYPE_NONE)
{
}

CTile::~CTile()
{
    Release();
}

void CTile::Initialize()
{
    m_tInfo.fCX = TILECX;
    m_tInfo.fCY = TILECY;

    //TODO 타일스프라이트 받아오기
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/EarthTileSet.bmp",L"Tile");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/FLOOR.bmp",L"FLOOR");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/WALL.bmp",L"Wall");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/TRAP.bmp",L"Trap");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/DECO.bmp",L"DECO");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/MONSTER.bmp",L"MONSTER_TILE");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/ARCHOR.bmp",L"ARCHOR_TILE");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/MAGE.bmp",L"MAGE_TILE");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/DETECTOR.bmp",L"DETECTOR");

    m_pFrameKey = L"Tile";

    __super::Update_Rect();
}

int CTile::Update()
{
    if (m_eTileTpye == TILE_DETECTOR)
    {
        if (!m_bDetected)
        {
            CObj* player = GET_SINGLE(CObjMgr)->Get_Player();
            if (player != nullptr)
            {
                Vector2D dir = player->Get_TileColPos() - Get_Pos();
                float dist = dir.Magnitude();
                if (dir.Magnitude() < 100.f)
                {
                    list<CObj*> tempTileList;

                    int startX = (m_tInfo.fX - 500) / TILECX;
                    int startY = (m_tInfo.fY - 500) / TILECY;
                    int endX = (m_tInfo.fX + 500) / TILECX;
                    int endY = (m_tInfo.fY + 500) / TILECY;

                    CTileMgr::Get_Instance()->Get_Range_TileList(startX, startY, endX, endY, &tempTileList);

                    for (auto& iter : tempTileList)
                    {
                        TILE_ID eType = static_cast<CTile*>(iter)->Get_TileType();

                        if (eType >= TILE_SWORDMAN && eType < TILE_DETECTOR)
                        {
                            static_cast<CTile*>(iter)->SummonMonster();
                        }
                    }

                    m_bDetected = true;
                }
            }
        }
    }
    

    return 0;
}

void CTile::Late_Update()
{
}

void CTile::Render(HDC hDC)
{
}

void CTile::Release()
{
}

void CTile::Debug_Render(HDC hDC)
{
    switch (m_eTileTpye)
    {
    case TILE_WALL:
    {
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"Wall");
        float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
        float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC,
            m_tRect.left + (long)fScrollX,
            m_tRect.top + (long)fScrollY,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            hMemDC,
            0,
            0,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            RGB(255,0,255));
    }
        break;
    case TILE_TRAP:
    {
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"Trap");
        float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
        float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC,
            m_tRect.left + (long)fScrollX,
            m_tRect.top + (long)fScrollY,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            hMemDC,
            0,
            0,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            RGB(255, 0, 255));
    }
        break;
    case TILE_DECO:
    {
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"DECO");
        float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
        float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC,
            m_tRect.left + (long)fScrollX,
            m_tRect.top + (long)fScrollY,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            hMemDC,
            0,
            0,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            RGB(255, 0, 255));
    }
        break;
    case TILE_SWORDMAN:
    {
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"MONSTER_TILE");
        float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
        float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC,
            m_tRect.left + (long)fScrollX,
            m_tRect.top + (long)fScrollY,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            hMemDC,
            0,
            0,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            RGB(255, 0, 255));
    }
        break;
    case TILE_ARCHOR:
    {
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"ARCHOR_TILE");
        float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
        float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC,
            m_tRect.left + (long)fScrollX,
            m_tRect.top + (long)fScrollY,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            hMemDC,
            0,
            0,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            RGB(255, 0, 255));
    }
        break;
    case TILE_WIZARD:
    {
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"MAGE_TILE");
        float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
        float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC,
            m_tRect.left + (long)fScrollX,
            m_tRect.top + (long)fScrollY,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            hMemDC,
            0,
            0,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            RGB(255, 0, 255));
    }
        break;
    case TILE_DETECTOR:
    {
        HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"DETECTOR");
        float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
        float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

        GdiTransparentBlt(hDC,
            m_tRect.left + (long)fScrollX,
            m_tRect.top + (long)fScrollY,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            hMemDC,
            0,
            0,
            (int)m_tInfo.fCX,
            (int)m_tInfo.fCY,
            RGB(255, 0, 255));
    }
        break;
    }

}

void CTile::SummonMonster()
{
    CEffect* pEffect = new CSummonEffect;

    switch (m_eTileTpye)
    {
    case TILE_SWORDMAN:
        pEffect->Set_Pos(m_tInfo.fX, m_tInfo.fY - 50.f);
        static_cast<CSummonEffect*>(pEffect)->Set_MonsterType(MONSTER_KNIGHT);
        pEffect->Initialize();
        break;
    case TILE_WIZARD:
        pEffect->Set_Pos(m_tInfo.fX, m_tInfo.fY - 50.f);
        static_cast<CSummonEffect*>(pEffect)->Set_MonsterType(MONSTER_WIZARD);
        pEffect->Initialize();
        break;
    case TILE_ARCHOR:
        pEffect->Set_Pos(m_tInfo.fX, m_tInfo.fY - 50.f);
        static_cast<CSummonEffect*>(pEffect)->Set_MonsterType(MONSTER_ARCHOR);
        pEffect->Initialize();
        break;
    }

    GET_SINGLE(CObjMgr)->Add_Object(OBJ_EFFECT, pEffect);
}

void CTile::Anim_Update()
{
}

void CTile::Just_Draw(HDC hDC)
{
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey.c_str());

    BitBlt(hDC,
        m_tRect.left,
        m_tRect.top,
        (int)m_tInfo.fCX,
        (int)m_tInfo.fCY,
        hMemDC,
        (int)m_tInfo.fCX * m_iDrawTileX,
        (int)m_tInfo.fCY * m_iDrawTileY,
        SRCCOPY);

}

