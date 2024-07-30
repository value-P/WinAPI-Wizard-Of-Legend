#pragma once

#define		WINCX		1080//800
#define		WINCY		600

#define		MAPCX		8000
#define		MAPCY		6000

#define		TILECX		32
#define		TILECY		32

#define		TILEX		250
#define		TILEY		188

#define		PURE		= 0

#define		OBJ_NOEVENT	 0
#define		OBJ_DEAD	 1

#define		ANIM_END	 -1

#define		PATTERN_ING	 0
#define		PATTERN_END	 1

#define		PI			3.14f

#define		VK_MAX		0xff

#define		DEG2RAD		PI/180
#define		RAD2DEG		180/PI

#define		RUNSPEED	400.f
#define		WALK_TO_RUNTIME 3000 // 달리기 가속되는 시간

#define		GET_SINGLE(CLASSNAME) CLASSNAME::Get_Instance()
#define		AB_FACTORY(CLASSNAME) CAbstractFactory<CLASSNAME>

#pragma region 구조체
typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;

	tagInfo() : fX(0.f), fY(0.f), fCX(0.f), fCY(0.f) {}
	tagInfo(float _x, float _y, float _cx, float _cy) : fX(_x), fY(_y), fCX(_cx), fCY(_cy) 
	{}
}INFO;

typedef struct tagFrame
{
	int			iFrameStart;
	int			iFrameEnd;
	int			iMotion;
	DWORD		dwSpeed;
	DWORD		dwTime;

	bool		stopFrame = false;

	void SetInfo(int _frameStart, int _frameEnd, int _motion, DWORD _animSpeed)
	{
		iFrameStart = _frameStart;
		iFrameEnd = _frameEnd;
		iMotion = _motion;
		dwSpeed = _animSpeed;
	}

	void Stop_Frame() { stopFrame = true; }
	void Resume_Frame() { stopFrame = false; }
	int Get_CurFrame() { return iFrameStart; }

	tagFrame() :iFrameStart(0), iFrameEnd(0), iMotion(0), dwSpeed(0), dwTime(GetTickCount()) {}
	tagFrame(int frameStart, int frameEnd, int motion, DWORD animSpeed)
		: iFrameStart(frameStart), iFrameEnd(frameEnd), iMotion(motion), dwSpeed(animSpeed), dwTime(GetTickCount()) {}

}FRAME;

#pragma endregion


#pragma region 열거체
enum OBJ_ID { OBJ_STRUCT, OBJ_PLAYER, OBJ_EFFECT, OBJ_SKILL, OBJ_MONSTER, OBJ_END };
enum SCENEID { SC_LOGO, SC_EDIT, SC_STAGE_1, SC_STAGE_Boss, SC_END };
enum TILE_ID { TILE_NONE = -1, TILE_FLOOR, TILE_WALL, TILE_TRAP, TILE_DECO, TILE_SWORDMAN, TILE_WIZARD, TILE_ARCHOR, TILE_DETECTOR};
enum STRUCTURE_ID { ST_NONE = -1, ST_STATIC, ST_DECO};
enum RENDERID { BACKGROUND, GAMEOBJECT, EFFECT, RENDER_END };

enum SKILL_TYPE {ST_THROW,ST_RANGE,ST_WALL};

//보스 애니메이션 용
enum BOSS_ANIM_STATE { BOSS_NONE = -1, BOSS_IDLE, BOSS_SKILL1, BOSS_SKILL2, BOSS_JUMPATK, BOSS_HIT, BOSS_DEAD };
enum BOSS_PATTERN { B_P_NONE = -1, BOSS_1, BOSS_2, BOSS_3, BOSS_P_END };

// 사운드 용
enum CHANNELID { SOUND_EFFECT_1, SOUND_EFFECT_2, SOUND_EFFECT_3,SOUND_MONSTER_HIT, SOUND_UI_1,SOUND_UI_2, SOUND_BGM, MAXCHANNEL };

// Obj공용
enum LookDir { LEFT, RIGHT, BACK, FRONT};

// 플레이어 전용
enum ANIM_STATE { IDLE, RUN,  DASH, ATTACK_1, ATTACK_2, ONHIT, DEAD, PLAYER_ANIM_END };
enum PlayerSkill { SKILL_NONE = -1, SKILL_NORMAL, SKILL_DASH, SKILL_SHARK, SKILL_EARTH, SKILL_VOIDSTORM, SKILL_ICE_ORBIT, SKILL_END };
enum PlayerSkillBtn { Mouse_LB, Key_Space, Mouse_RB, Key_Q, Key_E, Key_R, Btn_End };

// 몬스터 상태
enum MONSTER_STATE {M_NONE = -1,M_IDLE,M_MOVE,M_ATTACK,M_SKILL,M_HIT,M_DEAD,M_STATE_END};

// 일반 몬스터 종류
enum MONSTER_TYPE {MONSTER_TYPE_NONE = -1,MONSTER_KNIGHT,MONSTER_ARCHOR,MONSTER_BALL,MONSTER_WIZARD};

// UI전용
enum UI_ID {UI_SLIDER_HP, UI_SLIDER_MANA,UI_MONEY, UI_NORMAL, UI_ICON, UI_INVEN, TEMP_UI ,UI_MOUSE, UI_END};

#pragma endregion


#pragma region 함수/함수객체
template<typename T>
void		Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

struct DeleteObj
{
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};

#pragma endregion


extern	HWND	g_hWnd;