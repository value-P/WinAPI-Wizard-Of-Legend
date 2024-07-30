#include "stdafx.h"
#include "SceneMgr.h"
#include "MyEdit.h"
#include "Logo.h"
#include "MyStage1.h"
#include "NormalStage.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr() 
	: m_eCurScene(SC_LOGO),m_ePrevScene(SC_END),m_pScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Change_Scene(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePrevScene != m_eCurScene)
	{
		Safe_Delete(m_pScene);

		switch (m_eCurScene)
		{
		case SC_LOGO:
			m_pScene = new CLogo;
			break;
		case SC_EDIT:
			m_pScene = new CMyEdit;
			break;
		case SC_STAGE_1:
			m_pScene = new CNormalStage;
			break;
		case SC_STAGE_Boss:
			m_pScene = new CMyStage1;
			break;
		}

		m_pScene->Initialize();
		m_ePrevScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	Safe_Delete(m_pScene);
}
