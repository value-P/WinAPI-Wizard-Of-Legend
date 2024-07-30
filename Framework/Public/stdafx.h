// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++라이브러리
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <io.h>
#include <iostream>

// 외부 라이브러리
#include <ole2.h>
#include <gdiplus.h>
#include <vfw.h>
#include "fmod.h"
#include "fmod.hpp"
// 링커 추가
#pragma comment(lib,"Gdiplus.lib")
#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "fmodex_vc.lib")

// 사용자 정의 라이브러리
#include "Define.h"
#include "Vector2D.h"
#include "AbstractFactory.h"

// 매크로
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Gdiplus;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
