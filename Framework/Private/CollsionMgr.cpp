#include "stdafx.h"
#include "CollsionMgr.h"


CCollsionMgr::CCollsionMgr()
{
}
CCollsionMgr::~CCollsionMgr()
{
}


void CCollsionMgr::Collision_RectEx(list<CObj*> Dst, list<CObj*> Src)
{

}

bool CCollsionMgr::Check_Rect(INFO pDst, INFO pSrc,
	float* pX, float* pY)
{
	float		fWidth = abs(pDst.fX - pSrc.fX);
	float		fHeight = abs(pDst.fY - pSrc.fY);

	float		fCX = (pDst.fCX + pSrc.fCX) * 0.5f;
	float		fCY = (pDst.fCY + pSrc.fCY) * 0.5f;

	if ((fCX > fWidth) && (fCY > fHeight))
	{
		*pX = fCX - fWidth;
		*pY = fCY - fHeight;

		return true;
	}

	return false;
}

bool CCollsionMgr::Check_Sphere(CObj* pDst, CObj* pSrc)
{
	float		fWidth = fabs(pDst->Get_Info().fX - pSrc->Get_Info().fX);
	float		fHeight = fabs(pDst->Get_Info().fY - pSrc->Get_Info().fY);

	float		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float		fRadius = (pDst->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;

	return fRadius >= fDiagonal;
}

void CCollsionMgr::Collision_Sphere(list<CObj*> Dst, list<CObj*> Src)
{

	for (auto& DstList : Dst)
	{
		for (auto& SrcList : Src)
		{
			if (Check_Sphere(DstList, SrcList))
			{
				DstList->Set_Dead();
				SrcList->Set_Dead();
			}
		}
	}

}
