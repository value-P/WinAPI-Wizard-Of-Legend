#include "stdafx.h"
#include "SoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;

CSoundMgr::CSoundMgr()
{
	m_pSystem = nullptr;
}

CSoundMgr::~CSoundMgr()
{
	Release();
}

void CSoundMgr::Initialize()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}

void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

void CSoundMgr::Play_Sound(wstring wsSoundKey, CHANNELID eID, float fVolume)
{
	auto iter = m_mapSound.find(wsSoundKey);

	if (iter == m_mapSound.end()) return;

	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::Play_Sound_NoInterval(wstring wsSoundKey, CHANNELID eID, float fVolume)
{
	auto iter = m_mapSound.find(wsSoundKey);

	if (iter == m_mapSound.end()) return;

	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::Play_BGM(wstring wsSoundKey, float fVolume)
{
	auto iter = m_mapSound.find(wsSoundKey);

	if (iter == m_mapSound.end()) return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::Stop_Sound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::Stop_All()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::Set_ChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	long handle = _findfirst("../Sound/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);
		
		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);
			wstring keyStr(pSoundKey);
			delete[] pSoundKey;
			m_mapSound.emplace(keyStr, pSound);
		}

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);

}
