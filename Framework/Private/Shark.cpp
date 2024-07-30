#include "stdafx.h"
#include "Shark.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"

CShark::CShark()
	:m_fTime(0.f), Activated(false)
{
}

CShark::~CShark()
{
	Release();
}

void CShark::Initialize()
{
	m_eRenderID = EFFECT;
	m_eObjID = OBJ_SKILL;

	m_tFrame.SetInfo(0, 16, 0, 100);

	m_fAttack = m_pOwner->Get_Attack() * 20.f;

	m_fCoolTime = 10.f;
	m_eSkillType = SKILL_SHARK;
	m_pFrameKey = L"Shark";
	m_tInfo.fCX = 360.f;
	m_tInfo.fCY = 360.f;
	m_tColInfo.fCX = 360.f;
	m_tColInfo.fCY = 360.f;


	GET_SINGLE(CSoundMgr)->Play_Sound_NoInterval(L"Kong.mp3", SOUND_EFFECT_3, 0.6f);
}

int CShark::Update()
{
	if (m_bDead) return OBJ_DEAD;

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CShark::Late_Update()
{

	int iResult = __super::Move_Frame();

	if (iResult == ANIM_END)
		Set_Dead();
	else
	{
		if (m_tFrame.Get_CurFrame() == 7 && !Activated)
		{
			Activated = true;
			GET_SINGLE(CSoundMgr)->Play_Sound_NoInterval(L"SharkUp.mp3", SOUND_EFFECT_3, 0.6f);
			list<CObj*> hitObjects;
			// 데미지
			GET_SINGLE(CObjMgr)->CheckCollision(this, OBJ_MONSTER, &hitObjects);
			for (auto& iter : hitObjects)
				iter->Get_Damage(m_fAttack, this);
		}
	}
}

void CShark::Render(HDC hDC)
{
	HDC hCutDC = CBmpMgr::Get_Instance()->Find_Img(L"KONG");

	GdiTransparentBlt(hDC,
		0, 200,
		372, 279,
		hCutDC,
		0, 0,
		372, 279,
		RGB(0, 0, 0));

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

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

void CShark::Release()
{
}

void CShark::Debug_Render(HDC hDC)
{
	int	fScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int	fScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

	// 일반 콜라이더
	MoveToEx(hDC, (int)m_tColRect.left + fScrollX, (int)m_tColRect.top + fScrollY, nullptr);
	LineTo(hDC, (int)m_tColRect.right + fScrollX, (int)m_tColRect.top + fScrollY);
	LineTo(hDC, (int)m_tColRect.right + fScrollX, (int)m_tColRect.bottom + fScrollY);
	LineTo(hDC, (int)m_tColRect.left + fScrollX, (int)m_tColRect.bottom + fScrollY);
	LineTo(hDC, (int)m_tColRect.left + fScrollX, (int)m_tColRect.top + fScrollY);

	SelectObject(hDC, hOldPen); // 기존에 사용하던것(Newpen)을 리턴함
	DeleteObject(hNewPen);
}

void CShark::Anim_Update()
{
}

SKILL_TYPE CShark::Get_SkillType()
{
	return ST_RANGE;
}
