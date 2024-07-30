// DefaultWindow.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DefaultWindow.h"
#include "MainGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HWND	g_hWnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);		// 현재 창을 만들기 위해 옵션을 설정하고 창 스타일을 제어하는 함수
BOOL                InitInstance(HINSTANCE, int);				// 창을 생성하고 초기화 작업을 수행하는 함수
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,			// 메모리에 할당되는 실체, 즉 객체 정보
                     _In_opt_ HINSTANCE hPrevInstance,	// 전에 실행되었던 인스턴스 핸들 값, 없을 경우 NULL
                     _In_ LPWSTR    lpCmdLine,			// 커맨드 명령어
                     _In_ int       nCmdShow)			// 실행한 창의 스타일 옵션, 설정 (모양 / 최소화 모양 / 최대화 모양 등)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

    MSG msg;
	msg.message = WM_NULL;

	CMainGame		MainGame;
	MainGame.Initialize();

	// 운영체제가 시작된 후로 흐른 시간을 정수 형태로 반환하는 함수
	DWORD	dwTime = GetTickCount();	// 1 / 1000초의 정수 값을 반환
	// 200

	while (true)
	{
		// PM_REMOVE   : 메세지 큐에서 읽어옴과 동시에 메세지 큐에서 메세지를 제거
		// PM_NOREMOVE : 메세지 큐에 메세지가 있는지에 대한 여부 판단만 하고 메세지 큐에서 메세지를 제거하지는 않음

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			// 메뉴 기능의 단축키가 제대로 작동하도록 검사하는 함수
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				// 키보드 메세지를 가공하여 프로그램에서 쉽게 사용 할 수 있도록 해주는 함수
				// Getmessage가 읽어온 키보드 메세지를 가공하는 역할
				TranslateMessage(&msg);

				// 시스템 메세지 큐에서 꺼낸 메세지를 프로그렘 메세지 처리 함수(WndProc)로 전달하는 함수
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (dwTime + 10 < GetTickCount())
			{
				MainGame.Update();
				MainGame.Late_Update();
				MainGame.Render();

				dwTime = GetTickCount();
			}
            //MainGame.Update();
            //MainGame.Late_Update();
            //MainGame.Render();


		}		
	}

    return (int) msg.wParam;
}	 



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;	// 윈도우 창 생성 시 필요한 정보를 보관하는 구조체

    wcex.cbSize = sizeof(WNDCLASSEX);	// 자기 자신의 사이즈를 저장해놓음

    wcex.style          = CS_HREDRAW | CS_VREDRAW;	
    wcex.lpfnWndProc    = WndProc;		// 메세지 처리 함수, 저장해둔 함수가 호출되어 메세지가 처리되는 방식
    wcex.cbClsExtra     = 0;			// 윈도우가 특수한 목적으로 사용하는 여분의 공간
    wcex.cbWndExtra     = 0;			// 일종의 예약 영역이며 사용하지 않는 경우 0으로 지정
    wcex.hInstance      = hInstance;	// 이 윈도우 클래스를 사용하는 프로그램의 번호 설정(메인 함수의 전달된 hInstance값을 그대로 대입)		
    
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);
    
	wcex.lpszClassName  = szWindowClass;	// 실행 파일의 이름을 저장(가급적이면 프로젝트 이름과 일치시킴)
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	// 윈도우 창 상단에 있는 작은 아이콘 모양을 지정

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT rc{ 0, 0, WINCX, WINCY };

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // rc = rc + 기본 창 스타일 + 메뉴 바의 크기 고려
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   // 실제 창을 생성하는 함수
   HWND hWnd = CreateWindowW(szWindowClass,			// 앞서 정의한 윈도우 클래스의 전달한 클래스의 이름을 그대로 넣어줌
						   szTitle,					// 윈도우 창 상단의 출력할 문자열
						   WS_OVERLAPPEDWINDOW,		// 만들고자 하는 윈도우 창 스타일 지정(기본 값 지정하면 흔한 창 스타일 모양 생성)
						   CW_USEDEFAULT, 0,		// 생성할 창의 X,Y 좌표
						   rc.right - rc.left, 
						   rc.bottom - rc.top,	// 생성할 창의 가로, 세로 사이즈
						   nullptr, // 부모 윈도우가 있다면 부모 윈도우의 핸들을 지정
						   nullptr, // 윈도우에서 사용할 메뉴의 핸들을 지정
						   hInstance, // 윈도우를 만드는 주체
						   nullptr); // 운영체제가 특수한 목적으로 사용하므로 건드릴 일 없음

   
   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK RangeTileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK TileTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MakeObjectProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT	rcWnd{ 100, 100, 200, 200 };
	static list<RECT>	BulletList;


    switch (message)
    {
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_MENU_TILE:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_TILECOUNT), hWnd, TileCountProc);
                if (iRet == IDOK)
                {

                }
            }
                break;
            case ID_MENU_TILES:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RANGETILE), hWnd, RangeTileCountProc);
                if (iRet == IDOK)
                {

                }

            }
                break;
            case ID_TILE_OPTION:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_TILEOPTION), hWnd, TileTypeProc);
                if (iRet == IDOK)
                {

                }

            }
            break;
            case ID_STRUCT:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_OBJECT), hWnd, MakeObjectProc);
                if (iRet == IDOK)
                {

                }

            }
            break;

            case IDM_EXIT:
                DestroyWindow(g_hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			
            EndPaint(hWnd, &ps);
        }
        break;



    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;			
}

// 함수 호출 규약 : 함수의 실행이 종료되면 할당했더 스택 메모리를 해제해야 하는데 함수를 호출한 호출원(caller)에서 정리할 것인가
// 호출 받은 피호출자(callee)에서 할 것인가에 따른 약속 또는 규약

// __cdecl : c / c++ 언어의 기본 호출 규약, 호출원가 스택을 정리, 대표적인 함수로 printf를 떠올리면 쉽다.
// 정작 자기 자신에게 어떤 매개변수가 얼마나 들어올지 모른다는 점(가변인자 함수)때문에 본인이 스택을 정리할 수 없다.

// __stdcall : WIN32 api의 기본 호출 규약, 피호출자가 스택을 정리, api에서 제공하는 대부분의 함수들은 '고정인자 함수'이다. 
// 매개 변수가 고정되어 있어 피호출자가 어떤 것이 들어오고 크기가 얼마인지 예측 가능하기 때문에 스택을 스스로 정리 할 수 있다.

// __fastcall : 함수 호출을 빠르게 처리하기 위한 규약, 함수 코드를 레지스터를 통해 저장
// __thiscall : this 포인터를 매개 변수로 넘겨 받았을 때 사용하는 호출 규약

// ※ 네 가지 호출 규약 중 __cdecl만 호출원이 스택을 정리

// 1. 그리기 숙제(그림을 그려와라)
// 2. 스페이스 바를 눌렀을 때 위로 나가는 총알(ellipse)을 만들어와라