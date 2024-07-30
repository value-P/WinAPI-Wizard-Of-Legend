#include "stdafx.h"
#include "StonePillar.h"
#include "TimeMgr.h"
#include "BmpMgr.h"
#include "Pillar.h"
#include "TileMgr.h"
#include "ObjMgr.h"
#include "EarthCrack.h"

CStonePillar::CStonePillar()
    :m_bWallCollisioned(false), m_fTime(0.f)
{
}

CStonePillar::~CStonePillar()
{
    Release();
}

void CStonePillar::Initialize()
{
    m_tInfo.fX = m_pOwner->Get_Info().fX;
    m_tInfo.fY = m_pOwner->Get_Info().fY;
    m_tRect = m_pOwner->Get_Rect();
    m_pFrameKey = L"BOSS_SKILL";
    m_eRenderID = GAMEOBJECT;
    m_vStartPos.x = m_pOwner->Get_TileColPos().x + (m_vDir.x * 64.f);
    m_vStartPos.y = m_pOwner->Get_TileColPos().y - (m_vDir.y * 64.f);

    CObj* pPillar = CAbstractFactory<CPillar>::Create_Obj(m_vStartPos.x, m_vStartPos.y - 64.f, m_pFrameKey);
    pPillar->Set_Attack(m_pOwner->Get_Attack());
    CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pPillar);
    m_pPillarList.push_back(pPillar);

    // 바닥 부서지는 이펙트 생성
    CObj* pEffect = CAbstractFactory<CEarthCrack>::Create_Obj(m_vStartPos.x, m_vStartPos.y - 32.f);
    pEffect->Set_RenderID(BACKGROUND);
    CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

}

int CStonePillar::Update()
{
    // 벽까지 다 깔았으면 업데이트 진행
    if (m_bWallCollisioned)
    {
        m_fTime += CTimeMgr::Get_Instance()->Get_fDT();

        if (m_fTime > m_fDuration)
        {
            for (auto& iter : m_pPillarList)
                iter->Set_Dead();

            return OBJ_DEAD;
        }
    }
    else
    {
        m_vStartPos.x = m_vStartPos.x + (m_vDir.x * 64.f);
        m_vStartPos.y = m_vStartPos.y - (m_vDir.y * 64.f);

        int idxX = m_vStartPos.x / TILECX;
        int idxY = (m_vStartPos.y - 64.f) / TILECY;

        CObj* tempTile = CTileMgr::Get_Instance()->Get_Tile(idxX, idxY);

        TILE_ID TileType;
        if (tempTile != nullptr)
            TileType = static_cast<CTile*>(tempTile)->Get_TileType();
        else
            TileType = TILE_WALL;

        // 다음프레임에 벽에 충돌하는 위치에 기둥이 생성된다면 생성 멈추고 기둥 올리기
        if (TileType == TILE_WALL)
        {
            m_bWallCollisioned = true;
            for (auto& iter : m_pPillarList)
                static_cast<CPillar*>(iter)->StartUpdate();
        }
        else
        {
            CObj* pPillar = CAbstractFactory<CPillar>::Create_Obj(m_vStartPos.x, m_vStartPos.y - 64.f, m_pFrameKey);
            pPillar->Set_Attack(m_pOwner->Get_Attack());
            CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pPillar);
            m_pPillarList.push_back(pPillar);
            // 바닥 부서지는 이펙트 생성
            CObj* pEffect = CAbstractFactory<CEarthCrack>::Create_Obj(m_vStartPos.x, m_vStartPos.y - 32.f);
            pEffect->Set_RenderID(BACKGROUND);
            CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

        }       
    }

    return OBJ_NOEVENT;
}

void CStonePillar::Late_Update()
{
    // 벽까지 다 깔았으면 업데이트 진행
    if (m_bWallCollisioned)
    {
        for (auto& iter : m_pPillarList)
            iter->Late_Update();
    }
}

void CStonePillar::Render(HDC hDC)
{

}

void CStonePillar::Release()
{
    m_pPillarList.clear();
}

void CStonePillar::Debug_Render(HDC hDC)
{
    // 렌더는 그냥 진행
    for (auto& iter : m_pPillarList)
        iter->Debug_Render(hDC);
}

void CStonePillar::Anim_Update()
{
}

SKILL_TYPE CStonePillar::Get_SkillType()
{
    return ST_WALL;
}
