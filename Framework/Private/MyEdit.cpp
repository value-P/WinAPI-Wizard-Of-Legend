#include "stdafx.h"
#include "MyEdit.h"
#include "resource.h"
#include "SceneMgr.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"

CMyEdit::CMyEdit()
    :m_fCameraSpeed(0.f), m_EditMode(0), m_bClicked(false), m_RangeTileMode(false)
{
    ZeroMemory(&m_tStartPt, sizeof(POINT));
    ZeroMemory(&m_tEndPt, sizeof(POINT));
}

CMyEdit::~CMyEdit()
{
    Release();
}

void CMyEdit::Initialize()
{
    CScrollMgr::Get_Instance()->Set_ScrollPos((float)(MAPCX >> 1), (float)(MAPCY - WINCY * 0.5f));
    //CScrollMgr::Get_Instance()->Set_ScrollPos((float)(0), (float)(0));

    ShowCursor(true);

    m_fCameraSpeed = 5.f;
}

int CMyEdit::Update()
{
    Key_Input();

    CTileMgr::Get_Instance()->Update();
    CObjMgr::Get_Instance()->Update();

    return 0;
}

void CMyEdit::Late_Update()
{
    CTileMgr::Get_Instance()->Late_Update();
    CObjMgr::Get_Instance()->Late_Update();
}

void CMyEdit::Render(HDC hDC)
{
    float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"TutorialCourse");
    //HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(L"BossMap");

    BitBlt(hDC, 0 + fScrollX, 0 + fScrollY, MAPCX, MAPCY,
        hMemDC, 0, 0, SRCCOPY);

    //CTileMgr::Get_Instance()->Render(hDC);
    CObjMgr::Get_Instance()->Render(hDC);

    switch (m_EditMode)
    {
        // 타일 모드
    case 0:
        TextOut(hDC, 0, 0, TEXT("TILE_MODE"), 9);
            break;
        // 오브젝트 모드
    case 1:
        TextOut(hDC, 0, 0, TEXT("OBJECT_MODE"), 11);
        break;
        // 타일 타입 모드
    case 2:
        TextOut(hDC, 0, 0, TEXT("TILE_TYPE_MODE"), 14);
        CTileMgr::Get_Instance()->Debug_Render(hDC);
        CObjMgr::Get_Instance()->Debug_Render(hDC);
        break;
    }

    if(m_RangeTileMode)
        TextOut(hDC, 0, 20, TEXT("RANGE_TILE"), 11);
    else
        TextOut(hDC, 0, 20, TEXT("SINGLE_TILE"), 11);

}

void CMyEdit::Release()
{
}

void CMyEdit::Key_Input()
{
    // 모드변경
    if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
        m_EditMode = (m_EditMode + 1) % 3;
    // 타일모드 변경
    if (CKeyMgr::Get_Instance()->Key_Down('T') ||
        CKeyMgr::Get_Instance()->Key_Down('t'))
        m_RangeTileMode = !m_RangeTileMode;

    if (CKeyMgr::Get_Instance()->Key_Down('S'))
    {
        CTileMgr::Get_Instance()->Save_Tile(L"../Data/Normal_Tile.dat");
        CObjMgr::Get_Instance()->Save_Structure(L"../Data/Normal_Structure.dat");
        CTileMgr::Get_Instance()->Save_TotalMapImg();
    }
    if (CKeyMgr::Get_Instance()->Key_Down('L'))
    {
        CTileMgr::Get_Instance()->Load_Tile(L"../Data/Normal_Tile.dat");
        CObjMgr::Get_Instance()->Load_Structure(L"../Data/Normal_Structure.dat");
    }

    switch (m_EditMode)
    {
        // 타일 모드
    case 0:
    {
        if (m_RangeTileMode)
        {
            if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
            {

                float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
                float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

                POINT pt{};
                GetCursorPos(&pt);
                ScreenToClient(g_hWnd, &pt);
                pt.x -= (int)scrollX;
                pt.y -= (int)scrollY;


                CTileMgr::Get_Instance()->Ranged_Tile_Picking(pt);
            }
        }
        else
        {
            if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
            {
                float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
                float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

                if (m_bClicked == false)
                {
                    m_bClicked = true;

                    GetCursorPos(&m_tStartPt);
                    ScreenToClient(g_hWnd, &m_tStartPt);


                    m_tStartPt.x -= (int)scrollX;
                    m_tStartPt.y -= (int)scrollY;
                }
                else if (m_bClicked)
                {
                    m_bClicked = false;

                    GetCursorPos(&m_tEndPt);
                    ScreenToClient(g_hWnd, &m_tEndPt);

                    m_tEndPt.x -= (int)scrollX;
                    m_tEndPt.y -= (int)scrollY;

                    CTileMgr::Get_Instance()->Range_Picking(m_tStartPt, m_tEndPt);
                }
            }
        }
    }
        break;
        // 오브젝트 모드
    case 1:
    {
        if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
        {
            float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
            float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

            POINT pt{};
            GetCursorPos(&pt);
            ScreenToClient(g_hWnd, &pt);
            pt.x -= (int)scrollX;
            pt.y -= (int)scrollY;

            CObjMgr::Get_Instance()->PickingStructure(pt);
        }
    }
        break;
        // 타일 타입 모드
    case 2:
    {
        if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
        {
            float scrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
            float scrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

            POINT pt{};
            GetCursorPos(&pt);
            ScreenToClient(g_hWnd, &pt);
            pt.x -= (int)scrollX;
            pt.y -= (int)scrollY;

            CTileMgr::Get_Instance()->TileType_Picking(pt);
        }
    }
        break;

    }

    // 스크롤 입력
    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(m_fCameraSpeed);
    }
    else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
    {
        CScrollMgr::Get_Instance()->Set_ScrollX(-1 * m_fCameraSpeed);
    }

    if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(m_fCameraSpeed);
    }
    else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
    {
        CScrollMgr::Get_Instance()->Set_ScrollY(-1 * m_fCameraSpeed);
    }

    if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) || CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT))
        CScrollMgr::Get_Instance()->Set_ScrollX(0.f);
    if (CKeyMgr::Get_Instance()->Key_Up(VK_UP) || CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
        CScrollMgr::Get_Instance()->Set_ScrollY(0.f);


}


//=====================================================================================
// Tile Count Window Proc
//=====================================================================================


INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            UINT iXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
            UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);
            CTileMgr::Get_Instance()->Set_TileIdx((int)iXCount, (int)iYCount);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RangeTileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            UINT iStrtXCount = GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);
            UINT iStrtYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);
            UINT iEndXCount = GetDlgItemInt(hDlg, IDC_EDIT4, nullptr, false);
            UINT iEndYCount = GetDlgItemInt(hDlg, IDC_EDIT5, nullptr, false);

            Vector2D startIdx((float)iStrtXCount, (float)iStrtYCount);
            Vector2D endIdx((float)iEndXCount, (float)iEndYCount);

            CTileMgr::Get_Instance()->Set_RangeTileIdx(startIdx, endIdx);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK TileTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TILE_ID tileType = (TILE_ID)GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, false);

            CTileMgr::Get_Instance()->Set_TileType(tileType);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MakeObjectProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            TCHAR lpStr[50] = {};
            GetDlgItemText(hDlg, IDC_EDIT1, lpStr, 50);

            STRUCTURE_ID type = (STRUCTURE_ID)(GetDlgItemInt(hDlg, IDC_EDIT4, nullptr, false));

            wstring tempStr(lpStr);

            CObjMgr::Get_Instance()->SetPickData(lpStr);

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}
