#pragma once
#include "Obj.h"


class CPlayer :
    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    // CObj을(를) 통해 상속됨
    virtual void    Initialize()    override;
    virtual int     Update()        override;
    virtual void    Late_Update()   override;
    virtual void    Render(HDC hDC) override;
    virtual void    Release()       override;

public:
    virtual void    Debug_Render(HDC hDC)   override;

protected:
    virtual void    Anim_Update()           override;
    
public:
    void            Set_Inven(CUI* _inven)                      { m_pInven = _inven; }
    PlayerSkill     Get_SkillTypeInKey(PlayerSkillBtn _btnType) { return m_eSkill[_btnType]; }
    void            Change_SkillIdx(PlayerSkillBtn _Dst, PlayerSkillBtn _Src);
    void            Add_NewSkill(PlayerSkill _eAddSkillType);

protected:
    void    Update_LookDir();

    void    KeyInput();
    void    SetFrameByState();
    
    void    Play_Skill(PlayerSkillBtn _btn);
    void    Skill_OnMouse(POINT _mousePt, PlayerSkill _skill);
    void    Skill_NoMouse(PlayerSkill _skill);

    void    CheckFallTrap();
    // 입력받은 좌표에서 자신 주변의 타일을 순회하여 충돌시 true반환
    bool    CheckTile(float fX, float fY,TILE_ID& _returnID);

    // eSkill의 특정 인덱스에 원하는 스킬을 넣어주는 함수
    void    Set_Skill(int _slotIdx, PlayerSkill _skill);

    virtual void	Get_Damage(float _dmg, CObj* _pSrc);


protected:
    bool        m_bHitted;      // 맞았는지
    float       m_fHittedTime;  // 맞은 지 몇초가 지났는지

    int         m_iAttackStack; // 공격모션시 3번돌리기 위한 변수
    
    // 달리기 관련
    float       m_fDashSpeed;
    DWORD       m_dwRunTime;    // 이동시간을 체크하여 2초이상 갔다면 가속하도록;
    bool        m_bSpeedUp;     // 이동속도가 증가되었는지

    // 대쉬 관련
    Vector2D    m_vPrevPos;     // 대쉬 시작지점을 가지고 있을 변수
    bool        m_bOnTrap;      // 트랩에 빠져 DEAD상태가 되는것을 구분하려고

    // 애니매이션 상태
    ANIM_STATE m_eCurState;
    ANIM_STATE m_ePrevState;

    // 인벤 정보
    CUI*        m_pInven;

    PlayerSkill m_eSkill[Btn_End];

};

