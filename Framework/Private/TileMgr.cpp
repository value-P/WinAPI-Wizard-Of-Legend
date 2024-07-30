#include "stdafx.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "CollsionMgr.h"
CTileMgr* CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
	:m_iTileXIdx(0),m_iTileYIdx(0),m_eTileType(TILE_FLOOR)
{
	m_vecTile.reserve(TILEX * TILEY);
	ZeroMemory(&m_vStartTileIdx, sizeof(Vector2D));
	ZeroMemory(&m_vEndTileIdx, sizeof(Vector2D));
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float fy = (float)(TILECY >> 1) + (i * TILECY);
			float fx = (float)(TILECX >> 1) + (j * TILECX);

			CObj* pObj = CAbstractFactory<CTile>::Create_Obj(fx, fy);
			m_vecTile.push_back(pObj);
		}
	}
}

void CTileMgr::Update()
{
	int	iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCullX + WINCX / TILECX + 2;
	int iMaxY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			// 인덱스 치환
			int iIndex = i * TILEX + j;

			if (iIndex < 0 || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Update();
		}
	}

	//for (auto& iter : m_vecTile)
	//	iter->Update();
}

void CTileMgr::Late_Update()
{
	int	iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCullX + WINCX / TILECX + 2;
	int iMaxY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			// 인덱스 치환
			int iIndex = i * TILEX + j;

			if (iIndex < 0 || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Late_Update();
		}
	}

	//for (auto& iter : m_vecTile)
	//	iter->Late_Update();

}

void CTileMgr::Render(HDC hDC)
{
	int	iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCullX + WINCX/TILECX + 2;
	int iMaxY = iCullY + WINCY/TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			// 인덱스 치환
			int iIndex = i * TILEX + j;

			if (iIndex < 0 || m_vecTile.size() <= (size_t)iIndex)
				continue;

			// 데코타입이면 데코렌더에서 돌리도록 따로빼줌
			bool isDeco = static_cast<CTile*>(m_vecTile[iIndex])->Get_TileType() == TILE_DECO;
			if (isDeco) continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}


}

void CTileMgr::DecoTile_Render(HDC hDC)
{
	int	iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCullX + WINCX / TILECX + 2;
	int iMaxY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			// 인덱스 치환
			int iIndex = i * TILEX + j;

			if (iIndex < 0 || m_vecTile.size() <= (size_t)iIndex)
				continue;

			// 데코타입이면 데코렌더에서 돌리도록 따로빼줌
			bool isDeco = static_cast<CTile*>(m_vecTile[iIndex])->Get_TileType() == TILE_DECO;
			if (!isDeco) continue;

			m_vecTile[iIndex]->Render(hDC);
		}
	}
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTileMgr::Debug_Render(HDC hDC)
{
	int	iCullX = abs((int)CScrollMgr::Get_Instance()->Get_ScrollX() / TILECX);
	int	iCullY = abs((int)CScrollMgr::Get_Instance()->Get_ScrollY() / TILECY);

	int iMaxX = iCullX + WINCX / TILECX + 2;
	int iMaxY = iCullY + WINCY / TILECY + 2;

	for (int i = iCullY; i < iMaxY; ++i)
	{
		for (int j = iCullX; j < iMaxX; ++j)
		{
			// 인덱스 치환
			int iIndex = i * TILEX + j;

			if (iIndex < 0 || m_vecTile.size() <= (size_t)iIndex)
				continue;

			m_vecTile[iIndex]->Debug_Render(hDC);
		}
	}

}

void CTileMgr::Picking(POINT pt)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;

	int iIndex = y * TILEX + x;

	// 예외처리
	if (0 > iIndex || m_vecTile.size() <= size_t(iIndex))
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawTileX(m_iTileXIdx);
	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawTileY(m_iTileYIdx);
}

void CTileMgr::Range_Picking(POINT startPT, POINT endPT)
{
	int startX = startPT.x / TILECX;
	int startY = startPT.y / TILECY;
	int endX = endPT.x / TILECX;
	int endY = endPT.y / TILECY;

	for (int i = startY; i <= endY; ++i)
	{
		for (int j = startX; j <= endX; ++j)
		{
			int iIndex = i * TILEX + j;

			// 예외처리
			if (0 > iIndex || m_vecTile.size() <= size_t(iIndex))
				return;

			dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawTileX(m_iTileXIdx);
			dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawTileY(m_iTileYIdx);

		}
	}
}

