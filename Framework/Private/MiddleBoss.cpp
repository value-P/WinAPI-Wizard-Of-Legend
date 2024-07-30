#include "stdafx.h"
#include "MiddleBoss.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "CollsionMgr.h"
#include "TileMgr.h"
#include "Skill.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Effect.h"
#include "UIMgr.h"
#include "BossHpBar.h"
#include "M_VoidStorm.h"
#include "M_Shark.h"
#include "SummonEffect.h"

MiddleBoss::MiddleBoss()
    :m_eCurState_Anim(BOSS_NONE),
    m_ePrevState_Anim(BOSS_NONE),
    m_fDetectRangle(0.f),
    m_eCurState(M_NONE),
    m_eCurPattern(B_P_NONE),
    m_fTime(0.f),
    m_fPatternTime(0.f),
    iPattern_1_Cnt(0),
    m_fPattern1Time(0.f),
    iPattern_2_Cnt(0),
    m_fPattern2Time(0.f),
    bPattern_3_Start(false),
    m_fDeadTime(0.f)
{}

MiddleBoss::~MiddleBoss()
{
    Release();
}

void MiddleBoss::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/BOSS_MIDDLE.bmp", L"BOSS_MIDDLE");

    Set_Hp(1000.f);

    m_eRenderID = GAMEOBJECT;
    m_eObjID = OBJ_MONSTER;

    // 애니메이션 관련 초기화
    m_tFrame.dwTime = GetTickCount();
    m_pFrameKey = L"BOSS_MIDDLE";
    m_tFrame.SetInfo(0, 0, 0, 200);
    // 몬스터 스탯관련 초기화
    m_fAttack = 15.f;
    m_fSpeed = 8.f;
    m_fDetectRangle = 150.f;
    // 몬스터 포지션 관련 초기화
    m_tInfo.fCX = 340.f;
    m_tInfo.fCY = 340.f;

    // 콜라이더 크기
    m_vColOffset = Vector2D(0.f, -20.f);
    m_tColInfo.fCX = 140.f;
    m_tColInfo.fCY = 210.f;

}

int MiddleBoss::Update()
{
    if (m_bDead) return OBJ_DEAD;

    // 타겟 찾을때까지 아무것도 실행하지 않도록
    if (!m_pTarget)
    {
        Detect_Target();
        __super::Update_Rect();
        __super::Update_TileColRect();
        return OBJ_NOEVENT;
    }

    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    switch (m_eCurState)
    {
    case M_IDLE:
        m_eCurState_Anim = BOSS_IDLE;
        m_fPatternTime += deltaTime;
        if (m_fPatternTime > 1.5f)
        {
            m_fPatternTime = 0.f;
            m_eCurState = M_SKILL;
        }
        break;
    case M_SKILL:
        Pattern();
        break;
    case M_DEAD:
        m_eCurState_Anim = BOSS_DEAD;

        m_fDeadTime += deltaTime;
        if (m_fDeadTime > 4.f)
            Set_Dead();
        break;
    }

    __super::Update_Rect();
    __super::Update_TileColRect();
    return OBJ_NOEVENT;
}

void MiddleBoss::Late_Update()
{
    Anim_Update();

    // 상태변화를 감지하여 변화된 상태로 변경시켜주는 함수
    SetFrameByState();

    if (m_fCurHp <= 0.f) m_eCurState = M_DEAD;

    // 애니메이션 프레임이 한번끝까지 가면 ANIM_END반환
    // 그 외에는 해당 프레임 번호 반환
    int animEnd = __super::Move_Frame();

    if (animEnd == ANIM_END)
    {
        if (m_eCurState_Anim == BOSS_HIT)
            m_eCurState = M_IDLE;
        else if (m_eCurState_Anim == BOSS_DEAD)
        {
            if (!m_tFrame.stopFrame)
            {
                m_tFrame.Stop_Frame();
                m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
            }
        }
    }
}

void MiddleBoss::Render(HDC hDC)
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

void MiddleBoss::Release()
{
}

void MiddleBoss::Debug_Render(HDC hDC)
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

void MiddleBoss::Anim_Update()
{
}

void MiddleBoss::Get_Damage(float _dmg, CObj* _pSrc)
{
    m_fCurHp -= _dmg;

    // 피격 이펙트 생성
    Vector2D hitDir = _pSrc->Get_Pos() - Get_Pos();
    float	 Angle = hitDir.ToAngle_Rad();

    FRAME frameInfo;

    if (m_fAngle >= 7 * PI / 4 || m_fAngle < PI / 4)
        frameInfo.SetInfo(0, 4, 0, 300);
    else if (m_fAngle >= PI / 4 && m_fAngle < 3 * PI / 4)
        frameInfo.SetInfo(0, 4, 1, 300);
    else if (m_fAngle >= 3 * PI / 4 && m_fAngle < 5 * PI / 4)
        frameInfo.SetInfo(0, 4, 2, 300);
    else if (m_fAngle >= 5 * PI / 4 && m_fAngle < 7 * PI / 4)
        frameInfo.SetInfo(0, 4, 3, 300);

    CSoundMgr::Get_Instance()->Play_Sound(L"BOSS_WIND_HIT_1.mp3", SOUND_MONSTER_HIT, 1.f);
    CObj* pEffect = CAbstractFactory<CEffect>::Create_Obj(INFO(m_tInfo.fX, m_tInfo.fY, 115.f, 115.f), L"HITEFFECT", frameInfo);
    CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
}

