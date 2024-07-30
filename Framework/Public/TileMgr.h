#pragma once
#include "Tile.h"

class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();

public:
	static CTileMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
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
	void	Initialize();
	void	Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	DecoTile_Render(HDC hDC);
	void	Release();

public:
	void	Debug_Render(HDC hDC);

public:
	void	Picking(POINT pt);
	void	Range_Picking(POINT startPT, POINT endPT);
	void	TileType_Picking(POINT pt);
	void	Ranged_Tile_Picking(POINT pt);

	void	Set_TileIdx(int _tileXIdx, int _tileYIdx);
	void	Set_TileType(TILE_ID _tileType);
	void	Set_RangeTileIdx(Vector2D _startTileIdx, Vector2D _endTileIdx);

	INFO	Get_TileInfo(int _indexX, int _indexY);

	int		Get_VecTileSize() { return (int)m_vecTile.size(); }

	void	Save_Tile();
	void	Save_Tile(wstring _frameKey);
	void	Load_Tile();
	void	Load_Tile(wstring _frameKey);

	// 지정한 범위내의타일을 리스트에 넣어주는 함수
	void	Get_Range_TileList(int startX, int startY, int endX, int endY, list<CObj*>* _list);
	// 지정한 좌표타일을 반환하는 함수
	CObj*	Get_Tile(int idxX, int idxY);


	void	Save_TotalMapImg();

private:
	static CTileMgr*	m_pInstance;
	vector<CObj*>		m_vecTile;
	int					m_iTileXIdx;
	int					m_iTileYIdx;
	TILE_ID				m_eTileType;
	Vector2D			m_vStartTileIdx;
	Vector2D			m_vEndTileIdx;

};

