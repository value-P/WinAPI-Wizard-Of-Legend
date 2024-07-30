#pragma once

#include "Obj.h"

class CCollsionMgr
{
public:
	CCollsionMgr();
	~CCollsionMgr();

public:
	static void	Collision_RectEx(list<CObj*> Dst, list<CObj*> Src);
	static bool	Check_Rect(INFO pDst, INFO pSrc, float* pX, float* pY);
	static bool	Check_Sphere(CObj* pDst, CObj* pSrc);
	static void Collision_Sphere(list<CObj*> Dst, list<CObj*> Src);
};

