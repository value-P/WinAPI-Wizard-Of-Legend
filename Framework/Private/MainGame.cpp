#include "stdafx.h"
#include "MainGame.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "PngMgr.h"
#include "ScrollMgr.h"
#include "UIMgr.h"
#include "Player.h"
#include "SkillMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

CMainGame::CMainGame()
	: m_dwTime(GetTickCount64()), m_iFPS(0),m_DC(NULL)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize(void)
{
	srand((unsigned int)time(NULL));

	CTimeMgr::Get_Instance()->Initialize();
	CSoundMgr::Get_Instance()->Initialize();
	CSkillMgr::Get_Instance()->Initialize();

	// 플레이어 생성
	CObj* pPlayer = CAbstractFactory<CPlayer>::Create_Obj(4320, MAPCY - 350, 2000.f, 100.f, 10.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pPlayer);

	Initialize_Bmp();
	Initialize_UI();

	CTileMgr::Get_Instance()->Initialize();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Back.bmp",L"Back");

	m_DC = GetDC(g_hWnd);

	CSceneMgr::Get_Instance()->Change_Scene(SC_LOGO);
}

void CMainGame::Update(void)
{
	CTimeMgr::Get_Instance()->Update();

	CSceneMgr::Get_Instance()->Update();
}

void CMainGame::Late_Update(void)
{
	CSceneMgr::Get_Instance()->Late_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainGame::Render(void)
{	
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_dwTime = GetTickCount();

		// SetWindowText : 윈도우 타이틀에 출력하는 함수
		SetWindowText(g_hWnd, m_szFPS);
	}

	HDC memDC = CBmpMgr::Get_Instance()->Find_Img(L"Back");

	CSceneMgr::Get_Instance()->Render(memDC);

	BitBlt(m_DC, 0, 0, WINCX, WINCY, memDC, 0, 0, SRCCOPY);
}

void CMainGame::Release(void)
{
	CTileMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CBmpMgr::Destroy_Instance();
	CPngMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CObjMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CUIMgr::Destroy_Instance();
	CSkillMgr::Destroy_Instance();
	CTimeMgr::Destroy_Instance();
	CSoundMgr::Destroy_Instance();
	ReleaseDC(g_hWnd, m_DC);
}

