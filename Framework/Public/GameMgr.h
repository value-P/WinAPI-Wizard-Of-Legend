#pragma once
class CGameMgr
{
private:
	CGameMgr();
	~CGameMgr();

public:
	static CGameMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CGameMgr;

		return m_pInstance;
	}
	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:

	//void	Update();
	//void	Late_Update();
	//void	Render(HDC hDC);
	//void	Release();

	//CObj*	Get_Player() {return m_pPlayer;}
	//void	Set_player(CObj* _player) { m_pPlayer = _player; }

private:
	static CGameMgr* m_pInstance;
	CObj*	m_pPlayer;

};

