#include "stdafx.h"
#include "NormalStage.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "TileMgr.h"
#include "SkillMgr.h"
#include "UIMgr.h"
#include "ScrollMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "SwordMan.h"
#include "Card.h"
#include "TelePort.h"
#include "MiniMap.h"
#include "UIMgr.h"
#include "SceneChangePort.h"
#include "Potion.h"
#include "Npc.h"
#include "MiddleBoss.h"
#include "SpawnPlayer.h"

CNormalStage::CNormalStage()
    : m_iRenderMode(0)
{
}

CNormalStage::~CNormalStage()
{
    Release();
}

void CNormalStage::Initialize()
{
    CSoundMgr::Get_Instance()->Play_BGM(L"DUNGEON_BGM.mp3", 1.f);

    CTileMgr::Get_Instance()->Load_Tile(L"../Data/Normal_Tile.dat");
    CObjMgr::Get_Instance()->Load_Structure(L"../Data/Normal_Structure.dat");

    // 소환_1
    CSpawnPlayer* spawnEffect = new CSpawnPlayer;
    spawnEffect->Set_Pos(4230, MAPCY - 350);
    spawnEffect->Initialize();
    GET_SINGLE(CObjMgr)->Add_Object(OBJ_EFFECT, spawnEffect);

    CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
    pPlayer->Set_Pos(4230, MAPCY - 320);

    CCard* pCard = new CCard;
    pCard->Set_Pos(5060, 3355);
    pCard->Set_SkillType(SKILL_SHARK);
    pCard->Initialize();
    GET_SINGLE(CObjMgr)->Add_Object(OBJ_STRUCT, pCard);

    CTelePort* pTelePort = new CTelePort;
    pTelePort->Set_Pos(5060, 3355);
    pTelePort->Set_RenderID(BACKGROUND);
    pTelePort->Set_TeleportPos(2815, 5720);
    pTelePort->Initialize();
    GET_SINGLE(CObjMgr)->Add_Object(OBJ_STRUCT, pTelePort);

    CSceneChangePort* pChangePort = new CSceneChangePort;
    pChangePort->Set_Pos(3000,3770);
    pChangePort->Set_RenderID(BACKGROUND);
    pChangePort->Initialize();
    GET_SINGLE(CObjMgr)->Add_Object(OBJ_STRUCT, pChangePort);

    CPotion* pPotion = new CPotion;
    pPotion->Set_Pos(2600,3730);
    pPotion->Set_RenderID(GAMEOBJECT);
    pPotion->Initialize();
    GET_SINGLE(CObjMgr)->Add_Object(OBJ_STRUCT, pPotion);

    CNpc* pNpc = new CNpc;
    pNpc->Set_Pos(2600,3650);
    pNpc->Set_RenderID(GAMEOBJECT);
    pNpc->Initialize();
    GET_SINGLE(CObjMgr)->Add_Object(OBJ_STRUCT, pNpc);

    CObj* pBoss = CAbstractFactory<MiddleBoss>::Create_Obj(2455, 4500, 500, 0, 20.f);
    GET_SINGLE(CObjMgr)->Add_Object(OBJ_MONSTER, pBoss);

    GET_SINGLE(CUIMgr)->Create_Obj_UI(950, 100);
}

int CNormalStage::Update()
{
    KeyInput();

    CTileMgr::Get_Instance()->Update();
    CObjMgr::Get_Instance()->Update();
    CSkillMgr::Get_Instance()->Update();
    CUIMgr::Get_Instance()->Update();

    return 0;
}

void CNormalStage::Late_Update()
{
    CTileMgr::Get_Instance()->Late_Update();
    CObjMgr::Get_Instance()->Late_Update();
    CUIMgr::Get_Instance()->Late_Update();
}

void CNormalStage::Render(HDC hDC)
{
    float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"TutorialCourse");

    BitBlt(hDC, 0, 0, WINCX, WINCX,
        hMemDC, -fScrollX, -fScrollY, SRCCOPY);

    //CTileMgr::Get_Instance()->Render(hDC);
    CObjMgr::Get_Instance()->Render(hDC);
    CTileMgr::Get_Instance()->DecoTile_Render(hDC);
    CUIMgr::Get_Instance()->Render(hDC);
    CSkillMgr::Get_Instance()->Render(hDC);

    switch (m_iRenderMode)
    {
        // 디버그모드(디버그렌더처리)
    case 1:
        TextOut(hDC, 0, 0, TEXT("DEBUG_MODE"), 11);
        CObjMgr::Get_Instance()->Debug_Render(hDC);
        CTileMgr::Get_Instance()->Debug_Render(hDC);
        break;
    }

}

void CNormalStage::Release()
{
    CSoundMgr::Get_Instance()->Stop_All();

    GET_SINGLE(CObjMgr)->Delete_At_ChangeScene();
}

void CNormalStage::KeyInput()
{
    // 모드변경
    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_TAB))
        m_iRenderMode = (m_iRenderMode + 1) % 2;

    if (CKeyMgr::Get_Instance()->Key_Up(VK_F1))
        CSceneMgr::Get_Instance()->Change_Scene(SC_STAGE_Boss);

    if (CKeyMgr::Get_Instance()->Key_Up(VK_F2))
    {
        CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
        pPlayer->Set_Pos(2360, 4670);
    }
        


}
