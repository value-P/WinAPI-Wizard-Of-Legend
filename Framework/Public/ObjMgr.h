#pragma once
#include "stdafx.h"
#include "AbstractFactory.h"
#include "Mouse.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	static CObjMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;
		}

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
	bool		CheckCollision(CObj* _Src, OBJ_ID _DstType, list<CObj*>* _DstList);
	bool		CheckStructCol(CObj* _Src);
	CObj*		Get_NearestTarget(CObj* _Src, OBJ_ID _targetType, float _detectRangle);

	void		Add_Object(OBJ_ID eID, CObj* pObj);
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);

public:
	void		Debug_Render(HDC hDC);
	void		Delete_ID(OBJ_ID eID);
	void		Delete_At_ChangeScene();

	void		Save_Structure();
	void		Load_Structure();
	void		Save_Structure(wstring pathKey);
	void		Load_Structure(wstring pathKey);

	list<CObj*>* Get_Obj(OBJ_ID eID);

	void		SetPickData(wstring _lpStr);

	void		PickingStructure(POINT pt);

	CObj*		Get_Player() {return m_ObjList[OBJ_PLAYER].front(); }

	void		Add_KeyInMap(wstring _key, int _uniqueNum);

private:
	list<CObj*>		m_ObjList[OBJ_END];
	list<CObj*>		m_RenderList[RENDER_END];

	static CObjMgr*	m_pInstance;

	// 에딧창에서 생성시(피킹) 이용할 데이터
	INFO			m_tObjInfo;
	wstring			m_lpFrameKey;
	STRUCTURE_ID	m_eType;

	// 오브젝트 정보 불러오기용 데이터 파싱 맵
	map<wstring, int> m_KeyToIntMap;
	map<int, wstring> m_IntToKeyMap;

};
