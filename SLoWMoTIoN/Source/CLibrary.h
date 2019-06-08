#pragma once
#include "Refactor.h"
namespace myLibrary
{
	char* ConvertCharPointToString(string, string, int = -1);
	//char* ConvertCharPointToString(BitmapPath)
	char* ConvertCharPointToString(string);
	COLORREF ConvertStringToColor(string);
	bool ConvertStringToBoolen(string);
	void getFolderFile(string, vector<string>*); 
	int getFolerFileNumber(string);
	string getFileName(string); //得到file的檔名(不含副檔名)
	vector<string> SplitString(string);
	void DeleteCharPoint(vector<char*>&);
	int GetPostive(int);
	int GetRandom(int, int); //得到Random數字，min <= k <= max
	int ConvertStringToInt(string);
	class BitmapPath
	{
	public:
		BitmapPath() {};
		BitmapPath(string _fold, string _name, int _number) {
			ziliaojia = _fold; name = _name; number = _number;
			color = RGB(255, 255, 255);
		};
		BitmapPath(string _fold, string _name, int _number, COLORREF _color) {
			ziliaojia = _fold; name = _name; number = _number;
			color = _color;
		};
		BitmapPath(vector<string> _path, COLORREF _color) {
			path = _path;
			color = _color;
		};
		~BitmapPath() {};
		string ziliaojia;
		string name;
		int number;
		COLORREF color;
		vector<string> path;
	};
}
using namespace myLibrary;
namespace game_framework
{
	int ScreenX(int, int);

	bool IsPointInRect(CPoint, CRect);
	bool IsRectCollision(CRect, CRect);

	#pragma region - timer -
	class CTimer
	{
	public:
		CTimer();
		CTimer(int);
		CTimer(double);
		~CTimer();
		void	CountDown();
		double	GetTime();
		int		GetTime(int);
		bool	IsTimeOut();
		void	ResetTime(double);
		void	ResetTime();

		void operator=(CTimer); //運算子多載，方便在GameStateRun::OnBeginState中重構Timer
	private:
		const double reflash = (1000 / GAME_CYCLE_TIME); //一秒刷新幾次
		double time;
		double initTime;
	};
	#pragma endregion

	#pragma region - CAnimate -
	class CAnimate
	{
	public:

		CAnimate();
		~CAnimate();
		CMovingBitmap*  AddBitmap(int, COLORREF = CLR_INVALID);		// 增加一張圖形至動畫(圖的編號及透明色)
		CMovingBitmap*  AddBitmap(char *, COLORREF = CLR_INVALID);	// 增加一張圖形至動畫(圖的編號及透明色)
		
		void  LoadBitmap(vector<string>, COLORREF = CLR_INVALID);	//以字串陣列載入圖片
		void  LoadBitmap(string, string, int, COLORREF = CLR_INVALID);
		void  LoadBitmap(BitmapPath);
		
		int   Height();					// 取得動畫的高度
		int   Left();					// 取得動畫的左上角的 x 座標
		void  OnMove(int);				// 依照方向更換bitmap
		void  OnMove();
		void  OnShow();					// 將動畫貼到螢幕
		void  Reset();					// 重設播放順序回到第一張圖形
		void  SetTopLeft(int, int);		// 將動畫的左上角座標移至 (x,y)
		int   Top();					// 取得動畫的左上角的 y 座標
		int   Width();					// 取得動畫的寬度
		bool  IsNull();
		void  ReleaseAnimate();
		void  SetValid(bool);
		bool  GetValid();
		void  SetIndex(int);
		int   GetIndex();

		void  CopyAnimateInformation(CAnimate *);

		void  ResetDelayTime(double);
		CRect GetRect();
	private:

		vector<CMovingBitmap>			bmp;
		int								bmp_amount;
		int								x, y;			// 動畫的座標
		int								bmp_index;
		bool							isValid;
		CTimer							delayTimer;
		double							delayTime;
	};
#pragma endregion

	#pragma region - CLayer -

	class CLayer
	{
	public:
		CLayer();
		~CLayer();
		void SetLayer(int);
		int GetLayer();
	private:
		int layer;
	};
	#pragma endregion

	#pragma region - CDialog -
	//I don't know what time use this?
	//這是我打ㄉ?
	class CDialog
	{
	public:
		CDialog();
		CDialog(string, string);
		~CDialog();
		bool GetTriggered();
		void SetTriggered();
		void Initialize();
		//void SetTxt();
		int GetTxtSize();

		string GetAvatar(unsigned int);
		string GetDialogTxt(unsigned int);
		COLORREF GetDialogColor(unsigned int);

		string GetMode();

		void LoadTxt();
	private:
		string path;
		string mode;

		vector<string> txt;
		vector<string> avatar;
		vector<COLORREF> color;

		bool IsTriggered;
		bool CanReTrigger;

		//COLORREF ConvertStringToColor(string);
	};
	#pragma endregion

