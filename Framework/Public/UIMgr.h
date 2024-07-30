#pragma once
#include "UI.h"
class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();

public:
public:
	static CUIMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CUIMgr;

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
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();

public:
	void	Create_Obj_Mouse();
	void	Create_Obj_UI(float fx, float fy);
	void	Create_Obj_UI(INFO _UIInfo, CObj* _pOwner, PlayerSkillBtn _type);
	void	Create_Obj_UI(INFO _UIInfo, wstring _wsFrameKey, UI_ID _type);
	void	Create_Obj_UI(INFO _UIInfo, CObj* _pOwner, wstring _wsFrameKey, UI_ID _type);

	void	Add_UI(UI_ID _type, CUI* _Ui);

	// 특정 상황에서만 보여줄 UI를 삭제해주는 함수
	void	Delete_TempUI();

	CUI*	Get_Mouse() { return m_UIList[UI_MOUSE].front(); }

private:
	static CUIMgr* m_pInstance;

private:
	list<CUI*>		m_UIList[UI_END];
};

