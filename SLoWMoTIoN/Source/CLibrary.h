#pragma once
#include "Refactor.h"
namespace myLibrary
{
	char* ConvertCharPointToString(string, string, int = -1);
	//char* ConvertCharPointToString(BitmapPath)
	char* ConvertCharPointToString(string);
	void getFolderFile(string, vector<string>*); 
	void DeleteCharPoint(vector<char*>&);
	int GetPostive(int);
	int GetRandom(int, int); //得到Random數字，min <= k <= max

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
		~BitmapPath() {};
		string ziliaojia;
		string name;
		int number;
		COLORREF color;
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
		
		void  LoadBitmap(vector<char*>, COLORREF = CLR_INVALID);	//以字串陣列載入圖片
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

	#pragma region - Dialog -
	//I don't know what time use this?
	class CDialog
	{
	public:
		CDialog();
		CDialog(string, string, bool);
		~CDialog();
		bool GetTriggered();
		void SetTriggered();
		void Initialize();
		//void SetTxt();
		int GetTxtSize();
		string GetAvatar(unsigned int);
		string GetDialogTxt(unsigned int);

		string GetMode();

		void LoadTxt();
	private:
		string path;
		string mode;
		vector<string> txt;
		vector<string> avatar;
		bool IsTriggered;
		bool CanReTrigger;
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
			~CButton();
			int		GetX();
			int		GetY();
			void	SetXY(int, int);
			void	SetState(bool);
			bool	GetState();
			int		Width();
			int		Height();

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
			void SetInteger(int, int = 5);		// 設定整數值
			void SetTopLeft(int, int);	// 將動畫的左上角座標移至 (x,y)
			void ShowBitmap();			// 將動畫貼到螢幕
			void ShowBitmap(double);	// 將動畫加上倍率縮放貼到螢幕
			//const int NUMDIGITS;			// 共顯示NUMDIGITS個位數
			int NUMDIGITS;

			CMovingBitmap digit[11]; // 儲存0..9及負號之圖形(bitmap)
			CMovingBitmap number[4];
			CLayer layer;

		private:
			int  x, y;						// 顯示的座標
			int  n;							// 整數值
			bool isBmpLoaded;				// 是否已經載入圖形
			
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
		public:
			CWindows();
			CWindows(CPoint);
			~CWindows();
			
			void LoadResource();
			void Initialize(CPoint);

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
		CScrollWindows();
		~CScrollWindows();
		public:

		private:
	};
	#pragma endregion	

}