bool MiddleBoss::CheckTile(float fX, float fY)
{
    list<CObj*> tempTileList;

    float tempX = fX;
    float tempY = fY;

    INFO tempTileColInfo{ tempX,tempY,m_tTileColInfo.fCX,m_tTileColInfo.fCY };

    int startX = (tempX - 16.f) / TILECX;
    int startY = (tempY - 16.f) / TILECY;
    int endX = (tempX + 16.f) / TILECX;
    int endY = (tempY + 16.f) / TILECY;

    CTileMgr::Get_Instance()->Get_Range_TileList(startX, startY, endX, endY, &tempTileList);

    for (auto& iter : tempTileList)
    {
        TILE_ID eType = dynamic_cast<CTile*>(iter)->Get_TileType();

        float fX, fY;

        if (eType == TILE_WALL)
        {
            if (CCollsionMgr::Check_Rect(iter->Get_Info(), tempTileColInfo, &fX, &fY))
            {
                return true;
            }
        }
    }

    return false;

}

void MiddleBoss::Pattern()
{
    if (m_eCurPattern == B_P_NONE)
    {
        int randNum = rand() % (BOSS_P_END);
        m_eCurPattern = (BOSS_PATTERN)randNum;
        //m_eCurPattern = BOSS_3;
    }
    else
    {
        switch (m_eCurPattern)
        {
        case BOSS_1:
        {
            int Result = Pattern_first();
            if (Result == PATTERN_END)
            {
                m_eCurState = M_IDLE;
                m_eCurPattern = B_P_NONE;
            }
        }
        break;
        case BOSS_2:
        {
            int Result = Pattern_second();
            if (Result == PATTERN_END)
            {
                m_eCurState = M_IDLE;
                m_eCurPattern = B_P_NONE;
            }
        }
        break;
        case BOSS_3:
            int Result = Pattern_third();
            if (Result == PATTERN_END)
            {
                m_eCurState = M_IDLE;
                m_eCurPattern = B_P_NONE;
            }
            break;
        }
    }
}

int MiddleBoss::Pattern_first()
{
    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    // 애니메이션 변경
    if (m_eCurState_Anim != BOSS_SKILL1)
    {
        m_eCurState_Anim = BOSS_SKILL1;
    }
    else
    {
        // 애니 프레임이 안멈춰져 있다면
        if (!m_tFrame.stopFrame)
        {
            // 마지막 프레임에 정지
            int frameCur = m_tFrame.Get_CurFrame();
            if (frameCur >= m_tFrame.iFrameEnd)
            {
                m_tFrame.Stop_Frame();
            }
        }
        // 패턴 시작
        else
        {
            m_fPattern1Time += deltaTime;
            float angle = 0.f;
            if (m_fPattern1Time > 1.5f)
            {
                m_fPattern1Time = 0.f;
                switch (iPattern_1_Cnt)
                {
                case 0:
                    for (int i = 0; i < 6; ++i)
                    {
                        angle = (60 * i) + 30.f;
                        if (angle >= 360.f)
                            angle -= 360;

                        CM_VoidStorm* storm = new CM_VoidStorm;
                        storm->Set_Owner(this);
                        storm->Set_Angle(angle);
                        storm->Set_Target(m_pTarget);
                        storm->Initialize();
                        CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, storm);
                    }
                    ++iPattern_1_Cnt;
                    break;
                case 1:
                    for (int i = 0; i < 6; ++i)
                    {
                        angle = (60 * i) + 60.f;
                        if (angle >= 360.f)
                            angle -= 360;

                        CM_VoidStorm* storm = new CM_VoidStorm;
                        storm->Set_Owner(this);
                        storm->Set_Angle(angle);
                        storm->Set_Target(m_pTarget);
                        storm->Initialize();
                        CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, storm);
                    }
                    ++iPattern_1_Cnt;
                    break;
                case 2:
                    for (int i = 0; i < 6; ++i)
                    {
                        angle = (60 * i) + 30.f;
                        if (angle >= 360.f)
                            angle -= 360;

                        CM_VoidStorm* storm = new CM_VoidStorm;
                        storm->Set_Owner(this);
                        storm->Set_Angle(angle);
                        storm->Set_Target(m_pTarget);
                        storm->Initialize();
                        CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, storm);
                    }
                    iPattern_1_Cnt = 0;
                    m_tFrame.Resume_Frame();
                    return PATTERN_END;
                }
            }
        }
    }

    return PATTERN_ING;
}

