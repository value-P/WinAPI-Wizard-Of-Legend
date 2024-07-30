#include "stdafx.h"
#include "Archor.h"
#include "stdafx.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
#include "DamageEffect.h"
#include "Bow.h"

CArchor::CArchor()
    :m_eCurState_Anim(AR_IDLE), m_ePrevState_Anim(AR_IDLE), m_eCurState(M_IDLE), m_fTime(0.f), m_pBow(nullptr)
    , m_fTime2(0.f)
{
}

CArchor::~CArchor()
{
    Release();
}

void CArchor::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/ARCHER_LEFT.bmp", L"ARCHER_LEFT");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/ARCHER_RIGHT.bmp", L"ARCHER_RIGHT");

    Set_Hp(200.f);

    m_eRenderID = GAMEOBJECT;
    m_eObjID = OBJ_MONSTER;

    // 애니메이션 관련 초기화
    m_tFrame.dwTime = GetTickCount();
    m_pFrameKey = L"ARCHER_RIGHT";
    m_tFrame.SetInfo(0, 0, (int)AR_IDLE, 700);
    // 몬스터 스탯관련 초기화
    m_fAttack = 5.f;
    m_fSpeed = 150.f;
    // 몬스터 포지션 관련 초기화
    m_tInfo.fCX = 100.f;
    m_tInfo.fCY = 100.f;

    m_vTileColOffset = Vector2D(0.f, -32.f);
    m_tTileColInfo.fCX = 32.f;
    m_tTileColInfo.fCY = 32.f;

    // 콜라이더 크기
    m_tColInfo.fCX = 35.f;
    m_tColInfo.fCY = 90.f;

    m_pTarget = GET_SINGLE(CObjMgr)->Get_Player();
}

int CArchor::Update()
{
    if (m_bDead)
    {
        __super::Drop_Money();
        return OBJ_DEAD;
    }

    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    Vector2D targetDir = m_pTarget->Get_Pos() - Get_Pos();
    m_vDir = (targetDir).Normalize();

    if (m_fCurHp <= 0.f)
        m_eCurState = M_DEAD;

    if (m_eCurState != M_HIT && m_eCurState != M_ATTACK && m_eCurState != M_DEAD)
    {
        // 플레이어와의 거리가 32 이내일때 공격
        if (targetDir.Magnitude() < 200.f)
            m_eCurState = M_ATTACK;
        // 그 외에는 추적
        else
            m_eCurState = M_MOVE;
    }

    switch (m_eCurState)
    {
    case M_IDLE:
        m_eCurState_Anim = AR_IDLE;
        break;
    case M_MOVE:
    {
        m_tInfo.fX += m_fSpeed * deltaTime * m_vDir.x;
        m_tInfo.fY += m_fSpeed * deltaTime * m_vDir.y;
        m_eCurState_Anim = AR_RUN;
    }
    break;
    case M_ATTACK:
        if (m_pBow == nullptr)
        {
            m_pBow = new CBow;
            m_pBow->Set_Owner(this);
            m_pBow->Set_Target(GET_SINGLE(CObjMgr)->Get_Player());
            m_pBow->Initialize();
            GET_SINGLE(CObjMgr)->Add_Object(OBJ_SKILL, m_pBow);
        }
        m_eCurState_Anim = AR_ATTACK;
        break;
    case M_HIT:
        if (m_eCurState == M_ATTACK || m_eCurState == M_DEAD) break;
        m_eCurState_Anim = AR_HIT;

        m_fTime += deltaTime;
        if (m_fTime > 0.5f)
        {
            CSoundMgr::Get_Instance()->Play_Sound(L"ENEMY_HITED_1.mp3", SOUND_MONSTER_HIT, 1.f);
            FRAME frameInfo;
            frameInfo.SetInfo(0, 4, 0, 100);
            CObj* pEffect = CAbstractFactory<CEffect>::Create_Obj(INFO(m_tInfo.fX, m_tInfo.fY, 115.f, 115.f), L"HITEFFECT", frameInfo);
            CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
        }
        break;
    case M_DEAD:
        if (m_pBow != nullptr)
        {
            m_pBow->Set_Dead();
            m_pBow = nullptr;
        }
        m_eCurState_Anim = AR_DEAD;
        break;
    }

    __super::Update_Rect();
    __super::Update_TileColRect();
    return OBJ_NOEVENT;

}

