#include "stdafx.h"
#include "Structure.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"

CStructure::CStructure()
	:m_eStructureType(ST_NONE)
{
}

CStructure::~CStructure()
{
	Release();
}

void CStructure::Initialize()
{
	m_eRenderID = GAMEOBJECT;
	m_eObjID = OBJ_STRUCT;
}

int CStructure::Update()
{

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CStructure::Late_Update()
{
}

void CStructure::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left + (long)fScrollX,
		m_tRect.top + (long)fScrollY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CStructure::Release()
{
}

void CStructure::Debug_Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

	// 일반 콜라이더
	MoveToEx(hDC, m_tColRect.left + fScrollX, m_tColRect.top + fScrollY, nullptr);
	LineTo(hDC, m_tColRect.right + fScrollX, m_tColRect.top + fScrollY);
	LineTo(hDC, m_tColRect.right + fScrollX, m_tColRect.bottom + fScrollY);
	LineTo(hDC, m_tColRect.left + fScrollX, m_tColRect.bottom + fScrollY);
	LineTo(hDC, m_tColRect.left + fScrollX, m_tColRect.top + fScrollY);

	SelectObject(hDC, hOldPen); // 기존에 사용하던것(Newpen)을 리턴함
	DeleteObject(hNewPen);
}

void CStructure::Anim_Update()
{
}
