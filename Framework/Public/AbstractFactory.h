#pragma once
#include "stdafx.h"
#include "Obj.h"
#include "UI.h"
#include "Structure.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {}

public:
	static CObj* Create_Obj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create_Obj(float fx, float fy)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(fx, fy);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Obj(float fx, float fy, std::wstring frameKey)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(fx, fy);
		pObj->Set_FrameKey(frameKey);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Obj(INFO info, std::wstring frameKey, FRAME frameInfo)
	{
		CObj* pObj = new T;
		pObj->Set_Info(info);
		pObj->Set_FrameKey(frameKey);
		pObj->Set_FrameInfo(frameInfo);
		pObj->Initialize();
		return pObj;
	}

	// 체력있는 오브젝토 생성 함수
	static CObj* Create_Obj(float fx, float fy, float _maxHp, float _maxMana, float _attack)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(fx, fy);
		pObj->Set_Hp(_maxHp);
		pObj->Set_Mana(_maxMana);
		pObj->Set_Attack(_attack);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Obj(float fx, float fy, float angle, Vector2D _Dir)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(fx, fy);
		pObj->Set_Angle(angle);
		static_cast<CSkill*>(pObj)->Set_Dir(_Dir);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create_Obj( Vector2D _Dir, CObj* _Owner, float _duration)
	{
		CObj* pObj = new T;
		static_cast<CSkill*>(pObj)->Set_Dir(_Dir);
		static_cast<CSkill*>(pObj)->Set_Owner(_Owner);
		static_cast<CSkill*>(pObj)->Set_Duration(_duration);
		pObj->Initialize();
		return pObj;
	}

	// 구조물 Create_Obj
	static CObj* Create_Obj_Structure(INFO _structureInfo, STRUCTURE_ID _structureType, std::wstring _lpFrameKey)
	{
		CObj* pObj = new T;
		pObj->Set_Info(_structureInfo);
		pObj->Set_FrameKey(_lpFrameKey);
		static_cast<CStructure*>(pObj)->Set_STID(_structureType);
		pObj->Initialize();

		// static구조물만 충돌체 입력
		switch (_structureType)
		{
		case ST_STATIC:
			pObj->Set_ColInfo(_structureInfo);
			break;
		case ST_DECO:
			pObj->Set_RenderID(BACKGROUND);
			break;
		}

		
		return pObj;
	}

};