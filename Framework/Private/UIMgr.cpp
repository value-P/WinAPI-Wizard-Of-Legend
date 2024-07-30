#include "stdafx.h"
#include "UIMgr.h"
#include "Player.h"
#include "Mouse.h"
#include "Icon.h"
#include "Inven.h"
#include "MiniMap.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Update()
{
	for (int i = 0; i < UI_END; ++i)
	{
		for (auto iter = m_UIList[i].begin();
			iter != m_UIList[i].end(); )
		{
			int	iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CUI*>(*iter);
				iter = m_UIList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CUIMgr::Late_Update()
{
	for (int i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_UIList[i])
		{
			iter->Late_Update();
		}
	}
}

void CUIMgr::Render(HDC hDC)
{
	for (int i = 0; i < UI_END; ++i)
	{
		for (auto& iter : m_UIList[i])
		{
			iter->Render(hDC);
		}
	}
}

void CUIMgr::Release()
{
	for (int i = 0; i < UI_END; ++i)
	{
		for_each(m_UIList[i].begin(), m_UIList[i].end(), DeleteObj());
		m_UIList[i].clear();
	}
}

void CUIMgr::Create_Obj_Mouse()
{
	CUI* pUI = new CMouse;
	pUI->Initialize();
	pUI->Set_Type(UI_MOUSE);

	m_UIList[UI_MOUSE].push_back(pUI);
}

void CUIMgr::Create_Obj_UI(float fx, float fy)
{
	CUI* pUI = new CMiniMap;
	pUI->Set_Pos(fx, fy);
	pUI->Initialize();

	m_UIList[UI_NORMAL].push_back(pUI);
}

void CUIMgr::Create_Obj_UI(INFO _UIInfo, CObj* _pOwner, PlayerSkillBtn _type)
{
	CIcon* pUI = new CIcon;
	pUI->Set_Owner(_pOwner);
	pUI->Set_Info(_UIInfo);
	pUI->Set_BtnType(_type);
	pUI->Initialize();

	m_UIList[UI_ICON].push_back(pUI);
}

void CUIMgr::Create_Obj_UI(INFO _UIInfo, wstring _wsFrameKey, UI_ID _type)
{
	CUI* pUI = new CUI;
	pUI->Initialize();
	pUI->Set_Info(_UIInfo);
	pUI->Set_ColInfo(_UIInfo);
	pUI->Set_FrameKey(_wsFrameKey);
	pUI->Set_Type(_type);

	m_UIList[_type].push_back(pUI);
}

void CUIMgr::Create_Obj_UI(INFO _UIInfo, CObj* _pOwner, wstring _wsFrameKey, UI_ID _type)
{
	CUI* pUI = nullptr;

	if (_type == UI_INVEN)
	{
		pUI = new CInven;
		pUI->Set_Info(_UIInfo);
		pUI->Set_ColInfo(_UIInfo);
		pUI->Set_FrameKey(_wsFrameKey);
		pUI->Set_Type(_type);
		pUI->Set_Owner(_pOwner);
		pUI->Initialize();
		static_cast<CPlayer*>(_pOwner)->Set_Inven(pUI);
	}
	else
	{
		pUI = new CUI;
		pUI->Set_Info(_UIInfo);
		pUI->Set_ColInfo(_UIInfo);
		pUI->Set_FrameKey(_wsFrameKey);
		pUI->Set_Type(_type);
		pUI->Set_Owner(_pOwner);
		pUI->Initialize();
	}

	m_UIList[_type].push_back(pUI);
}

void CUIMgr::Add_UI(UI_ID _type, CUI* _Ui)
{
	if(_Ui != nullptr)
		m_UIList[_type].push_back(_Ui);
}

void CUIMgr::Delete_TempUI()
{
	for_each(m_UIList[TEMP_UI].begin(), m_UIList[TEMP_UI].end(), DeleteObj());
	m_UIList[TEMP_UI].clear();
}
