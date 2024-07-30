#pragma once
#include "Obj.h"

class CSkill : public CObj
{
public:
	CSkill();
	virtual ~CSkill();

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize()		PURE;
	virtual int Update()			PURE;
	virtual void Late_Update()		PURE;
	virtual void Render(HDC hDC)	PURE;
	virtual void Release()			PURE;
	virtual void Debug_Render(HDC hDC) PURE;
	virtual void Anim_Update() PURE;

	virtual SKILL_TYPE Get_SkillType() PURE;


public:
	virtual void InitFrameOption() {}
	void		Set_Owner(CObj* _pOwner) { m_pOwner = _pOwner; }
	float		Get_CoolTime() { return m_fCoolTime; }
	void		Set_Combo(int _curCombo) { m_iCurCombo = _curCombo; }
	void		Set_Dir(Vector2D _dir) { m_vDir = _dir; }
	void		Set_StartPos(Vector2D _startPos) { m_vStartPos = _startPos; }
	void		Set_Duration(float _duration) { m_fDuration = _duration; }

protected:
	CObj*		m_pOwner;

	int			m_iCurCombo;  // 해당 스킬이 몇콤보짜리인지

	float		m_fCoolTime;
	PlayerSkill	m_eSkillType;
	float		m_fOffset;		// 시전시 얼마만큼 떨어져서 생성될지
	float		m_fDuration;	// 유지시간

	bool		m_bSkillEnded; // 전프레임에서 마지막 애니메이션이 재생되었는가

	list<CObj*> m_TargetList;

	Vector2D	m_vStartPos; // 시작 위치가 필요할때 사용할 변수
};

