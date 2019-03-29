#pragma once
#include "CBlockMap.h"
#include "Refactor.h"
#include "CEraser.h"

#include "CLibrary.h"
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
		CNPC* AddPasserby(vector<int> , int );
		void CreatePasserby(int, vector<int>, int);
		void DeletePasserby(vector<CNPC*>::iterator);
		vector<CNPC*> GetPasserby();
		CLayer layer; //為了layerManager做的髒髒東東
	private:
		vector<CNPC*> passerby;
		string ziliaojia = "Role\\NPC";
		string name[10] = { "LUKA", "RIN" };
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
		void AddPasserby();
		void Initialize();
		CPasserbyManager passerbyManager; //temp
	private:
		const int directionX = 20;
		int nowMap;
		int loadMap;
		string loadMapPath;
		CBlockMap blockMap[MAX_MAP_NUMBER];
		CMovingBitmap background;
		int x;
		bool isMovingLeft;
		bool isMovingRight;
		int npcNumber;

		void InitializeCBlockMap();
	};
	#pragma endregion

	#pragma region - layerManager -
	#pragma region - layerManager - How to use -
	//How to use layerManager
	//Step1 : include "CManager.h"
	//Step2 : confirm your animate or cmovingbitmap is correctly Load
	//Step3 : write "CLayerManager::Instance()->AddObject( (animate/cmovingbitmap), layer )"
	//Maybe the statement can write anywhere if Step 2 is true!
	//by the way, Notice your Step3 can't Add same animate or cmovingbitmap repeatly
    //            Notice If your animate or comvingbitmap is move, you must reset there point(use SetTopLeft(x, y))
	#pragma endregion
	class CLayerManager
	{
	public:
		CLayerManager();
		~CLayerManager();
		void Clear();
		void AddObject(CMovingBitmap*, int);
		void AddObject(CAnimate*, int);
		void ShowLayer();
		static CLayerManager* Instance();
		void Initialize();
	private:
		vector <CMovingBitmap*> layerBitmap[MAX_LAYER_NUMBER];
		vector <CAnimate*> layerAnimate[MAX_LAYER_NUMBER];
		static CLayerManager layerManager;
	};
	#pragma endregion


}