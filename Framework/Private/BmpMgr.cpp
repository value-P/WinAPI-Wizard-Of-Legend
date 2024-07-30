#include "stdafx.h"
#include "BmpMgr.h"

CBmpMgr* CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(wstring pFilePath, wstring pImgKey)
{
	// 이미지가 맵에 없다면 삽입
	auto iter = m_mapBit.find(pImgKey);
	if (iter == m_mapBit.end())
	{
		CMyBitmap* pBmp = new CMyBitmap;
		pBmp->Load_Bmp(pFilePath);
		m_mapBit.emplace(pImgKey, pBmp);
	}
}

HDC CBmpMgr::Find_Img(const wstring pImgKey)
{
	auto iter = m_mapBit.find(pImgKey);

	if (iter == m_mapBit.end()) return nullptr;
	
	return iter->second->Get_MemDC();
}

void CBmpMgr::Release()
{
	for (auto& iter : m_mapBit)
	{
		Safe_Delete(iter.second);
	}

	m_mapBit.clear();
}
