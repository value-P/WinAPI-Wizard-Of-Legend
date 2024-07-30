#include "stdafx.h"
#include "PngMgr.h"

CPngMgr* CPngMgr::m_pInstance = nullptr;

CPngMgr::CPngMgr()
{
}

CPngMgr::~CPngMgr()
{
	Release();
}

void CPngMgr::Insert_Img(const TCHAR* pFilePath, const TCHAR* pImgKey)
{
	// 이미지 없다면 삽입
	auto iter = m_mapPng.find(pImgKey);
	if (iter == m_mapPng.end())
	{
		CMyPng* pPng = new CMyPng;
		pPng->Load_Png(pFilePath);
		m_mapPng.emplace(pImgKey, pPng);
	}
}

HDC CPngMgr::Find_Img(const TCHAR* pImgKey)
{
	auto iter = m_mapPng.find(pImgKey);

	if (iter == m_mapPng.end()) return nullptr;

	return iter->second->Get_MemDC();
}

void CPngMgr::Release()
{
	for (auto& iter : m_mapPng)
		Safe_Delete(iter.second);

	m_mapPng.clear();
}
