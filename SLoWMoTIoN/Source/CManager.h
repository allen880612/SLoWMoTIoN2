#pragma once
#include "CBlockMap.h"
#include "Refactor.h"
#include "CEraser.h"
#include "CBoss.h"
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
		CPasserby* AddPasserby(vector<int> , int );
		void CreatePasserby(int, vector<int>, int);
		void DeletePasserby(vector<CPasserby*>::iterator);
		vector<CPasserby*> GetPasserby();
		CLayer layer; //為了layerManager做的髒髒東東
		
		void MoveWithMap(string);
	private:
		vector<CPasserby*> passerby;
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
		void SetXY(int, int);

		bool IsMapEnd();

		void ChangeMap(int, string);
		void LoadMapBitmap();
		void OnShow();
		CMovingBitmap* GetBitmap();
		CAnimate* GetNpc(int);
		CLayer layer;

		vector<CPasserby*>* GetPasserby();
		void DeletePasserby(vector<CPasserby*>::iterator); //temp
		void AddPasserby();
		void Initialize();
		CPasserbyManager passerbyManager; //temp
	private:
		const int directionX = MOVE_DISTANCE;
		int nowMap;
		int loadMap;
		string loadMapPath;
		CBlockMap blockMap[MAX_MAP_NUMBER];
		CMovingBitmap background;
		int x, init_x;
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

	#pragma region - DialogManager -
	class CDialogManager
	{
	public:
		CDialogManager();
		~CDialogManager();
		void OnCycle(); //要重複執行的事情
		void Initialize();
		static CDialogManager* Instance();
		void Start(string);
		void Next();
		void Stop();
		void ShowText();
		bool GetDialogState();
	private:
		void Dialog();
		map<string, vector<string> > txt;
		map<string, CDialog> dialogmap;
		map<string, CMovingBitmap> dialogAvatar;

		CMovingBitmap dialog_background;

		//CMovingBitmap avatar_role;
		//CMovingBitmap avatar_xingting;
		CMovingBitmap avatar_null; //隨便load一張圖，給予他valid = false屬性，對話結束的時候將avatar指向這傢伙，使LayerManager不顯示avatar

		CLayer backgroundLayer;
		CLayer avatarLayer;

		CMovingBitmap avatar;
		static CDialogManager dialogManager;

		CAnimate textNext;
		
		int step;
		string showtext;
		int nowShowTextSize;
		bool IsBitmapLoaded = false;
		bool IsDialoging = false;
		bool IsPrintTips = false; //用於一個對話完畢，要到下一個對話的提示文字 (右下角的next)
		//string mode = "";
		CDialog *nowDialog;
		CTimer AddShowTextTimer;

		void Load_Image();
		void LoadDialog();
		void ShowText_Next();
		bool DebugMode = DEBUG_MODE;
	};
	#pragma endregion

	#pragma region - BossManager -
	class CBossManager
	{
	public:
		CBossManager();
		~CBossManager();
		void Initialize();
		void Clear();
		void TargetBoss(int);

		CBoss *targetBoss;
	private:
		map<string, CBoss*> bossInformation;
	};
	#pragma endregion
}