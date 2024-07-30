#include "stdafx.h"
#include "ScrewBullet.h"


CScrewBullet::CScrewBullet() : m_fRotAngle(0.f),
								m_fRotSpeed(30.f), 
								m_fDistance(20.f), 
								m_bStart(true)
{
	ZeroMemory(&m_tCenter, sizeof(POINT));
}

CScrewBullet::~CScrewBullet()
{
	Release();
}

void CScrewBullet::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;

	
}

int CScrewBullet::Update()
{

	if (m_bStart)
	{
		m_tCenter.x = (LONG)m_tInfo.fX;
		m_tCenter.y = (LONG)m_tInfo.fY;
		m_bStart = false;
	}

	m_tCenter.x += LONG(m_fSpeed * cosf(m_fAngle * (PI / 180.f)));
	m_tCenter.y -= LONG(m_fSpeed * sinf(m_fAngle * (PI / 180.f)));

	m_fRotAngle += m_fRotSpeed;

	m_tInfo.fX = m_tCenter.x + m_fDistance * cosf(m_fRotAngle * (PI / 180.f));
	m_tInfo.fY = m_tCenter.y - m_fDistance * sinf(m_fRotAngle * (PI / 180.f));

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CScrewBullet::Late_Update()
{
}

void CScrewBullet::Render(HDC hDC)
{
	Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CScrewBullet::Release()
{
}