void CTileMgr::TileType_Picking(POINT pt)
{
	int x = pt.x / TILECX;
	int y = pt.y / TILECY;

	int iIndex = y * TILEX + x;

	// 예외처리
	if (0 > iIndex || m_vecTile.size() <= size_t(iIndex))
		return;

	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_TileType(m_eTileType);
}

void CTileMgr::Ranged_Tile_Picking(POINT pt)
{
	// 내타일이 시작할 위치인덱스와 얼마의 범위만큼 적용할지에 대한 수
	int startX = pt.x / TILECX;
	int startY = pt.y / TILECY;
	int iLengX = (int)m_vEndTileIdx.x - (int)m_vStartTileIdx.x + 1;
	int iLengY = (int)m_vEndTileIdx.y - (int)m_vStartTileIdx.y + 1;
	//  스프라이트의 좌표
	int iTileXIdx = (int)m_vStartTileIdx.x;
	int iTileYIdx = (int)m_vStartTileIdx.y;

	for (int y= startY; y < startY + iLengY; ++y)
	{
		for (int x = startX; x < startX + iLengX; ++x)
		{
			int iIndex = y * TILEX + x;
			++iTileXIdx;
			// 예외처리
			if (0 > iIndex || m_vecTile.size() <= size_t(iIndex))
				continue;
			
			dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawTileX(iTileXIdx-1);
			dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawTileY(iTileYIdx);

		}
		++iTileYIdx;
		iTileXIdx = (int)m_vStartTileIdx.x;
	}

}



void CTileMgr::Set_TileIdx(int _tileXIdx, int _tileYIdx)
{
	m_iTileXIdx = _tileXIdx; 
	m_iTileYIdx = _tileYIdx; 
}

void CTileMgr::Set_TileType(TILE_ID _tileType)
{
	m_eTileType = _tileType;
}

void CTileMgr::Set_RangeTileIdx(Vector2D _startTileIdx, Vector2D _endTileIdx)
{
	m_vStartTileIdx = _startTileIdx;
	m_vEndTileIdx = _endTileIdx;
}

INFO CTileMgr::Get_TileInfo(int _indexX, int _indexY)
{
	int iIndex = _indexY * TILEX + _indexX;

	return m_vecTile[iIndex]->Get_Info();
}