int MiddleBoss::Pattern_second()
{
    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    // 애니메이션 변경
    if (m_eCurState_Anim != BOSS_SKILL2)
    {
        m_eCurState_Anim = BOSS_SKILL2;
    }
    else
    {
        // 애니 프레임이 안멈춰져 있다면
        if (!m_tFrame.stopFrame)
        {
            // 마지막 프레임에 정지
            int frameCur = m_tFrame.Get_CurFrame();
            if (frameCur >= m_tFrame.iFrameEnd)
            {
                m_tFrame.Stop_Frame();
            }
        }
        // 패턴 시작
        else
        {
            m_fPattern2Time += deltaTime;
            float angle = 0.f;
            CM_Shark* shark = nullptr;
            if (m_fPattern2Time > 1.5f)
            {
                m_fPattern2Time = 0.f;
                switch (iPattern_2_Cnt)
                {
                case 0:
                    ++iPattern_2_Cnt;
                    shark = new CM_Shark;
                    shark->Set_Owner(this);
                    shark->Set_Target(m_pTarget);
                    shark->Initialize();
                    CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, shark);
                    break;
                case 1:
                    ++iPattern_2_Cnt;
                    shark = new CM_Shark;
                    shark->Set_Owner(this);
                    shark->Set_Target(m_pTarget);
                    shark->Initialize();
                    CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, shark);
                    break;
                case 2:
                    iPattern_2_Cnt = 0;
                    shark = new CM_Shark;
                    shark->Set_Owner(this);
                    shark->Set_Target(m_pTarget);
                    shark->Initialize();
                    CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, shark);
                    m_tFrame.Resume_Frame();
                    return PATTERN_END;
                }
            }
        }

        return PATTERN_ING;
    }
}

int MiddleBoss::Pattern_third()
{
    // 애니메이션 변경
    if (m_eCurState_Anim != BOSS_JUMPATK)
    {
        m_eCurState_Anim = BOSS_JUMPATK;
    }
    else
    {
        // 애니 프레임이 안멈춰져 있다면
        if (!m_tFrame.stopFrame)
        {
            // 마지막 프레임에 정지
            int frameCur = m_tFrame.Get_CurFrame();
            if (frameCur >= m_tFrame.iFrameEnd)
            {
                m_tFrame.Stop_Frame();
            }
        }
        else
        {
            // 소환_1
            CSummonEffect* pEffect_1 = new CSummonEffect;
            pEffect_1->Set_Pos(m_tInfo.fX + 150.f, m_tInfo.fY);
            static_cast<CSummonEffect*>(pEffect_1)->Set_MonsterType(MONSTER_BALL);
            pEffect_1->Initialize();
            GET_SINGLE(CObjMgr)->Add_Object(OBJ_EFFECT, pEffect_1);
            // 소환_2
            CSummonEffect* pEffect_2 = new CSummonEffect;
            pEffect_2->Set_Pos(m_tInfo.fX - 150.f, m_tInfo.fY);
            static_cast<CSummonEffect*>(pEffect_2)->Set_MonsterType(MONSTER_BALL);
            pEffect_2->Initialize();
            GET_SINGLE(CObjMgr)->Add_Object(OBJ_EFFECT, pEffect_2);

            m_tFrame.Resume_Frame();
            return PATTERN_END;
        }

    }

    return PATTERN_ING;

}

void MiddleBoss::SetFrameByState()
{
    // 상태 변화 없다면 나가기
    if (m_eCurState_Anim == m_ePrevState_Anim)
        return;

    if (m_ePrevState_Anim == BOSS_DEAD) return;

    // 이전 상태 갱신
    m_ePrevState_Anim = m_eCurState_Anim;

    switch (m_eCurState_Anim)
    {
    case BOSS_IDLE:
        m_tFrame.SetInfo(0, 0, 1, 200);
        break;
    case BOSS_SKILL1:
        m_tFrame.SetInfo(0, 4, 2, 400);
        break;
    case BOSS_SKILL2:
        m_tFrame.SetInfo(0, 4, 3, 400);
        break;
    case BOSS_JUMPATK:
        m_tFrame.SetInfo(0, 2, 4, 400);
        break;
    case BOSS_DEAD:
        m_tFrame.SetInfo(0, 0, 0, 200);
        break;
    }
}

void MiddleBoss::Detect_Target()
{
    // 이미 감지된 적이 있으면 실행 안함
    if (m_pTarget) return;

    m_pTarget = CObjMgr::Get_Instance()->Get_NearestTarget(this, OBJ_PLAYER, m_fDetectRangle);

    if (m_pTarget)
    {
        if (m_pTarget)
        {
            CBossHpBar* pUI = new CBossHpBar();
            pUI->Set_Owner(this);
            pUI->Set_FrameName(L"MIDDLEBOSS_NAMEBAR");
            pUI->Set_Pos(WINCX * 0.5f, 55.f);
            pUI->Initialize();
            GET_SINGLE(CUIMgr)->Add_UI(UI_NORMAL, pUI);

            m_eCurState = M_IDLE;
        }

    }
}

float MiddleBoss::Get_TargetAngle()
{
    Vector2D dir = (m_pTarget->Get_Pos() - Get_Pos()).Normalize();

    float tempAngle = dir.ToAngle_Rad();

    return tempAngle * RAD2DEG;
}
