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
		int GetSplitLeft();
		int GetSplitRight();
		int GetX1();
		int GetX2();
		int GetBitmapWidth();
		void SetMovingLeft(bool);
		void SetMovingRight(bool);
		void OnMove();


		void ChangeMap(int, string);
		void LoadMapBitmap();
		void OnShow();
		CMovingBitmap* GetBitmap();
		CLayer layer;
	private:
		const int directionX = 20;
		int nowMap;
		int loadMap;
		CBlockMap blockMap[MAX_MAP_NUMBER];
		int x;
		bool isMovingLeft;
		bool isMovingRight;

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
		void AddObject(CAnimate*, int);
		void ShowLayer();
	private:
		vector <CMovingBitmap*> layerBitmap[MAX_LAYER_NUMBER];
		vector <CAnimate*> layerAnimate[MAX_LAYER_NUMBER];
	};
	#pragma endregion
}