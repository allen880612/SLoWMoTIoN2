#pragma once
#include "CBlockMap.h"
#include "CLibrary.h"
#include "Refactor.h"
namespace game_framework
{
	class CMapManager
	{
	public:
		CMapManager();
		~CMapManager();
		int GetNowMap();
		int GetUpMap();
		int GetDownMap();
		int GetLeftMap();
		int GetRightMap();
		int GetLoadMap();

		void ChangeMap(int);
		void LoadMapBitmap();
		void OnShow();
		CMovingBitmap* GetBitmap();
		CLayer layer;
	private:
		int nowMap;
		int loadMap;
		CBlockMap blockMap[MAX_MAP_NUMBER];

		void InitializeCBlockMap();
	};
}