void CArchor::Late_Update()
{
    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    Anim_Update();

    // 상태변화를 감지하여 변화된 상태로 변경시켜주는 함수
    SetFrameByState();

    // 애니메이션 프레임이 한번끝까지 가면 ANIM_END반환
    // 그 외에는 해당 프레임 번호 반환
    int animEnd = __super::Move_Frame();

    if (animEnd == ANIM_END)
    {
        switch (m_eCurState_Anim)
        {
        case AR_IDLE:
            break;
        case AR_RUN:
            break;
        case AR_ATTACK:
            Shot();
            m_eCurState = M_IDLE;
            break;
        case AR_HIT:
            m_eCurState = M_IDLE;
            GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_MONSTER_HIT);
            break;
        case AR_DEAD:
            Set_Dead();
            break;
        }
    }
    else
    {
        switch (m_eCurState_Anim)
        {
        case AR_ATTACK:
            if (!m_tFrame.stopFrame)
            {
                if (m_tFrame.Get_CurFrame() == 3)
                    m_tFrame.Stop_Frame();
            }
            else
            {
                m_fTime2 += deltaTime;
                if (m_fTime2 > 0.5f)
                {
                    m_tFrame.Resume_Frame();
                    m_fTime2 = 0.f;
                }
            }
            break;
        }
    }
}

void CArchor::Render(HDC hDC)
{
    float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

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

void CArchor::Release()
{
}

void CArchor::Debug_Render(HDC hDC)
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

    // 펜 재설정
    hNewPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    hOldPen = (HPEN)SelectObject(hDC, hNewPen);
    // 타일 콜라이더
    MoveToEx(hDC, m_tTileColRect.left + fScrollX, m_tTileColRect.top + fScrollY, nullptr);
    LineTo(hDC, m_tTileColRect.right + fScrollX, m_tTileColRect.top + fScrollY);
    LineTo(hDC, m_tTileColRect.right + fScrollX, m_tTileColRect.bottom + fScrollY);
    LineTo(hDC, m_tTileColRect.left + fScrollX, m_tTileColRect.bottom + fScrollY);
    LineTo(hDC, m_tTileColRect.left + fScrollX, m_tTileColRect.top + fScrollY);


    SelectObject(hDC, hOldPen); // 기존에 사용하던것(Newpen)을 리턴함
    DeleteObject(hNewPen);
}

void CArchor::Anim_Update()
{

}

void CArchor::Get_Damage(float _dmg, CObj* _pSrc)
{
    if (m_eCurState == M_DEAD) return;

    Vector2D nuckBackDir = (Get_Pos() - _pSrc->Get_Pos()).Normalize();
    m_tInfo.fX -= nuckBackDir.x * 1.5f;
    m_tInfo.fY -= nuckBackDir.y * 1.5f;

    m_fCurHp -= _dmg;

    m_eCurState = M_HIT;
}

void CArchor::Shot()
{
    if (m_pBow)
    {
        m_pBow->Start_Shoot();
        m_pBow = nullptr;
    }
}

void CArchor::SetFrameByState()
{
    if (m_vDir.x > 0) m_pFrameKey = L"ARCHER_RIGHT";
    else m_pFrameKey = L"ARCHER_LEFT";

    // 상태 변화 없다면 나가기
    if (m_eCurState_Anim == m_ePrevState_Anim)
        return;

    // 이전 상태 갱신
    m_ePrevState_Anim = m_eCurState_Anim;

    switch (m_eCurState_Anim)
    {
    case AR_IDLE:
        m_tFrame.SetInfo(0, 0, (int)AR_IDLE, 200);
        break;
    case AR_RUN:
        m_tFrame.SetInfo(0, 4, (int)AR_RUN, 200);
        break;
    case AR_ATTACK:
        m_tFrame.SetInfo(0, 2, (int)AR_ATTACK, 500);
        break;
    case AR_HIT:
        m_tFrame.SetInfo(0, 1, (int)AR_HIT, 200);
        break;
    case AR_DEAD:
        m_tFrame.SetInfo(0, 5, (int)AR_DEAD, 200);
        break;
    }
}