	#pragma region - Camera -
	class CCamera
	{
		public:
			CCamera();
			~CCamera();
			int		GetX();
			int		GetY();
			void	SetXY(int, int);
			void	AddX(int);
			void	AddY(int);
			void    SetCanMoving(bool);
			void	Initialize();
			void	Reset();
			void	SetCameraBoundary(int, int);
			static CCamera* Instance();
		private:
			int x, y;
			int max_left, max_right;
			bool canMoving;
			static CCamera camera;
	};
	#pragma endregion

	#pragma region - CButton -
	class CButton
	{
		friend class CButtonManager;
		public:
			CButton();
			CButton(const CButton&);
			CButton(BitmapPath, CPoint, bool, bool);	//路徑、初始點、初始狀態, 是否需要與滑鼠碰撞
			//~CButton();
			int		GetX();
			int		GetY();
			void	SetXY(int, int);
			void	SetState(bool);
			bool	GetState();
			int		Width();
			int		Height();
			void    SetName(string _name) { name = _name; };
			string  GetName() { return name; };

			void	LoadBitmap();
			void	LoadBitmap(BitmapPath);

			void	OnMove();					//更新Button狀態，有需要時也可更改顯示位置
			void	OnShow();					
			void	Initialize();
			void	Initialize(CPoint, bool);
			void	SetValid(bool);
			bool	GetValid();
			

			void CollisonMouse(CPoint);
			void ClickButton();

			bool IsCollisionMouse(CPoint);
			CAnimate* GetAnimate();
			void operator=(const CButton&);

		private:
			CAnimate animation;
			BitmapPath loadpath;
			string name;
			int x, y;
			bool needCollision;
			bool state;
			bool valid;

	};
	#pragma endregion

	#pragma region - CInteger -
	class CInteger {
		public:
			CInteger(int = 5);			// default 5 digits
			void Initialize(CPoint, int, int = 2);
			void Add(int n);			// 增加整數值
			int  GetInteger();			// 回傳整數值
			void LoadBitmap();			// 載入0..9及負號之圖形
			void LoadBitmap(string, string);

			void SetInteger(int);		// 設定整數值
			void SetTopLeft(int, int);	// 將動畫的左上角座標移至 (x,y)
			void ShowBitmap();			// 將動畫貼到螢幕
			void ShowBitmap(double);	// 將動畫加上倍率縮放貼到螢幕
			//const int NUMDIGITS;			// 共顯示NUMDIGITS個位數
			int NUMDIGITS;

			bool IsNull() { return !isBmpLoaded; };
			void SetValid(bool _flag) { isValid = _flag; };
			bool GetValid() { return isValid; };

			CMovingBitmap digit[11]; // 儲存0..9及負號之圖形(bitmap)
			CMovingBitmap number[4];
			CLayer layer;

		private:
			int  x, y;						// 顯示的座標
			int  n;							// 整數值
			bool isBmpLoaded;				// 是否已經載入圖形
			bool isValid;
			
	};
	#pragma endregion

	#pragma region - CAction -
	class CAction {
	public:
		CAction();
		
		void	OnMove(string);
		void	OnShow();

		//void	LoadAction(vector<CMovingBitmap>*, );
		void  Initialize();
		void  LoadAction(string, BitmapPath);	//action ,BitmapPath

		int   Height(); 				// 取得動畫的高度
		int   Width();
		int   Left();					// 取得動畫的左上角的 x 座標
		int   Top();					// 取得動畫的左上角的 y 座標
		
		void  SetTopLeft(int, int);		// 將動畫的左上角座標移至 (x,y)
							// 取得動畫的寬度
		bool	IsNull();

		void	SetAction(string);
		string  GetAction();
		void	SetValid(bool);
		bool	GetValid();
		void	SetIndex(int);
		int		GetIndex();
		void	SetFaceTo(string);
		string	GetFaceTo();

		CLayer	layer;
		CMovingBitmap* GetNowBitmap();
		CRect GetRect();
		
		int GetActionHeight(string);

	private:
		CMovingBitmap *nowBitmap;
		//vector<CMovingBitmap>  nowAction;
		vector<CMovingBitmap>*  nowAction;
		map<string, vector<CMovingBitmap> >  paser;
		CTimer delayTimer;

		map <string, double> delayAdapter;
		double delay_run = 0.08;
		double delay_idle = 0.12;
		double delay_jump = 0.04;


		

		/*vector<CMovingBitmap>	action_run;
		vector<CMovingBitmap>	action_idle;
		vector<CMovingBitmap>	action_jump;

		vector<CMovingBitmap>	action_run_L;
		vector<CMovingBitmap>	action_idle_L;
		vector<CMovingBitmap>	action_jump_L;*/

		
		bool	isBmpLoaded;				// 是否已經載入圖形
		bool	isValid;

		int		x, y;						// 顯示的座標
		int		action_index;
		string	action;
		string	faceTo;					// 目前面對方向

	};
	#pragma endregion

