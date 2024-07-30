#include "stdafx.h"
#include "Ball.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
#include "DamageEffect.h"
#include "CollsionMgr.h"

CBall::CBall()
    :m_eCurState_Anim(BA_IDLE), m_ePrevState_Anim(BA_IDLE), m_eCurState(M_IDLE), m_fTime(0.f)
    , m_fTime2(0.f), m_bTargeting(false), m_fShotTime(0.f), m_fRotateTime(0.f), m_fIdleTime(0.f)
{
}

CBall::~CBall()
{
    Release();
}

void CBall::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/WIZARD_BALL.bmp", L"WIZARD_BALL");

    Set_Hp(70.f);

    m_eRenderID = GAMEOBJECT;
    m_eObjID = OBJ_MONSTER;

    // 애니메이션 관련 초기화
    m_tFrame.dwTime = GetTickCount();
    m_pFrameKey = L"WIZARD_BALL";
    // 몬스터 스탯관련 초기화
    m_fAttack = 5.f;
    m_fSpeed = 650.f;
    // 몬스터 포지션 관련 초기화
    m_tInfo.fCX = 45.f;
    m_tInfo.fCY = 45.f;

    // 콜라이더 크기
    m_tColInfo = m_tInfo;

    m_pTarget = GET_SINGLE(CObjMgr)->Get_Player();
}

int CBall::Update()
{
    if (m_bDead)
        return OBJ_DEAD;

    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    Vector2D targetDir = m_pTarget->Get_Pos() - Get_Pos();
    m_vDir = (targetDir).Normalize();

    if (m_fCurHp <= 0.f)
    {
        m_eCurState = M_DEAD;
    }
    switch (m_eCurState)
    {
    case M_IDLE:
        m_eCurState_Anim = BA_IDLE;
        m_tFrame.Resume_Frame();
        m_fIdleTime += deltaTime;
        if (m_fIdleTime > 1.5f)
        {
            m_eCurState = M_ATTACK;
            m_fIdleTime = 0.f;
        }
        break;
    case M_ATTACK:
        m_eCurState_Anim = BA_SHOT;
        if (!m_bTargeting)
        {
            m_fAngle = m_vDir.ToAngle_Rad() * RAD2DEG;

            {
                float angle = 0.f;

                if (m_fAngle + 11.25 >= 360)
                    angle = m_fAngle + 11 - 360;
                else
                    angle = m_fAngle + 11;

                int num = angle / 22.5;

                switch (num)
                {
                case 0:
                    m_tFrame.iMotion = 1;
                    break;
                case 1:
                    m_tFrame.iMotion = 16;
                    break;
                case 2:
                    m_tFrame.iMotion = 15;
                    break;
                case 3:
                    m_tFrame.iMotion = 14;
                    break;
                case 4:
                    m_tFrame.iMotion = 13;
                    break;
                case 5:
                    m_tFrame.iMotion = 12;
                    break;
                case 6:
                    m_tFrame.iMotion = 11;
                    break;
                case 7:
                    m_tFrame.iMotion = 10;
                    break;
                case 8:
                    m_tFrame.iMotion = 9;
                    break;
                case 9:
                    m_tFrame.iMotion = 8;
                    break;
                case 10:
                    m_tFrame.iMotion = 7;
                    break;
                case 11:
                    m_tFrame.iMotion = 6;
                    break;
                case 12:
                    m_tFrame.iMotion = 5;
                    break;
                case 13:
                    m_tFrame.iMotion = 4;
                    break;
                case 14:
                    m_tFrame.iMotion = 3;
                    break;
                case 15:
                    m_tFrame.iMotion = 2;
                    break;
                }
            }

            m_fRotateTime += deltaTime;
            if (m_fRotateTime > 1.f)
            {
                m_bTargeting = true;
                m_fRotateTime = 0.f;
            }
        }
        else
        {
            m_fShotTime += deltaTime;
            if (m_fShotTime > 0.5f)
            {
                m_bTargeting = false;
                m_tFrame.Resume_Frame();
                m_eCurState = M_IDLE;
                m_fShotTime = 0.f;
            }

            m_tInfo.fX += m_fSpeed * deltaTime * cosf(m_fAngle * DEG2RAD);
            m_tInfo.fY -= m_fSpeed * deltaTime * sinf(m_fAngle * DEG2RAD);

            m_fTime2 += deltaTime;
            if (m_fTime2 > 0.1f)
            {
                float fx, fy;
                if (CCollsionMgr::Check_Rect(m_tInfo, m_pTarget->Get_ColInfo(), &fx, &fy))
                {
                    m_pTarget->Get_Damage(m_fAttack, this);
                }

                m_fTime2 = 0.f;
            }
        }
        break;
    case M_HIT:
        m_eCurState_Anim = BA_HIT;
        m_bTargeting = false;
        m_tFrame.Resume_Frame();

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
        m_eCurState_Anim = BA_DEAD;
        break;
    }

    __super::Update_Rect();
    __super::Update_TileColRect();
    return OBJ_NOEVENT;

}

void CBall::Late_Update()
{
    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    if (m_fCurHp <= 0.f) m_eCurState = M_DEAD;

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
        case BA_IDLE:
            break;
        case BA_SHOT:
            if (!m_bTargeting)
            {
                m_tFrame.Stop_Frame();
                m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
            }
            break;
        case BA_HIT:
            m_eCurState = M_IDLE;
            GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_MONSTER_HIT);
            break;
        case BA_DEAD:
            Set_Dead();
            break;
        }
    }
}

void CBall::Render(HDC hDC)
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

void CBall::Release()
{
}

void CBall::Debug_Render(HDC hDC)
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

void CBall::Anim_Update()
{

}

void CBall::Get_Damage(float _dmg, CObj* _pSrc)
{
    if (m_eCurState == M_ATTACK || m_eCurState == M_DEAD) return;

    Vector2D nuckBackDir = (Get_Pos() - _pSrc->Get_Pos()).Normalize();
    m_tInfo.fX -= nuckBackDir.x * 1.5f;
    m_tInfo.fY -= nuckBackDir.y * 1.5f;

    m_fCurHp -= _dmg;

    m_eCurState = M_HIT;
}

void CBall::SetFrameByState()
{
    // 상태 변화 없다면 나가기
    if (m_eCurState_Anim == m_ePrevState_Anim)
        return;

    // 이전 상태 갱신
    m_ePrevState_Anim = m_eCurState_Anim;

    switch (m_eCurState_Anim)
    {
    case BA_IDLE:
        m_tFrame.SetInfo(0, 0, 0, 200);
        break;
    case BA_SHOT:
        m_tFrame.dwSpeed = 200;
        m_tFrame.iFrameStart = 0;
        m_tFrame.iFrameEnd = 3;
        m_tFrame.dwTime = GetTickCount();
        break;
    case BA_HIT:
        m_tFrame.SetInfo(0, 2, 0, 200);
        break;
    case BA_DEAD:
        m_tFrame.SetInfo(0, 1, 0, 200);
        break;
    }
}
