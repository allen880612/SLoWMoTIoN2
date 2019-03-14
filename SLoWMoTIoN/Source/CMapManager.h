#pragma once
#include "CBlockMap.h"
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
		int GetLayer();
		void SetLayer(int);

		void ChangeMap(int);
		void LoadMapBitmap();
		void OnShow();
		CMovingBitmap* GetBitmap();
	private:
		const int MAX_MAP_NUMBER = 3;
		int nowMap;
		int loadMap;
		CBlockMap blockMap[3];
		int layer;

		void InitializeCBlockMap();
	};
}