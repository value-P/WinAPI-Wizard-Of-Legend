#pragma once

class CMainGame
{

public:
	CMainGame();
	~CMainGame();
public:
	void		Initialize();
	void		Update();
	void		Late_Update();
	void		Render();
	void		Release();

private:
	void		Initialize_Bmp();
	void		Initialize_UI();

private:

	HDC			m_DC;

	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[16];

};

