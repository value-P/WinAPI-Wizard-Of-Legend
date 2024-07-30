#pragma once
#include "Scene.h"

class CSceneMgr
{
private:
	CSceneMgr();
	~CSceneMgr();

public:
	static CSceneMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;

		return m_pInstance;
	}
	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void	Change_Scene(SCENEID eID);
	SCENEID	Get_CurScene() { return m_eCurScene; }

	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;

	SCENEID				m_ePrevScene;
	SCENEID				m_eCurScene;
};

