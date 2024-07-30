#pragma once
class CTimeMgr final
{
public:
	CTimeMgr();
	~CTimeMgr();

public:
	static CTimeMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTimeMgr;
		}

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
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

public:
	double Get_DT() { return m_dDT; }
	float Get_fDT() { return static_cast<float>(m_dDT); }

private:
	LARGE_INTEGER	m_IICurCount;
	LARGE_INTEGER	m_IIPrevCount;
	LARGE_INTEGER	m_IIFrequency;

	double	m_dDT;
	double	m_FPS;
	double	m_dAcc;
	UINT	m_iCallCount;
	UINT	m_iFPS;

	static CTimeMgr* m_pInstance;
};

