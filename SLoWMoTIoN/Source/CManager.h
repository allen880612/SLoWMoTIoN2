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
		string ziliaojia = "RES\\Role\\NPC";
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

		int GetBlockMapSize() { return (int)blockMap.size(); };
		vector<CBlock> *GetBlockVector();
		vector<CDoor> *GetDoorVector();

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

		void ReloadBlockMap(); //如果有更新map，線上更新
	private:
		const int directionX = MOVE_DISTANCE;
		int nowMap;
		int loadMap;
		string loadMapPath;
		vector<CBlockMap> blockMap;
		CMovingBitmap background;
		int x, init_x;
		bool isMovingLeft;
		bool isMovingRight;
		int npcNumber;

		void InitializeCBlockMap();

		int max_map_number = MAX_MAP_NUMBER;
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
		void AddObject(CAction*, int);

		void ShowLayer();
		static CLayerManager* Instance();
		void Initialize();
	private:
		vector <CMovingBitmap*> layerBitmap[MAX_LAYER_NUMBER];
		vector <CAnimate*> layerAnimate[MAX_LAYER_NUMBER];
		vector <CAction*> layerAction[MAX_LAYER_NUMBER];
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
		void LoadDialog(string, string, bool);

		void OpenMusicPlayer(string flag) { musicFromNPC = flag; };
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
		bool IsDialogLoad = false;
		bool IsPrintTips = false; //用於一個對話完畢，要到下一個對話的提示文字 (右下角的next)

		string musicFromNPC = "";
		//string mode = "";
		CDialog *nowDialog;
		string nowTxtName; //目前使用的文本名稱
		CTimer AddShowTextTimer;

		void Load_Image();
		void LoadDialog();
		void ShowText_Next();
		void DialogWithSound(); //例外處理 - 配合對話播出語音 (因為不是每句都有 所以用利外處理)
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
		void SetBattle(bool flag) { isBattle = flag; };
		bool IsBattle() { return isBattle; };
		void BossDead();
		CBoss *targetBoss;
	private:
		map<string, CBoss*> bossInformation;
		bool isBattle = false;
	};
	#pragma endregion

	#pragma region - NPCManager -
	class CNPCManager
	{
	public:
		CNPCManager();
		~CNPCManager();
		void Clear();
		void Initialize(int = NOW_MAP);
		void ChangeMap(int, int);
		void SetNPCValid(int, bool);
		void OnMove(int);
		vector<CNPC*>* GetNpc(int nowMap) { return &npc[nowMap]; };
	private:
		vector<CNPC*> npc[99];

		void LoadNPC();
		int map_max_Number;
	};
	#pragma endregion

	#pragma region - EventManager -
	class CEventManager
	{
	public:
		CEventManager();
		~CEventManager();
		void Initialize();
		void SetGameStateRun(CGameStateRun*);
		void trigger();
	private:
		CGameStateRun *gameState = nullptr;
		bool tips = true;
		bool dialogWithXingting = false;
	};
	#pragma endregion

	#pragma region - CEndManager -
	class CEndManager
	{
	public:
		CEndManager();
		~CEndManager();
		void Initialize();
		void Start(string);
		void Stop();
		bool GetEndingState() { return isEnding; };
		void OnCycle();
		bool IsPassEnd(string);
		static CEndManager* Instance() { return &endManager; };
	private:
		map< string, CEnd > endmap;
		CEnd *nowEnd;

		CToumeiImage endBmp;

		CTimer time_remaining;
		//CTimer time_switchNext_noDialog;

		int step = 0;
		int alpha = 0;

		bool isEnding = false;
		bool isLoadEnd = false;
		bool isFadeIn = true;
		bool isFadeOut = false;
		bool isOpenDialog = false;

		void LoadEnd();
		void Play();

		static CEndManager endManager;
	};
	#pragma endregion

	#pragma region - ButtonManager -
	class CButtonManager
	{
		public:
			CButtonManager();
			~CButtonManager();

			void Load();
			void Initialize();
			void Clear();
		
			void AddButton(CButton*);
			void CreateButton(BitmapPath, CPoint, bool, bool);	//loadpath, init location, init state
			
			void ClickButton(string);

			bool IsCollisionMouse(string);
			bool GetState(string);

			void UpdateState(CPoint);
			void ShowButton();
			void OnCycle();
			//void ShowButton();

		private:
			map <string, CButton*> buttons;
			CPoint mouse;
	};
	#pragma endregion

	#pragma region - CMapEditer -
	class CMapEditer
	{
	public:
		CMapEditer();
		~CMapEditer();
		void Initialize();
		void AddImage(vector<string>);
		void SetImageXY(CPoint point) {

		};
		void SetMapMoveDir(string);
		void OnSave();
		void OnMove();
		void OnShow();

		void SelectBlock(CPoint);
		void DragBlock(CPoint); //拖曳Block
		void DeleteBlock();

		bool GetMouseState() { return isMouseDown; };   //取得滑鼠狀態(是否按住)
		void SetMouseState(bool f) { isMouseDown = f; }; //設置滑鼠狀態

		void LoadMapInfo(string);
		void SetDPoint_MouseToTopLeft(CPoint mouse) {if (selectObj != NULL) dpoint_mouseToTopleft = CPoint(mouse.x - selectObj->bmp.Left() - cameraX, mouse.y - selectObj->bmp.Top());};
		void SetDPoint_MouseToTopLeft() { dpoint_mouseToTopleft = CPoint(0, 0); };

		bool isPrintNowMap = false;
		string GetNowMap();

		void CreateReloadMapInformation(); //按ESC離開mapEditer時，寫下reload的資訊
		void LoadReloadMapInformation();   //進入mapEditer時，如果上次有留下來的reload資訊就留著

		void StoreMapInformaion(); //儲存所有blockMap的資料

		bool IsInSelectMapMode() { //是否在選擇地圖的模式中
			return (selectMapMode == "up" || selectMapMode == "down" || selectMapMode == "left" || selectMapMode == "right");
		}

		void SetSelectMapZero(); //上/下/左/右的連結地圖 -> 歸零
		void SetSelectMapMode(string _mode);
		string GetSelectMapMode() { return "Set" + selectMapMode + "Map"; };

		void ClickArrow(CPoint);

		void SwitchMap(string);
		void GotoMap(int);

	private:
		#pragma region - image info -
		class ImageInfo
		{
		public:
			void SetXY(int _x, int _y, int camera){
				x = _x; y = _y; 
				bmp.SetTopLeft(x - camera, y);
			};
			ImageInfo() { x = y = 0; };
			ImageInfo(string _path, string _id)
			{
				id = _id;
				path = _path;
				char *address = ConvertCharPointToString(path);
				bmp.LoadBitmap(address);
				delete address;
				x = y = 0;
				bmp.SetTopLeft(0, 0);
			};
			~ImageInfo() {};
			CMovingBitmap bmp;
			string path;
			string id;
			int x = 0, y = 0;
		};
		#pragma endregion

		ImageInfo background;
		vector<ImageInfo> mapObj; //地圖上一些物件

		ImageInfo *selectObj;
		bool haveBG = false;
		bool isMouseDown;
		
		string saveTxtName;

		CPoint dpoint_mouseToTopleft; //儲存滑鼠座標與block座標的 x, y座標差

		CBlockMap bkmap;

		bool isMapRight, isMapLeft;
		int cameraX;

		bool isSaved; //曾經有儲存過地圖

		void CreateBlockToBkmap();
		void LoadBlockMap(string);
		string WriteSaveInfo(string, string, CPoint);

		int *printNowMap = NULL;
		int nowMap;
		int nowMapNumber; //reload前的地圖數 (求大佬賜個名，我可能沒有取名的天份)
		vector<bool> reloadMap;

		vector<CBlockMap> blockMap; //所有地圖
		string selectMapMode;
		int selectNowMap;

		#pragma region - Arrow -
		class Arrow
		{
		public:
			Arrow() { dir = ""; isLoad = false; };
			Arrow(string _dir) { dir = _dir; };
			~Arrow() {};
			void Initialize() {
				LoadImg();
				canShow = true;
			}
			void LoadImg(){
				if (!isLoad)
				{
					#pragma region - Load Image -
					if (dir == "up")
					{
						bmp.LoadBitmap("RES\\Object\\Arrow\\up_arrow.bmp", RGB(255, 255, 255));
						bmp.SetTopLeft(320 - bmp.Width() / 2, 0);
					}
					else if (dir == "down")
					{
						bmp.LoadBitmap("RES\\Object\\Arrow\\down_arrow.bmp", RGB(255, 255, 255));
						bmp.SetTopLeft(320 - bmp.Width() / 2, 480 - bmp.Height());
					}
					else if (dir == "left")
					{
						bmp.LoadBitmap("RES\\Object\\Arrow\\left_arrow.bmp", RGB(255, 255, 255));
						bmp.SetTopLeft(0, 240 - bmp.Height() / 2);
					}
					else if (dir == "right")
					{
						bmp.LoadBitmap("RES\\Object\\Arrow\\right_arrow.bmp", RGB(255, 255, 255));
						bmp.SetTopLeft(620 - bmp.Width(), 240 - bmp.Height() / 2);
					}
					#pragma endregion
					isLoad = true;
				}
			}
			bool CanShow() { return canShow; };
			void SetCanShow(bool f) { canShow = f; };
			CMovingBitmap* GetBmp() { return &bmp; };
			string GetDir() { return dir; };
		private:
			string dir;
			CMovingBitmap bmp;
			bool isLoad = false;
			bool canShow;
		};
		#pragma endregion

		Arrow arrow[4];
		void SetArrowCanShow();

		void NewMapInit();
	};
	#pragma endregion

}