#include "stdafx.h"
#include "ObjMgr.h"
#include "CollsionMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "SceneMgr.h"

CObjMgr*	CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
	: m_eType(ST_NONE)
{
	ZeroMemory(&m_tObjInfo, sizeof(INFO));
}


CObjMgr::~CObjMgr()
{
	Release();
}


bool CObjMgr::CheckCollision(CObj* _Src, OBJ_ID _DstType, list<CObj*>* _DstList)
{
	float fx = 0.f;
	float fy = 0.f;
	for (auto& iter : m_ObjList[_DstType])
	{
		if (CCollsionMgr::Check_Rect(_Src->Get_Info(), iter->Get_Info(), &fx, &fy))
		{
			_DstList->push_back(iter);
		}
	}

	if (_DstList->size() > 0) return true;

	return false;
}

bool CObjMgr::CheckStructCol(CObj* _Src)
{
	float fx = 0.f;
	float fy = 0.f;
	for (auto& iter : m_ObjList[OBJ_STRUCT])
	{
		if (static_cast<CStructure*>(iter)->Get_STID() == ST_STATIC)
		{
			if (CCollsionMgr::Check_Rect(_Src->Get_Info(), iter->Get_Info(), &fx, &fy))
				return true;
		}
	}

	return false;
}

CObj* CObjMgr::Get_NearestTarget(CObj* _Src, OBJ_ID _targetType, float _detectRangle)
{
	CObj* pResult = nullptr;

	Vector2D vPos = _Src->Get_Pos();
	float NearestDst = _detectRangle + 1.f;
	float tempDst = 0.f;

	for (auto& iter : m_ObjList[_targetType])
	{
		tempDst = (iter->Get_Pos() - vPos).Magnitude();
		if (tempDst < NearestDst)
		{
			pResult = iter;
			NearestDst = tempDst;
		}
	}

	return pResult;
}

void CObjMgr::Add_Object(OBJ_ID eID, CObj* pObj)
{
	if (OBJ_END <= eID || OBJ_STRUCT  > eID|| nullptr == pObj)
		return;


	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Update(void)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_EDIT && i == OBJ_PLAYER) continue;

		for (auto iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int	iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update(void)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_EDIT && i == OBJ_PLAYER) continue;

		for (auto& iter : m_ObjList[i])
		{
			iter->Late_Update();

			if (m_ObjList[i].empty())
				break;

			RENDERID eID = iter->Get_RenderID();
			m_RenderList[eID].push_back(iter);
		}
	}

}

void CObjMgr::Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	for (int i = 0; i < RENDER_END; ++i)
	{
		// 오브젝트 렌더 순서를 y가 높은 순서대로 정렬
		m_RenderList[i].sort([](CObj* pDst, CObj* pSrc)->bool
			{
				return pDst->Get_Info().fY < pSrc->Get_Info().fY;
			});

		for (auto& iter : m_RenderList[i])
		{
			if (iter->Get_Rect().right < fScrollX * -1 ||
				iter->Get_Rect().left > fScrollX * -1 + WINCX ||
				iter->Get_Rect().top > fScrollY * -1 + WINCY ||
				iter->Get_Rect().bottom < fScrollY * -1)
				continue;

			if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_EDIT && iter->Get_ObjID() == OBJ_PLAYER) continue;

			iter->Render(hDC);
		}

		m_RenderList[i].clear();
	}

}

void CObjMgr::Release(void)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), DeleteObj());
		m_ObjList[i].clear();
	}
}

void CObjMgr::Debug_Render(HDC hDC)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (CSceneMgr::Get_Instance()->Get_CurScene() == SC_EDIT && i == OBJ_PLAYER) continue;

		for (auto& iter : m_ObjList[i])
		{
			if (iter->Get_Pos().x < fScrollX * -1 ||
				iter->Get_Pos().x > fScrollX * -1 + WINCX ||
				iter->Get_Pos().y > fScrollY * -1 + WINCY ||
				iter->Get_Pos().y < fScrollY * -1)
				continue;

			iter->Debug_Render(hDC);
		}
	}
}

void CObjMgr::Delete_ID(OBJ_ID eID)
{
	for (auto& iter : m_ObjList[eID])
		Safe_Delete(iter);

	m_ObjList[eID].clear();
}

void CObjMgr::Delete_At_ChangeScene()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		if (i == OBJ_PLAYER) continue;
		
		for (auto& iter : m_ObjList[i])
			Safe_Delete(iter);

		m_ObjList[i].clear();
	}
}

