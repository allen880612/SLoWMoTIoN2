#pragma once
#include "Refactor.h"
namespace myLibrary
{
	char* ConvertCharPointToString(string, string, int);
	void DeleteCharPoint(vector<char*>&);
	int GetPostive(int);
	int GetRandom(int, int); //�o��Random�Ʀr�Amin <= k <= max

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

		void operator=(CTimer); //�B��l�h���A��K�bGameStateRun::OnBeginState�����cTimer
	private:
		const double reflash = (1000 / GAME_CYCLE_TIME); //�@���s�X��
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
		CMovingBitmap*  AddBitmap(int, COLORREF = CLR_INVALID);		// �W�[�@�i�ϧΦܰʵe(�Ϫ��s���γz����)
		CMovingBitmap*  AddBitmap(char *, COLORREF = CLR_INVALID);	// �W�[�@�i�ϧΦܰʵe(�Ϫ��s���γz����)
		
		void  LoadBitmap(vector<char*>, COLORREF = CLR_INVALID);	//�H�r��}�C���J�Ϥ�
		void  LoadBitmap(string, string, int, COLORREF = CLR_INVALID);
		
		int   Height();					// ���o�ʵe������
		int   Left();					// ���o�ʵe�����W���� x �y��
		void  OnMove(int);				// �̷Ӥ�V��bitmap
		void  OnMove();
		void  OnShow();					// �N�ʵe�K��ù�
		void  Reset();					// ���]���񶶧Ǧ^��Ĥ@�i�ϧ�
		void  SetTopLeft(int, int);		// �N�ʵe�����W���y�в��� (x,y)
		int   Top();					// ���o�ʵe�����W���� y �y��
		int   Width();					// ���o�ʵe���e��
		bool  IsNull();
		void  ReleaseAnimate();
		void  SetValid(bool);
		bool  GetValid();
		
		void  ResetDelayTime(double);
		CRect GetRect();
	private:

		vector<CMovingBitmap>			bmp;
		int								bmp_amount;
		int								x, y;			// �ʵe���y��
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

}