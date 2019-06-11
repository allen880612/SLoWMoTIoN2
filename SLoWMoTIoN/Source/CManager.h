#pragma once
#include "CBlockMap.h"
#include "Refactor.h"
#include "CEraser.h"
#include "CBoss.h"
#include "CLibrary.h"

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
		CPasserby* GetPasserbyType(int);
		CPasserby* AddPasserby(vector<int> , int );
		void CreatePasserby(int, vector<int>, int, vector<CBlock>*);
		void DeletePasserby(vector<CPasserby*>::iterator);
		vector<CPasserby*> GetPasserby();
		CLayer layer;

	private:
		vector<CPasserby*> passerby;
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

		void ReloadBlockMap(); //�p�G����smap�A�u�W��s
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

		int max_map_number;
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
		void AddObject(CInteger*, int);

		void ShowLayer();
		static CLayerManager* Instance();
		void Initialize();
	private:
		vector <CMovingBitmap*> layerBitmap[MAX_LAYER_NUMBER];
		vector <CAnimate*> layerAnimate[MAX_LAYER_NUMBER];
		vector <CAction*> layerAction[MAX_LAYER_NUMBER];
		vector <CInteger*> layerInteger[MAX_LAYER_NUMBER];
		static CLayerManager layerManager;
	};
	#pragma endregion

	#pragma region - DialogManager -
	class CDialogManager
	{
	public:
		CDialogManager();
		~CDialogManager();
		void OnCycle(); //�n���ư��檺�Ʊ�
		void Initialize();
		static CDialogManager* Instance();
		void Start(string);
		void Next();
		void Stop();
		void ShowText();
		bool GetDialogState();
		void LoadDialog(string, string);

		void OpenMusicPlayer(string flag) { musicFromNPC = flag; };
	private:
		void Dialog();
		map<string, vector<string> > txt;
		map<string, CDialog> dialogmap;
		map<string, CMovingBitmap> dialogAvatar;

		CMovingBitmap dialog_background;
		CMovingBitmap avatar_null; //�H�Kload�@�i�ϡA�����Lvalid = false�ݩʡA��ܵ������ɭԱNavatar���V�o�å�A��LayerManager�����avatar
		CMovingBitmap avatar;
		CAnimate textNext;

		CLayer backgroundLayer;
		CLayer avatarLayer;

		static CDialogManager dialogManager;

		int step;
		string showtext;
		int nowShowTextSize;
		bool IsBitmapLoaded = false;
		bool IsDialoging = false;
		bool IsDialogLoad = false;
		bool IsPrintTips = false; //�Ω�@�ӹ�ܧ����A�n��U�@�ӹ�ܪ����ܤ�r (�k�U����next)

		string musicFromNPC = "";
		CDialog *nowDialog;
		string nowTxtName; //�ثe�ϥΪ��奻�W��
		CTimer AddShowTextTimer;

		void Load_Image();
		void LoadDialog();
		void ShowText_Next();
		void DialogWithSound(); //�ҥ~�B�z - �t�X��ܼ��X�y�� (�]�����O�C�y���� �ҥH�ΧQ�~�B�z)
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
		void OnCycle(int, CPoint);
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
		bool dialogWithFacaiSeed = false;
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

		vector<string> GetAllEndName();
	private:
		map< string, CEnd > endmap;
		CEnd *nowEnd;
		CToumeiImage endBmp;

		CTimer time_remaining;

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
			void CreateButton(CButton*);

			void ClickButton(string);

			bool IsCollisionMouse(string);
			bool GetState(string);
			string GetCollisionButtonName();

			void SetValid(bool _v) { isValid = _v; };
			void UpdateState(CPoint);
			void ShowButton();
			void OnCycle();

		private:
			map <string, CButton*> buttons;
			CPoint mouse;
			bool isValid = false;
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
		void SetImageXY(CPoint point) { };
		void SetMapMoveDir(string);
		void OnSave();
		void OnMove();
		void OnShow();

		void SelectBlock(CPoint);
		void DragBlock(CPoint); //�즲Block
		void DeleteBlock();

		bool GetMouseState() { return isMouseDown; };   //���o�ƹ����A(�O�_����)
		void SetMouseState(bool f) { isMouseDown = f; }; //�]�m�ƹ����A

		void SetDPoint_MouseToTopLeft(CPoint mouse) {if (selectObj != NULL) dpoint_mouseToTopleft = CPoint(mouse.x - selectObj->bmp.Left() - cameraX, mouse.y - selectObj->bmp.Top());};
		void SetDPoint_MouseToTopLeft() { dpoint_mouseToTopleft = CPoint(0, 0); };

		bool isPrintNowMap = false;
		string GetNowMap();

		void CreateReloadMapInformation(); //��ESC���}mapEditer�ɡA�g�Ureload����T
		void LoadReloadMapInformation();   //�i�JmapEditer�ɡA�p�G�W�����d�U�Ӫ�reload��T�N�d��

		void StoreMapInformaion(); //�x�s�Ҧ�blockMap�����

		bool IsInSelectMapMode() { //�O�_�b��ܦa�Ϫ��Ҧ���
			return (selectMapMode == "up" || selectMapMode == "down" || selectMapMode == "left" || selectMapMode == "right");
		}

		void SetSelectMapZero(); //�W/�U/��/�k���s���a�� -> �k�s
		void SetSelectMapMode(string _mode);
		string GetSelectMapMode() { return "Set" + selectMapMode + "Map"; };

		void ClickArrow(CPoint);

		void SwitchMap(string);
		void GotoMap(int);

	private:
		#pragma region - image info -
		class ImageInfo : public CSimpleMapObj
		{
		public:
			ImageInfo() : CSimpleMapObj(){ };
			ImageInfo(string _path, string _id) : CSimpleMapObj(_path, 0, 0)
			{
				id = _id;
				LoadImg();
				SetXY(x, y);
			};
			~ImageInfo() {};
			string id;
		};
		#pragma endregion

		ImageInfo background;
		vector<ImageInfo> mapObj; //�a�ϤW�@�Ǫ���
		ImageInfo *selectObj;

		bool haveBG = false;
		bool isMouseDown;
		
		string saveTxtName;
		CPoint dpoint_mouseToTopleft; //�x�s�ƹ��y�лPblock�y�Ъ� x, y�y�Юt

		CBlockMap bkmap;

		bool isMapRight, isMapLeft;
		int cameraX;

		bool isSaved; //���g���x�s�L�a��

		void CreateObjToBkmap();
		void LoadBlockMap(string);

		int *printNowMap = NULL;
		int nowMap;
		int nowMapNumber; //reload�e���a�ϼ� (�D�j�н�ӦW�A�ڥi��S�����W���ѥ�)
		vector<bool> reloadMap;

		vector<CBlockMap> blockMap; //�Ҧ��a��
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

	#pragma region - UIManager -
	class CRole;
	class UIManager
	{
	public:
		UIManager();

		void Load();
		void Initialize(CRole*, CBossManager*);
		void OnCycle(int);

	private:
		int left_time;	// Game left time
		int point;		// Usless kill passerby point

		CInteger uiTime, uiScore;
		CStatusBoard roleStatus;
		CBossBoard bossStatus;

		CRole *role;
		CBossManager *bManager;
	};
	#pragma endregion

}