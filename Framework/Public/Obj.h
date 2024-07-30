#pragma once
#include "stdafx.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();
public:
	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }

	INFO		Get_Info() { return m_tInfo; }
	void		Set_Info(INFO _info) { m_tInfo = _info; }

	INFO		Get_ColInfo() { return m_tColInfo; }
	void		Set_ColInfo(INFO _colInfo) { m_tColInfo = _colInfo; } // 콜라이더 인포 세팅함수

	RECT		Get_Rect() { return m_tRect; }

	INFO		Get_TileColInfo() { return m_tTileColInfo; }

	Vector2D	Get_Pos() { return Vector2D(m_tInfo.fX, m_tInfo.fY); }
	void		Set_Pos(float fx, float fy) { m_tInfo.fX = fx; m_tInfo.fY = fy; }

	Vector2D	Get_TileColPos() { return Vector2D(m_tTileColInfo.fX, m_tTileColInfo.fY); }

	bool		Get_Dead() { return m_bDead; }
	void		Set_Dead() { m_bDead = true; }

	void		Set_FrameInfo(FRAME _frameInfo) { m_tFrame = _frameInfo; }
	void		Set_FrameKey(std::wstring pFrameKey) { m_pFrameKey = pFrameKey; }
	std::wstring		Get_FrameKey() { return m_pFrameKey; }

	float		Get_HpRate() { return m_fCurHp / m_fMaxHp; }
	void		Set_Hp(float _maxHp) { m_fMaxHp = _maxHp; m_fCurHp = _maxHp; }
	void		Plus_Hp(float _hp) { m_fCurHp = min(m_fCurHp + _hp, m_fMaxHp); }

	virtual void		Get_Damage(float _dmg, CObj* _pSrc);

	float		Get_ManaRate() { return m_fCurMana / m_fMaxMana; }
	void		Set_Mana(float _maxMana) { m_fMaxMana = _maxMana; }
	void		Lose_Mana(float _lose) { m_fCurMana = max(0.f, m_fCurMana - _lose); }

	void		Get_Mana(float _mana);

	int			Get_Money() { return m_iMoney; }
	void		Take_Money(int _money);
	void		Lose_Money(int _money) { m_iMoney -= _money; }

	void		Set_Angle(float _angle) { m_fAngle = _angle; }
	float		Get_Angle() { return m_fAngle; }

	RENDERID	Get_RenderID() { return m_eRenderID; }
	void		Set_RenderID(RENDERID _renderID) { m_eRenderID = _renderID; }

	OBJ_ID		Get_ObjID() { return m_eObjID; }
	void		Set_ObjID(OBJ_ID _ObjID) { m_eObjID = _ObjID; }

	float		Get_Attack() { return m_fAttack; }
	void		Set_Attack(float _attack) { m_fAttack = _attack; }

	void		Drop_Money();

public:
	virtual void		Initialize()			PURE;
	virtual int			Update()				PURE;
	virtual void		Late_Update()			PURE;
	virtual void		Render(HDC hDC)			PURE;
	virtual void		Release()				PURE;

public:
	virtual void		Debug_Render(HDC hDC)	PURE;

protected:
	virtual void		Anim_Update()			PURE;

protected:
	void		Update_Rect();
	void		Update_TileColRect();

	// 애니메이션이 한번 끝까지 가면 1반환(ANIM_END)
	int			Move_Frame();

protected:
	// 렌더를 위한 정보
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;

	// 충돌을 위한 정보
	Vector2D	m_vColOffset;
	INFO		m_tColInfo;
	RECT		m_tColRect;

	// 타일 충돌용
	Vector2D	m_vTileColOffset; // 타일 콜라이더가 중점에서 어느정도 떨어진 곳에 있을 것인지
	INFO		m_tTileColInfo;
	RECT		m_tTileColRect;

	// 그림자
	std::wstring		m_wsShadowKey;
	INFO		m_tShadowInfo;
	RECT		m_tShadowRect;
	float		m_fShdowOffset;

	// 움직임에 사용되는 방향벡터
	Vector2D	m_vDir;

	// 렌더시 보고있는 방향을 알기 위한 변수
	Vector2D	m_vLookDir;

	CObj*		m_pTarget;
	std::wstring		m_pFrameKey;

	// 스탯요소
	float		m_fSpeed;
	float		m_fAttack;
	//-----------------------
	float		m_fCurHp;
	float		m_fMaxHp;
	//-----------------------
	float		m_fCurMana;
	float		m_fMaxMana;

	// 재화
	int			m_iMoney;

	// 상태요소
	bool		m_bDead;

	// 각도
	float		m_fAngle;

	RENDERID	m_eRenderID = GAMEOBJECT;
	OBJ_ID		m_eObjID;

};

