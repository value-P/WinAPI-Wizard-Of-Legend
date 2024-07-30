#pragma once
#include "Obj.h"

class CFloatRock;

class CBoss :
    public CObj
{
public:
    CBoss();
    virtual ~CBoss();

public: 
    // CObj을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

public:
    virtual void Get_Damage(float _dmg, CObj* _pSrc) override;

    // 입력받은 좌표에서 자신 주변의 타일을 순회하여 충돌시 true반환
    bool    CheckTile(float fX, float fY);

private:
    void    Pattern();
    int     Pattern_first();
    int     Pattern_second();
    int     Pattern_third();

private:
    void    SetFrameByState();
    void    Detect_Target();

    float   Get_TargetAngle();

private:
    // 보스 스탯
    float   m_fJumpSpeed;
    float   m_fDetectRangle; // 플레이어를 감지하는 범위
    
    // 보스 애니메이션 관련 
    //1.기본 2.스킬1 3.스킬2(패턴1,2공용) 4.점프공격 5.피격 6.사망
    BOSS_ANIM_STATE  m_eCurState_Anim;
    BOSS_ANIM_STATE  m_ePrevState_Anim;
    MONSTER_STATE    m_eCurState;
    BOSS_PATTERN     m_eCurPattern;

    // 보스 점프공격 타겟위치
    Vector2D    m_vJumpStartPos;

    // 타격이 지속되면 강제로 패턴을 사용하도록 히트스택 적용
    int         m_iHitStack;

    // 돌기둥 웨이브 몇번했는가 확인용
    int         m_iPillarWaveStack;

    float       m_fTime;
    float       m_fPatternTime;

    int         m_iWave;
    bool        m_bOnWave;

    list<CFloatRock*> m_floatRockList;

    float       m_fDeadTime;
};

