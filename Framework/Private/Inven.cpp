#include "stdafx.h"
#include "Inven.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "CollsionMgr.h"
#include "UIMgr.h"
#include "Player.h"
#include "SoundMgr.h"
CInven::CInven()
    :m_iSelectedIdx(0), m_iPrecSelectedIdx(0), m_bStartChange(false)
{
    m_wsExFrameKey = L"";
}

CInven::~CInven()
{
    Release();
}

void CInven::Initialize()
{
    m_pFrameKey = L"INVENTORY";
    m_bToggle = false;

    // 스킬 아이콘들
    m_skillIconArr[0].Set_BtnType(Mouse_LB);
    m_skillIconArr[0].Set_Pos(m_tInfo.fX - 115.f, m_tInfo.fY - 195.f);
    m_skillIconArr[1].Set_BtnType(Key_Space);
    m_skillIconArr[1].Set_Pos(m_tInfo.fX - 70.f, m_tInfo.fY - 195.f);
    m_skillIconArr[2].Set_BtnType(Mouse_RB);
    m_skillIconArr[2].Set_Pos(m_tInfo.fX - 23.f, m_tInfo.fY - 195.f);
    m_skillIconArr[3].Set_BtnType(Key_Q);
    m_skillIconArr[3].Set_Pos(m_tInfo.fX + 25.f, m_tInfo.fY - 195.f);
    m_skillIconArr[4].Set_BtnType(Key_E);
    m_skillIconArr[4].Set_Pos(m_tInfo.fX + 70.f, m_tInfo.fY - 195.f);
    m_skillIconArr[5].Set_BtnType(Key_R);
    m_skillIconArr[5].Set_Pos(m_tInfo.fX + 115.f, m_tInfo.fY - 195.f);

    for (auto& iter : m_skillIconArr)
    {
        iter.Set_Owner(m_pOwner);
        iter.Initialize();
    }
}

int CInven::Update()
{
    if (!m_bToggle)
    {
        if (m_bStartChange) m_bStartChange = false;
        return OBJ_NOEVENT;
    }

    KeyInput();

    for (auto& iter : m_skillIconArr)
        iter.Update();

    __super::Update_Rect();

    return OBJ_NOEVENT;
}

void CInven::Late_Update()
{
    if (!m_bToggle) return;

    for (auto& iter : m_skillIconArr)
        iter.Late_Update();

    for (int i = 0; i < SKILL_END; ++i)
    {
        if (i == m_iSelectedIdx)
            m_skillIconArr[i].Set_Selected(true);
        else
            m_skillIconArr[i].Set_Selected(false);
    }
    
    ChangeExKey();
}

void CInven::Render(HDC hDC)
{
    if (!m_bToggle) return;

    // 인벤 메인
    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);
    BitBlt(hDC,
        m_tRect.left, m_tRect.top,
        m_tInfo.fCX, m_tInfo.fCY,
        hMemDC,
        0, 0, 
        SRCCOPY);

    // 스킬 아이콘 띄우기
    for (auto& iter : m_skillIconArr)
        iter.Render(hDC);

    // 선택 스킬 설명 띄우기
    if (m_skillIconArr[m_iSelectedIdx].Get_IconType() == SKILL_NONE)
        return;

    HDC hExDC = CBmpMgr::Get_Instance()->Find_Img(m_wsExFrameKey);

    BitBlt(hDC,
        m_tRect.left + 55, m_tRect.top + 350,
        238, 100,
        hExDC,
        0, 0,
        SRCCOPY);
}

void CInven::Release()
{
}

void CInven::Debug_Render(HDC hDC)
{
}

void CInven::Anim_Update()
{
}

void CInven::KeyInput()
{
    if(GET_SINGLE(CKeyMgr)->Key_Pressing(VK_LBUTTON))
    {
        INFO mouseInfo = GET_SINGLE(CUIMgr)->Get_Mouse()->Get_Info();
        float fx, fy;

        for (int i = 0; i < SKILL_END; ++i)
        {
            // 마우스 클릭 시 아이콘 눌렀으면
            if (CCollsionMgr::Check_Rect(m_skillIconArr[i].Get_Info(), mouseInfo, &fx, &fy))
            {
                m_iSelectedIdx = i;

                // 스페이스바를 누른 이후 클릭시 교환되도록
                if (m_bStartChange)
                {
                    GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_UI_2);
                    GET_SINGLE(CSoundMgr)->Play_Sound(L"CHANGE_SKILL.mp3", SOUND_UI_2, 1.f);
                    PlayerSkillBtn DstBtn = m_skillIconArr[m_iPrecSelectedIdx].Get_BtnType();
                    PlayerSkillBtn SrcBtn = m_skillIconArr[m_iSelectedIdx].Get_BtnType();

                    static_cast<CPlayer*>(m_pOwner)->Change_SkillIdx(DstBtn, SrcBtn);
 
                    m_bStartChange = false;
                }
            }
        }

    }

    if (GET_SINGLE(CKeyMgr)->Key_Pressing(VK_SPACE))
    {
        if (!m_bStartChange)
        {
            m_iPrecSelectedIdx = m_iSelectedIdx;
            m_bStartChange = true;
        }
    }
}

void CInven::ChangeExKey()
{
    PlayerSkill skillType = m_skillIconArr[m_iSelectedIdx].Get_IconType();

    switch (skillType)
    {
    case SKILL_NORMAL:
        m_wsExFrameKey = L"NORMAL_ATTACK_EX";
        break;
    case SKILL_DASH:
        m_wsExFrameKey = L"DASH_EX";
        break;
    case SKILL_SHARK:
        m_wsExFrameKey = L"FIRE_DRAGON_EX";
        break;
    case SKILL_EARTH:
        m_wsExFrameKey = L"GAIA_ARMOR_EX";
        break;
    case SKILL_VOIDSTORM:
        m_wsExFrameKey = L"ICE_BLAST_EX";
        break;
    case SKILL_ICE_ORBIT:
        m_wsExFrameKey = L"ICE_KRYSTAL_EX";
        break;
    }
}