	#pragma region - CEnd -
	class CEnd
	{
	public:
		CEnd();
		CEnd(string);
		~CEnd();
		string GetBmpPath(int);
		string GetTxt(int);
		void SetGetEnd(bool f = true) { isGet = f; }
		bool IsGetEnd() { return isGet; };
	private:
		string endName;
		vector<string> bmpPath;
		vector<string> txt;
		#pragma region -- C plus plus 真的很棒 --
		//我他媽 在這裡放 CBitmap 會 爆錯??
		//幹，C plus plus 真的很棒 操你媽的
		//我簡單說啦
		//1.不能在vector裡面放CBitmap
		//2.CBitmap可以重複Load
		//3.不能在這裡放CBitmap 他會刪掉default operator=
		//就算你overload operator= 他會說你無法存取，操你媽的
		//CBitmap endBmp;
		#pragma endregion

		void LoadEnd();
		void LoadBmpTxt(string);
		void LoadBmpTxt(string, int, int);	//end name, bmp amount, txt amount
		bool isGet;
	};
	#pragma endregion

	#pragma region - CToumeiImage -
	class CToumeiImage
	{
	public:
		CToumeiImage();
		CToumeiImage(int, int);
		CToumeiImage(string, int, int);
		~CToumeiImage();
		void SetAlpha(int _a);
		int GetAlpha() { return alpha; };
		void SetBmp(string _path);
		void SetFadeInOut(int, int);
		void FadeIn(); //淡入 每次alpha+3
		void FadeOut(); //淡出 每次alpha-4
		void DrawImage();
		bool temp = false;
	private:
		CBitmap bmp;
		string bmpLoadPath;
		int alpha = 0;
		int dFadeInValue = 3;
		int dFadeOutValue = -4;
	};
	#pragma endregion

	#pragma region - CWindows -
	class CWindows
	{
		friend class CScrollWindows;
		
		public:
			CWindows();
			CWindows(CPoint);
			~CWindows();
			
			void LoadResource();
			void Initialize(CPoint);
			void Clear();

			void Open();
			void Close();

			bool IsCollisionClose(CPoint);
			bool IsOpen();

			void SetCloseButton(CPoint);
			void SetXY(CPoint);
			
			void CollisionClose(CPoint);
			void OnCycle();
			void OnShow();

		protected:
			int x, y;
			CButton *closeButton;
			CMovingBitmap background;
		private:
			bool isOpen;
			bool isLoaded;
			
	};

	class CScrollWindows : public CWindows
	{
			
		public:
			CScrollWindows();
			~CScrollWindows();

			void LoadResource();
			void Initialize(CPoint);
			//void Clear();

			void Close();
			void OnCycle();
			void OnShow();
			void OnScrolling(short _s);

			string GetCollisionButtonName(CPoint);

		private:
			CMovingBitmap cover;
			vector< vector <CButton> > endingVector;
			vector<string> endName;
			int colNum, rowNum;
			int img_x, img_y;
			int img_height, img_width;
			int limit_top, limit_buttom;
	};
	#pragma endregion	

	#pragma region - CStatusBoard -
	class CStatusBoard
	{

	public:
		CStatusBoard();
		CStatusBoard(CPoint, int, int);	// 初始點, 血量, EQ
		//~CStatusBoard();

		void Load();
		void Initialize(CPoint, int, int);	// 初始點, 血量, EQ
		void Clear();

		void UpdateBar(int, int);
		
		void SetXY(CPoint);
		void SetDeltaBar(int, int);
		void SetDeltaBar();
		int GetHP() { return hp; };
		int GetEQ() { return eq; };
		
		void OnCycle(int ,int);
		void OnShow();

	protected:

		CMovingBitmap HP_frame, EQ_frame;
		CMovingBitmap HP_bar, EQ_bar;
		CMovingBitmap avatar, avatar_frame;
		//bool useSecondBar = false;
	private:
		int dHP, dEQ; // 一次扣的寬度
		int hp, eq;

	};
	

	#pragma endregion

	#pragma region - CBossBoard -
	class CBossManager;
	class CBossBoard
	{
		friend class CBossManager;

	public:
		CBossBoard();
		CBossBoard(CPoint, int);	// 初始點, 血量 然而沒 Copy Constructor 就用不到
		
		void Load();
		void Initialize(CPoint);	// 初始點
		void Clear();

		void UpdateBar(int);

		void SetXY(CPoint);
		void SetDeltaBar(int);
		void SetShow(bool);
		void SetHP(int _hp);

		int GetHP() { return hp; };
		bool IsShow() { return isShow; };

		void OnCycle(CBossManager*);
		void OnShow();

	protected:
		map<string, CMovingBitmap> bossAvatar;

		CMovingBitmap HP_frame;
		CMovingBitmap HP_bar;
		CMovingBitmap avatar, avatar_frame;
		//bool useSecondBar = false;
	private:
		CPoint initPos;
		int dHP; // 一次扣的寬度
		int hp;
		bool isShow;

	};


	#pragma endregion


}