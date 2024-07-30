#include "stdafx.h"
#include "MyStage1.h"
#include "Obj.h"
#include "Player.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "UIMgr.h"
#include "GameMgr.h"
#include "SkillMgr.h"
#include "Boss.h"
#include "SoundMgr.h"
#include "BmpMgr.h"

CMyStage1::CMyStage1()
	:m_pPlayer(nullptr), m_iRenderMode(0)
{
}

CMyStage1::~CMyStage1()
{
	Release();
}

void CMyStage1::Initialize()
{
	CTileMgr::Get_Instance()->Load_Tile(L"../Data/Boss_Tile.dat");
	CObjMgr::Get_Instance()->Load_Structure();
	CSoundMgr::Get_Instance()->Play_BGM(L"BOSS_BGM.mp3", 1.f);

	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
	pPlayer->Set_Pos(4320, MAPCY - 350);	

	CObj* pBoss = CAbstractFactory<CBoss>::Create_Obj(4320, MAPCY - 1500, 500, 0, 20.f);
	CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, pBoss);
}

int CMyStage1::Update()
{
	KeyInput();

	CTileMgr::Get_Instance()->Update();
	CObjMgr::Get_Instance()->Update();
	CSkillMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();

	return 0;
}

void CMyStage1::Late_Update()
{
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update();
	CUIMgr::Get_Instance()->Late_Update();
}

void CMyStage1::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"BossMap");

	BitBlt(hDC, 0, 0, WINCX, WINCX,
		hMemDC, -fScrollX, -fScrollY, SRCCOPY);

	CObjMgr::Get_Instance()->Render(hDC);
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

void CMyStage1::Release()
{
	CSoundMgr::Get_Instance()->Stop_All();
}

void CMyStage1::KeyInput()
{
    // 모드변경
    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_TAB))
		m_iRenderMode = (m_iRenderMode + 1) % 2;
}
