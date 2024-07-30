#include "stdafx.h"
#include "TimeMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr() :
	m_IICurCount{},
	m_IIPrevCount{},
	m_IIFrequency{},
	m_dDT(0.0f),
	m_FPS(0.0f),
	m_dAcc(0.0f),
	m_iCallCount(0),
	m_iFPS(0) {}

CTimeMgr::~CTimeMgr()
{
	Release();
}

void CTimeMgr::Initialize()
{
	QueryPerformanceCounter(&m_IIPrevCount);
	QueryPerformanceFrequency(&m_IIFrequency);
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_IICurCount);

	m_dDT = (double)(m_IICurCount.QuadPart - m_IIPrevCount.QuadPart) / (double)m_IIFrequency.QuadPart;
	m_IIPrevCount = m_IICurCount;
}

void CTimeMgr::Late_Update()
{
}

void CTimeMgr::Render()
{
}

void CTimeMgr::Release()
{
}

