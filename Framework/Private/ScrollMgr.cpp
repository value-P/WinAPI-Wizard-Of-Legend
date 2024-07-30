#include "stdafx.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr() : m_fScrollX(0.f), m_fScrollY(0.f)
{
}

CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Set_ScrollPos(float _fX, float _fY)
{
    m_fScrollX = -1 * _fX;
    m_fScrollY = -1 * _fY;
}

void CScrollMgr::Scroll_Lock()
{
    if(0.f < m_fScrollX)
        m_fScrollX = 0.f;

    if(0.f < m_fScrollY)
        m_fScrollY = 0.f;
    
    if(WINCX - MAPCX > m_fScrollX)
        m_fScrollX = WINCX - MAPCX;
    
    if(WINCY - MAPCY > m_fScrollY)
        m_fScrollY = WINCY - MAPCY;

}