void CMainGame::Initialize_Bmp()
{
	int cnt = 0;
	// 구조물 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/CHAIR_FRONT.bmp", L"CHAIR_FRONT");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"CHAIR_FRONT", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/CHAIR_LEFT.bmp", L"CHAIR_LEFT");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"CHAIR_LEFT", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/FLAG.bmp", L"FLAG");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"FLAG", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/Map_Column.bmp", L"Map_Column");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"Map_Column", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/PRISON_HOR.bmp", L"PRISON_HOR");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"PRISON_HOR", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/SHOP_TABLE.bmp", L"SHOP_TABLE");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"SHOP_TABLE", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/STATUE.bmp", L"STATUE");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"STATUE", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/TELEPORT.bmp", L"TELEPORT");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"TELEPORT", cnt++);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Structure/PRISON_VER.bmp", L"PRISON_VER");
	CObjMgr::Get_Instance()->Add_KeyInMap(L"PRISON_VER", cnt++);
	// UI 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/BOSS_HPBAR.bmp",				L"BOSS_HPBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/BOSS_NAMEBAR.bmp",			L"BOSS_NAMEBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/COOLTIME_SHADOW.bmp",			L"COOLTIME_SHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DASH_EX.bmp",					L"DASH_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/DASH_ICON.bmp",				L"DASH_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/FIRE_DRAGON_EX.bmp",			L"FIRE_DRAGON_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/FIRE_DRAGON_ICON.bmp",		L"FIRE_DRAGON_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/FIRE_DRAGON_SKILLBAR.bmp",	L"FIRE_DRAGON_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GAIA_ARMOR_EX.bmp",			L"GAIA_ARMOR_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GAIA_ARMOR_ICON.bmp",			L"GAIA_ARMOR_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/GAIA_ARMOR_SKILLBAR.bmp",		L"GAIA_ARMOR_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_BLAST_EX.bmp",			L"ICE_BLAST_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_BLAST_ICON.bmp",			L"ICE_BLAST_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_BLAST_SKILLBAR.bmp",		L"ICE_BLAST_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/VOID_STORM_ICON.bmp",			L"VOID_STORM_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/VOID_STORM_SKILLBAR.bmp",		L"VOID_STORM_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_KRYSTAL_EX.bmp",			L"ICE_KRYSTAL_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_KRYSTAL_ICON.bmp",		L"ICE_KRYSTAL_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/ICE_KRYSTAL_SKILLBAR.bmp",	L"ICE_KRYSTAL_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/INVENTORY.bmp",				L"INVENTORY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/MIDDLEBOSS_NAMEBAR.bmp",		L"MIDDLEBOSS_NAMEBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/MiniMap.bmp",					L"MiniMap");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/Map.bmp",						L"Map");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/NORMAL_ATTACK_EX.bmp",		L"NORMAL_ATTACK_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/NORMAL_ATTACK_ICON.bmp",		L"NORMAL_ATTACK_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/PLAYER_MINIMAP.bmp",			L"PLAYER_MINIMAP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SELECT_ICON.bmp",				L"SELECT_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_HPBAR.bmp",				L"UI_HPBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MANABAR.bmp",				L"UI_MANABAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MONEY.bmp",				L"UI_MONEY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_MOUSE.bmp",				L"UI_MOUSE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_PLAYERBAR.bmp",			L"UI_PLAYERBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_SHADOW.bmp",				L"UI_SHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/UI_SKILLBAR.bmp",				L"UI_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SHARK_ICON.bmp",				L"SHARK_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/SHARK_SKILLBAR.bmp",			L"SHARK_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/KeyboardKeys.bmp",			L"KeyboardKeys");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/UI/BossMiniMap.bmp",				L"BossMiniMap");

	// 스킬 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/NORMAL_ATTACK.bmp", L"NORMAL_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/ICE_CRYSTAL.bmp", L"ICE_CRYSTAL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/GAIA_ARMOR.bmp", L"GAIA_ARMOR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/Shark.bmp", L"Shark");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/VoidStorm_Start.bmp", L"VoidStorm_Start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/VoidStorm_ING.bmp", L"VoidStorm_ING");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/VoidStorm_End.bmp", L"VoidStorm_End");

	// 보스 스킬 이미지
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/BOSS_SKILL.bmp", L"BOSS_SKILL");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/BOSS_BOXATTACK.bmp", L"BOSS_BOXATTACK");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/BOSS_PUSH.bmp", L"BOSS_PUSH");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/WAVEROCK.bmp", L"WAVEROCK");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/M_SharkCircle.bmp", L"M_SharkCircle");

	// 몬스터 스킬 이미지
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/SWORDMAN_ATTACK.bmp", L"SWORDMAN_ATTACK");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/ARCHER_BOW_LEFT.bmp", L"ARCHER_BOW_LEFT");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/ARCHER_BOW_RIGHT.bmp", L"ARCHER_BOW_RIGHT");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/WIZARD_FIRE.bmp", L"WIZARD_FIRE");

	// 맵 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/TutorialCourse.bmp", L"TutorialCourse");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/BossMap.bmp", L"BossMap");

	// 이펙트 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/HITEFFECT.bmp", L"HITEFFECT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/BOTTOM_HOLE.bmp", L"BOTTOM_HOLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/SOIL_EFFECT.bmp", L"SOIL_EFFECT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/SUMMON_CARD_SWORDMAN.bmp",	L"SUMMON_CARD_SWORDMAN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/SUMMON_CARD_ARCHER.bmp",		L"SUMMON_CARD_ARCHER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/SUMMON_CARD_WIZARD.bmp",		L"SUMMON_CARD_WIZARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Effect/SUMMON_CARD_WIZARDBALL.bmp",	L"SUMMON_CARD_WIZARDBALL");

	// 그림자 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shadow/BIG_SHADOW.bmp",	L"BIG_SHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shadow/BIG_SHADOW_VER.bmp", L"BIG_SHADOW_VER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shadow/MIDDLE_SHADOW.bmp", L"MIDDLE_SHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Shadow/NORMAL_SHADOW.bmp", L"NORMAL_SHADOW");

	// 카드 이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SHARK_CARD.bmp", L"SHARK_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/GAIA_ARMOR_CARD.bmp", L"GAIA_ARMOR_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/ICE_KRYSTAL_CARD.bmp", L"ICE_KRYSTAL_CARD");

	// 돈이미지
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MONEY.bmp", L"MONEY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/POTION.bmp", L"POTION");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/ITEMSHOP_NPC.bmp", L"ITEMSHOP_NPC");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/SUMMON.bmp", L"SUMMON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/CutScene.bmp", L"CutScene");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/KONG.bmp", L"KONG");
}

void CMainGame::Initialize_UI()
{
	// 마우스 생성
	CUIMgr::Get_Instance()->Create_Obj_Mouse();

	// 스킬바 UI
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(38.f, WINCY - 55.f, 27.f, 27.f), CObjMgr::Get_Instance()->Get_Player(), Mouse_LB);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(77.f, WINCY - 55.f, 27.f, 27.f), CObjMgr::Get_Instance()->Get_Player(), Key_Space);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(118.f, WINCY - 55.f, 27.f, 27.f), CObjMgr::Get_Instance()->Get_Player(), Mouse_RB);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(155.f, WINCY - 55.f, 27.f, 27.f), CObjMgr::Get_Instance()->Get_Player(), Key_Q);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(192.f, WINCY - 55.f, 27.f, 27.f), CObjMgr::Get_Instance()->Get_Player(), Key_E);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(229.f, WINCY - 55.f, 27.f, 27.f), CObjMgr::Get_Instance()->Get_Player(), Key_R);

	// 다른 UI
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(107.f, 29.f, 122.f, 16.f),					CObjMgr::Get_Instance()->Get_Player(), L"UI_HPBAR", UI_SLIDER_HP);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(95.f, 44.f, 96.f, 8.f),						CObjMgr::Get_Instance()->Get_Player(), L"UI_MANABAR", UI_SLIDER_MANA);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(WINCX - 20.f, WINCY - 60.f, 31.f, 31.f),		CObjMgr::Get_Instance()->Get_Player(), L"UI_MONEY", UI_MONEY);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(190.f, 300.f, 350.f, 526.f),					CObjMgr::Get_Instance()->Get_Player(), L"INVENTORY", UI_INVEN);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(90.f, 35.f, 164.f, 40.f),					L"UI_PLAYERBAR", UI_NORMAL);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(180.f, WINCY - 60.f, 320.f, 80.f),			L"UI_SKILLBAR", UI_NORMAL);
	CUIMgr::Get_Instance()->Create_Obj_UI(INFO(WINCX - 100.f, WINCY - 60.f, 31.f, 31.f),	L"UI_MONEY", UI_NORMAL);
}