void CTileMgr::Save_Tile()
{
	HANDLE	hFile = CreateFile(L"../Data/Tile.dat",
		GENERIC_WRITE, NULL, NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	int iTileX = 0;
	int iTileY = 0;
	int eType = 0;
	DWORD dwByte = 0;

	for (auto& iter : m_vecTile)
	{
		iTileX = dynamic_cast<CTile*>(iter)->Get_DrawTileX();
		iTileY = dynamic_cast<CTile*>(iter)->Get_DrawTileY();
		eType = (int)(dynamic_cast<CTile*>(iter)->Get_TileType());
		
		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &iTileX, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iTileY, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &eType, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"TileSave", L"성공", MB_OK);
}

void CTileMgr::Save_Tile(wstring _frameKey)
{
	HANDLE	hFile = CreateFile(_frameKey.c_str(),
		GENERIC_WRITE, NULL, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	int iTileX = 0;
	int iTileY = 0;
	int eType = 0;
	DWORD dwByte = 0;

	for (auto& iter : m_vecTile)
	{
		iTileX = dynamic_cast<CTile*>(iter)->Get_DrawTileX();
		iTileY = dynamic_cast<CTile*>(iter)->Get_DrawTileY();
		eType = (int)(dynamic_cast<CTile*>(iter)->Get_TileType());

		WriteFile(hFile, &(iter->Get_Info()), sizeof(INFO), &dwByte, nullptr);
		WriteFile(hFile, &iTileX, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &iTileY, sizeof(int), &dwByte, nullptr);
		WriteFile(hFile, &eType, sizeof(int), &dwByte, nullptr);
	}

	CloseHandle(hFile);

}

void CTileMgr::Load_Tile()
{
	HANDLE hFile = CreateFile(L"../Data/Tile.dat",
		GENERIC_READ, NULL, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	INFO tInfo{};
	int iTileX = 0;
	int iTileY = 0;
	int eType = 0;
	DWORD dwByte = 0;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iTileX, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iTileY, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &eType, sizeof(int), &dwByte, nullptr);
		
		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create_Obj(tInfo.fX, tInfo.fY);
		dynamic_cast<CTile*>(pObj)->Set_DrawTileX(iTileX);
		dynamic_cast<CTile*>(pObj)->Set_DrawTileY(iTileY);
		dynamic_cast<CTile*>(pObj)->Set_TileType((TILE_ID)eType);

		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);

	MessageBox(g_hWnd, L"Tile Load", L"성공", MB_OK);
}

void CTileMgr::Load_Tile(wstring _frameKey)
{
	HANDLE hFile = CreateFile(_frameKey.c_str(),
		GENERIC_READ, NULL, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	INFO tInfo{};
	int iTileX = 0;
	int iTileY = 0;
	int eType = 0;
	DWORD dwByte = 0;

	Release();

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iTileX, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iTileY, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &eType, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pObj = CAbstractFactory<CTile>::Create_Obj(tInfo.fX, tInfo.fY);
		dynamic_cast<CTile*>(pObj)->Set_DrawTileX(iTileX);
		dynamic_cast<CTile*>(pObj)->Set_DrawTileY(iTileY);
		dynamic_cast<CTile*>(pObj)->Set_TileType((TILE_ID)eType);

		m_vecTile.push_back(pObj);
	}

	CloseHandle(hFile);
}

void CTileMgr::Get_Range_TileList(int startX, int startY, int endX, int endY, list<CObj*>* _list)
{
	for (int i = startY; i < endY + 1; ++i)
	{
		for (int j = startX; j < endX + 1; ++j)
		{
			int iIndex = i * TILEX + j;

			// 예외처리
			if (0 > iIndex || m_vecTile.size() <= size_t(iIndex))
				return;

			_list->push_back(m_vecTile[iIndex]);
		}
	}
}

CObj* CTileMgr::Get_Tile(int idxX, int idxY)
{
	int iIndex = idxY * TILEX + idxX;

	// 예외처리
	if (0 > iIndex || m_vecTile.size() <= size_t(iIndex))
		return nullptr;

	return m_vecTile[iIndex];
}

void CTileMgr::Save_TotalMapImg()
{
	HDC m_mainDc = GetDC(g_hWnd);
	// (사이즈 커스텀)
	HBITMAP m_hBitmap = CreateCompatibleBitmap(m_mainDc, MAPCX, MAPCY);
	HDC subDC = CreateCompatibleDC(m_mainDc);
	HBITMAP tempBitmap = (HBITMAP)SelectObject(subDC, m_hBitmap);

	for (auto iter : m_vecTile)
	{
		static_cast<CTile*>(iter)->Just_Draw(subDC);
	}

	SelectObject(subDC, tempBitmap);

	BITMAP bit;
	GetObject(m_hBitmap, sizeof(BITMAP), &bit);

	DWORD dwByte = 0;

	// 파일 경로
	HANDLE	hFile = CreateFile(L"../Data/MapImg.bmp",
		GENERIC_WRITE, NULL, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	BITMAPINFOHEADER bmih;

	ZeroMemory(&bmih, sizeof(BITMAPINFOHEADER));
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = bit.bmWidth;
	bmih.biHeight = bit.bmHeight;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;

	GetDIBits(subDC, m_hBitmap, 0, bit.bmHeight, NULL, (LPBITMAPINFO)&bmih,
		DIB_RGB_COLORS);

	LPBYTE lpBits = new BYTE[bmih.biSizeImage];

	GetDIBits(subDC, m_hBitmap, 0, bit.bmHeight, lpBits, (LPBITMAPINFO)&bmih,
		DIB_RGB_COLORS);

	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 'MB';
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfSize = bmfh.bfOffBits + bmih.biSizeImage;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;


	WriteFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	WriteFile(hFile, &bmih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	WriteFile(hFile, lpBits, bmih.biSizeImage, &dwByte, nullptr);

	CloseHandle(hFile);
	delete[] lpBits;
}
