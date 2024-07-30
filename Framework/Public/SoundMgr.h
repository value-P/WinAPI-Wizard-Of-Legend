#pragma once
class CSoundMgr
{
private:
	CSoundMgr();
	~CSoundMgr();

public:
	static CSoundMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSoundMgr;
		}

		return m_pInstance;
	}
	static void		  Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void	Initialize();
	void	Release();

public:
	void	Play_Sound(wstring wsSoundKey, CHANNELID eID, float fVolume);
	void	Play_Sound_NoInterval(wstring wsSoundKey, CHANNELID eID, float fVolume);
	void	Play_BGM(wstring wsSoundKey, float fVolume);
	void	Stop_Sound(CHANNELID eID);
	void	Stop_All();
	void	Set_ChannelVolume(CHANNELID eID, float fVolume);

private:
	void	LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;

	map<wstring, FMOD_SOUND*> m_mapSound;
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	FMOD_SYSTEM* m_pSystem;

};

