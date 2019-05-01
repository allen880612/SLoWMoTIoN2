#pragma once
#include "Refactor.h"
namespace myLibrary
{
	char* ConvertCharPointToString(string, string, int = -1);
	//char* ConvertCharPointToString(BitmapPath)
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
	public:
		CButton();
		CButton(const CButton&);
		CButton(BitmapPath, CPoint, bool);	//路徑、初始點、初始狀態
		~CButton();
		int		GetX();
		int		GetY();
		void	SetXY(int, int);
		void	SetState(bool);
		bool	GetState();
		void	LoadBitmap(BitmapPath);
		void	OnMove();					//更新Button狀態，有需要時也可更改顯示位置
		void	OnShow();					
		void	Initialize();
		void	Initialize(CPoint, bool);
		CAnimate* GetAnimate();

		void CollisonMouse(CPoint);

		void operator=(const CButton&);

	private:
		CAnimate animation;
		BitmapPath loadpath;
		int x, y;
		bool state = true;

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
		
		void	OnMove(string );
		void	OnShow();

		//void	LoadAction(vector<CMovingBitmap>*, );
		void	SetAction(string);
		string  GetAction();
		void	LoadAction(string, BitmapPath);	//action ,BitmapPath

		int   Height(); 				// 取得動畫的高度
		int   Width();
		int   Left();					// 取得動畫的左上角的 x 座標
		int   Top();					// 取得動畫的左上角的 y 座標
		
		void  SetTopLeft(int, int);		// 將動畫的左上角座標移至 (x,y)
							// 取得動畫的寬度
		bool  IsNull();
		
		void  SetValid(bool);
		bool  GetValid();
		void  SetIndex(int);
		int   GetIndex();

		CLayer	layer;

	private:
		vector<CMovingBitmap>	*nowAction;

		map<string, vector<CMovingBitmap> >	    paser;

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
		char	face_to;					// 目前面對方向

	};
#pragma endregion
}