#include "stdafx.h"
#include "FloatRock.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "CollsionMgr.h"
#include "ObjMgr.h"
#include "EarthCrack.h"

CFloatRock::CFloatRock()
    :m_bStartThrow(false)
{
}

CFloatRock::~CFloatRock()
{
}

void CFloatRock::Initialize()
{
    m_fSpeed = 1500.f;

    m_tInfo.fX = m_pOwner->Get_Pos().x + 120.f * cosf(m_fAngle * DEG2RAD);
    m_tInfo.fY = m_pOwner->Get_Pos().y - 120.f * sinf(m_fAngle * DEG2RAD);
    m_tInfo.fCX = 55.f;
    m_tInfo.fCY = 78.f;

    m_tColInfo = m_tInfo;

    m_pFrameKey = L"BOSS_BOXATTACK";
    m_tFrame.SetInfo(0, 0, 0, 1000);
    m_tFrame.Stop_Frame();

    m_vStartPos = Vector2D(m_tInfo.fX, m_tInfo.fY);

    // 바닥 부서지는 이펙트 생성
    CObj* pEffect = CAbstractFactory<CEarthCrack>::Create_Obj(m_tInfo.fX, m_tInfo.fY + 10.f);
    pEffect->Set_RenderID(BACKGROUND);
    CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);

}

int CFloatRock::Update()
{
    if (m_bDead) return OBJ_DEAD;

    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    if (m_bStartThrow)
    {
        m_tInfo.fX += m_vDir.x * m_fSpeed * deltaTime;
        m_tInfo.fY += m_vDir.y * m_fSpeed * deltaTime;
    }
    else
    {
        if(m_tInfo.fY < m_vStartPos.y - 15.f)
            m_tInfo.fY -= 10.f * deltaTime;

        CObj* player = CObjMgr::Get_Instance()->Get_Player();
        m_vDir =  (player->Get_Pos() - Get_Pos()).Normalize();
    }

    __super::Update_Rect();
    return OBJ_NOEVENT;
}

void CFloatRock::Late_Update()
{
    if (Wall_Collisioned(m_tInfo.fX, m_tInfo.fY))
        Set_Dead();

    if (m_bStartThrow)
    {
        CObj* player = CObjMgr::Get_Instance()->Get_Player();
        float fx, fy;
        if (CCollsionMgr::Check_Rect(m_tColInfo, player->Get_ColInfo(), &fx, &fy))
        {
            player->Get_Damage(m_fAttack, this);
        }
    }

}

void CFloatRock::Render(HDC hDC)
{
    float scrollX = GET_SINGLE(CScrollMgr)->Get_ScrollX();
    float scrollY = GET_SINGLE(CScrollMgr)->Get_ScrollY();

    HDC hMemDC = GET_SINGLE(CBmpMgr)->Find_Img(m_pFrameKey);

    GdiTransparentBlt(hDC,
        m_tRect.left + (int)scrollX,
        m_tRect.top + (int)scrollY,
        m_tInfo.fCX, 
        m_tInfo.fCY,
        hMemDC, 
        0, 0, 
        m_tInfo.fCX, 
        m_tInfo.fCY, 
        RGB(255, 0, 255));
}

void CFloatRock::Release()
{
}

void CFloatRock::Debug_Render(HDC hDC)
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

void CFloatRock::Anim_Update()
{
}

bool CFloatRock::Wall_Collisioned(float fX, float fY)
{
    list<CObj*> tempTileList;

    int startX = (fX - m_tColInfo.fCX * 0.5f) / TILECX;
    int startY = (fY - m_tColInfo.fCY * 0.5f) / TILECY;
    int endX = (fX + m_tColInfo.fCX * 0.5f) / TILECX;
    int endY = (fY + m_tColInfo.fCY * 0.5f) / TILECY;

    CTileMgr::Get_Instance()->Get_Range_TileList(startX, startY, endX, endY, &tempTileList);

    for (auto& iter : tempTileList)
    {
        TILE_ID eType = dynamic_cast<CTile*>(iter)->Get_TileType();

        float fX, fY;

        if (eType == TILE_WALL)
        {
            if (CCollsionMgr::Check_Rect(iter->Get_Info(), m_tColInfo, &fX, &fY))
            {
                return true;
            }
        }
    }


    return false;

}

SKILL_TYPE CFloatRock::Get_SkillType()
{
    return ST_THROW;
}