void CObjMgr::Save_Structure()
{
	HANDLE	hFile = CreateFile(L"../Data/Structure.dat",
		GENERIC_WRITE, NULL, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	INFO tempInfo{};
	int structureType = 0;
	int wsFrameKey = 0;
	DWORD dwByte = 0;

	for (auto iter : m_ObjList[OBJ_STRUCT])
	{
		tempInfo = iter->Get_Info();
		structureType = (int)(dynamic_cast<CStructure*>(iter)->Get_STID());
		wsFrameKey = m_KeyToIntMap[iter->Get_FrameKey()];

		WriteFile(hFile, &tempInfo, sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &structureType, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &wsFrameKey, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Structure Save", L"성공", MB_OK);
}

void CObjMgr::Load_Structure()
{
	HANDLE	hFile = CreateFile(L"../Data/Structure.dat",
		GENERIC_READ, NULL, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	INFO tempInfo{};
	int structureType = 0;
	int wsFrameKey = 0;
	DWORD dwByte = 0;

	for_each(m_ObjList[OBJ_STRUCT].begin(), m_ObjList[OBJ_STRUCT].end(), DeleteObj());
	m_ObjList[OBJ_STRUCT].clear();

	while (true)
	{

		ReadFile(hFile, &tempInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &structureType, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &wsFrameKey, sizeof(int), &dwByte, nullptr);
		
		if (0 == dwByte)
			break;

		wstring key = m_IntToKeyMap[wsFrameKey];

		CObj* pObj = CAbstractFactory<CStructure>::Create_Obj_Structure(tempInfo, (STRUCTURE_ID)structureType, key);
		m_ObjList[OBJ_STRUCT].push_back(pObj);
	}

	CloseHandle(hFile);

}

void CObjMgr::Save_Structure(wstring pathKey)
{
	HANDLE	hFile = CreateFile(pathKey.c_str(),
		GENERIC_WRITE, NULL, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	INFO tempInfo{};
	int structureType = 0;
	int wsFrameKey = 0;
	DWORD dwByte = 0;

	for (auto iter : m_ObjList[OBJ_STRUCT])
	{
		tempInfo = iter->Get_Info();
		structureType = (int)(dynamic_cast<CStructure*>(iter)->Get_STID());
		wsFrameKey = m_KeyToIntMap[iter->Get_FrameKey()];

		WriteFile(hFile, &tempInfo, sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &structureType, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &wsFrameKey, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Structure Save", L"성공", MB_OK);
}

void CObjMgr::Load_Structure(wstring pathKey)
{
	HANDLE	hFile = CreateFile(pathKey.c_str(),
		GENERIC_READ, NULL, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	INFO tempInfo{};
	int structureType = 0;
	int wsFrameKey = 0;
	DWORD dwByte = 0;

	for_each(m_ObjList[OBJ_STRUCT].begin(), m_ObjList[OBJ_STRUCT].end(), DeleteObj());
	m_ObjList[OBJ_STRUCT].clear();

	while (true)
	{

		ReadFile(hFile, &tempInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &structureType, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &wsFrameKey, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		wstring key = m_IntToKeyMap[wsFrameKey];

		CObj* pObj = CAbstractFactory<CStructure>::Create_Obj_Structure(tempInfo, (STRUCTURE_ID)structureType, key);
		m_ObjList[OBJ_STRUCT].push_back(pObj);
	}

	CloseHandle(hFile);
}

list<CObj*>* CObjMgr::Get_Obj(OBJ_ID eID)
{
	return &(m_ObjList[eID]);
}

void CObjMgr::SetPickData(wstring _lpStr)
{
	m_lpFrameKey = _lpStr;

	switch (m_KeyToIntMap[_lpStr])
	{
	case 0:
		m_tObjInfo.fCX = 110.f;
		m_tObjInfo.fCY = 73;
		m_eType = ST_STATIC;
		break;
	case 1:
		m_tObjInfo.fCX = 32.f;
		m_tObjInfo.fCY = 126.f;
		m_eType = ST_STATIC;
		break;
	case 2:
		m_tObjInfo.fCX = 43.f;
		m_tObjInfo.fCY = 64.f;
		m_eType = ST_DECO;
		break;
	case 3:
		m_tObjInfo.fCX = 73.f;
		m_tObjInfo.fCY = 183.f;
		m_eType = ST_STATIC;
		break;
	case 4:
		m_tObjInfo.fCX = 248.f;
		m_tObjInfo.fCY = 240.f;
		m_eType = ST_STATIC;
		break;
	case 5:
		m_tObjInfo.fCX = 259.f;
		m_tObjInfo.fCY = 100.f;
		m_eType = ST_STATIC;
		break;
	case 6:
		m_tObjInfo.fCX = 73.f;
		m_tObjInfo.fCY = 146.f;
		m_eType = ST_STATIC;
		break;
	case 7:
		m_tObjInfo.fCX = 156.f;
		m_tObjInfo.fCY = 149.f;
		m_eType = ST_DECO;
		break;
	case 8:
		m_tObjInfo.fCX = 7.f;
		m_tObjInfo.fCY = 128.f;
		m_eType = ST_STATIC;
		break;
	}
}

void CObjMgr::PickingStructure(POINT pt)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;

	int iIndex = y * TILEX + x;

	if (0 > iIndex || CTileMgr::Get_Instance()->Get_VecTileSize() <= iIndex)
		return;

	INFO tempInfo = CTileMgr::Get_Instance()->Get_TileInfo(x, y);

	CObj* pObj = CAbstractFactory<CStructure>::Create_Obj_Structure(m_tObjInfo, m_eType, m_lpFrameKey);
	pObj->Set_Pos(tempInfo.fX, tempInfo.fY);

	Add_Object(OBJ_STRUCT, pObj);
}

void CObjMgr::Add_KeyInMap(wstring _key, int _uniqueNum)
{
	m_KeyToIntMap.emplace(_key, _uniqueNum);
	m_IntToKeyMap.emplace(_uniqueNum, _key);
}
