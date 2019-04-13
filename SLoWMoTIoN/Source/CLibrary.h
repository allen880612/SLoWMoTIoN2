#pragma once
#include "Refactor.h"
namespace myLibrary
{
	char* ConvertCharPointToString(string, string, int);
	void DeleteCharPoint(vector<char*>&);
	int GetPostive(int);
	int GetRandom(int, int); //得到Random數字，min <= k <= max

	class BitmapPath
	{
	public:
		BitmapPath() {};
		BitmapPath(string _fold, string _name, int _number){
			ziliaojia = _fold; name = _name; number = _number;
		};
		~BitmapPath() {};
		string ziliaojia;
		string name;
		int number;
	};
}
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
		CMovingBitmap*  AddBitmap(int, COLORREF = CLR_INVALID);
		// 增加一張圖形至動畫(圖的編號及透明色)
		CMovingBitmap*  AddBitmap(char *, COLORREF = CLR_INVALID);
		// 增加一張圖形至動畫(圖的編號及透明色)
		void  LoadBitmap(vector<char*>, COLORREF = CLR_INVALID);
		//已字串陣列載入圖片
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

	#pragma region Camera
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
			void	Initialize();
			void	Reset();
			void	SetCameraBoundary(int, int);
			static CCamera* Instance();
		private:
			int x, y;
			int max_left, max_right;
			static CCamera camera;
	};
	#pragma endregion

}