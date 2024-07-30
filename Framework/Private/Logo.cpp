#include "stdafx.h"
#include "Logo.h"
#include "SceneMgr.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CLogo::CLogo()
	:m_eMenuType(READY)
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	ShowCursor(false);

	CSoundMgr::Get_Instance()->Play_BGM(L"MAIN_MENU_BGM.mp3", 1.f);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/READY_MENU.bmp", L"READY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/MAIN_MENU.bmp", L"SINGLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/OPTION_MENU.bmp", L"OPTION");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/QUIT_MENU.bmp", L"QUIT");

}

int CLogo::Update()
{
	KeyInput();

	return 0;
}

void CLogo::Late_Update()
{
}

void CLogo::Render(HDC hDC)
{
	HDC hMemDC = NULL;

	switch (m_eMenuType)
	{
	case READY:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"READY");
		break;
	case SINGLE:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"SINGLE");
		break;
	case OPTION:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"OPTION");
		break;
	case QUIT:
		hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"QUIT");
		break;
	}

	BitBlt(hDC, 0, 0, WINCX, WINCY, hMemDC, 0, 4, SRCCOPY);
}

void CLogo::Release()
{
	CSoundMgr::Get_Instance()->Stop_All();
}

void CLogo::KeyInput()
{
	// 레디일때만 리턴으로 입력받기
	if (m_eMenuType == READY)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
			m_eMenuType = SINGLE;
	}
	else
	{
		// 선택변경
		if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
		{
			CSoundMgr::Get_Instance()->Play_Sound_NoInterval(L"SELECT_MENU.mp3", SOUND_EFFECT_1, 1.f);

			if (m_eMenuType == SINGLE)
				m_eMenuType = QUIT;
			else
				m_eMenuType = (MENU_ID)(m_eMenuType - 1);
		}
		else if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
		{
			CSoundMgr::Get_Instance()->Play_Sound_NoInterval(L"SELECT_MENU.mp3", SOUND_EFFECT_2, 1.f);
			if (m_eMenuType == QUIT)
				m_eMenuType = SINGLE;
			else
				m_eMenuType = (MENU_ID)(m_eMenuType + 1);
		}

		if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
		{
			switch (m_eMenuType)
			{
			case SINGLE:
				CSceneMgr::Get_Instance()->Change_Scene(SC_STAGE_1);
				//CSceneMgr::Get_Instance()->Change_Scene(SC_STAGE_Boss);
				break;
			case OPTION:
				CSceneMgr::Get_Instance()->Change_Scene(SC_EDIT);
				break;
			case QUIT:
				DestroyWindow(g_hWnd);
				break;
			}
		}

		// 레디일때로 돌아가도록
		if (CKeyMgr::Get_Instance()->Key_Down(VK_ESCAPE))
			m_eMenuType = READY;
	}
}
