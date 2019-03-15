#pragma once
#include "CBlockMap.h"
#include "CLibrary.h"
#include "Refactor.h"
//CManager: 地圖管理者 and 圖層管理者，供mygame使用
namespace game_framework
{
	#pragma region - MapManager -
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
	#pragma endregion

	#pragma region - layerManager -
	class CLayerManager
	{
	public:
		CLayerManager();
		~CLayerManager();
		void Clear();
		void AddObject(CMovingBitmap*, int);
		void AddObject(CAnimation*, int);
		void ShowLayer();
	private:
		vector <CMovingBitmap*> layerBitmap[MAX_LAYER_NUMBER];
		vector <CAnimation*> layerAnimation[MAX_LAYER_NUMBER];
	};
	#pragma endregion
}