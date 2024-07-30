#pragma once
class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();

public:
	static	CScrollMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CScrollMgr;

		return m_pInstance;
	}
	static	void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	float	Get_ScrollX()				{ return m_fScrollX; }
	float	Get_ScrollY()				{ return m_fScrollY; }

	void	Set_ScrollX(float scrollX)	{ m_fScrollX += scrollX;}
	void	Set_ScrollY(float scrollY)	{ m_fScrollY += scrollY;}

	void	Set_ScrollPos(float _fX, float _fY);

	void	Scroll_Lock();

private:
	static	CScrollMgr* m_pInstance;
	float				m_fScrollX;
	float				m_fScrollY;

};

