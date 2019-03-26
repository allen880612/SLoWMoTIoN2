#pragma once
#include "CBlockMap.h"
#include "CLibrary.h"
#include "Refactor.h"
#include "CEraser.h"
//CManager: 地圖管理者 and 圖層管理者 and Timer，供mygame使用
namespace game_framework
{
	#pragma region - passerbyManager -
	class CPasserbyManager
	{
		friend class CMapManager;
	public:
		CPasserbyManager();
		~CPasserbyManager();
		void Clear();
		void AddPasserbyManager(CNPC*);
		void CreatePasserby(int, vector<int>, int);
		void DeletePasserby(vector<CNPC*>::iterator);
		vector<CNPC*> GetPasserby();
	private:
		vector<CNPC*> passerby;
		string ziliaojia = "Role\\NPC";
		string name[10] = { "LUKA", "KALU" };
	};
	#pragma endregion
	
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
		int GetNpcNumber();
		int GetNpcLayer(int);
		bool GetNpcValid(int);

		void SetMovingLeft(bool);
		void SetMovingRight(bool);
		void OnMove();


		void ChangeMap(int, string);
		void LoadMapBitmap();
		void OnShow();
		CMovingBitmap* GetBitmap();
		CAnimate* GetNpc(int);
		CLayer layer;

		vector<CNPC*>* GetPasserby();
		void DeletePasserby(vector<CNPC*>::iterator); //temp
		void Initialize();
	private:
		const int directionX = 20;
		int nowMap;
		int loadMap;
		string loadMapPath;
		CBlockMap blockMap[MAX_MAP_NUMBER];
		CMovingBitmap background;
		CPasserbyManager passerbyManager;
		int x;
		bool isMovingLeft;
		bool isMovingRight;
		int npcNumber;

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