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
	string getFileName(string); //�o��file���ɦW(���t���ɦW)
	vector<string> SplitString(string);
	void DeleteCharPoint(vector<char*>&);
	int GetPostive(int);
	int GetRandom(int, int); //�o��Random�Ʀr�Amin <= k <= max
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
		
		void  LoadBitmap(vector<string>, COLORREF = CLR_INVALID);	//�H�r��}�C���J�Ϥ�
		void  LoadBitmap(string, string, int, COLORREF = CLR_INVALID);
		void  LoadBitmap(BitmapPath);
		
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
		void  SetIndex(int);
		int   GetIndex();

		void  CopyAnimateInformation(CAnimate *);

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

	#pragma region - CDialog -
	//I don't know what time use this?
	//�o�O�ڥ��x?
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
			CButton(BitmapPath, CPoint, bool, bool);	//���|�B��l�I�B��l���A, �O�_�ݭn�P�ƹ��I��
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

			void	OnMove();					//��sButton���A�A���ݭn�ɤ]�i�����ܦ�m
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
			void Add(int n);			// �W�[��ƭ�
			int  GetInteger();			// �^�Ǿ�ƭ�
			void LoadBitmap();			// ���J0..9�έt�����ϧ�
			void LoadBitmap(string, string);

			void SetInteger(int);		// �]�w��ƭ�
			void SetTopLeft(int, int);	// �N�ʵe�����W���y�в��� (x,y)
			void ShowBitmap();			// �N�ʵe�K��ù�
			void ShowBitmap(double);	// �N�ʵe�[�W���v�Y��K��ù�
			//const int NUMDIGITS;			// �@���NUMDIGITS�Ӧ��
			int NUMDIGITS;

			bool IsNull() { return !isBmpLoaded; };
			void SetValid(bool _flag) { isValid = _flag; };
			bool GetValid() { return isValid; };

			CMovingBitmap digit[11]; // �x�s0..9�έt�����ϧ�(bitmap)
			CMovingBitmap number[4];
			CLayer layer;

		private:
			int  x, y;						// ��ܪ��y��
			int  n;							// ��ƭ�
			bool isBmpLoaded;				// �O�_�w�g���J�ϧ�
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

		int   Height(); 				// ���o�ʵe������
		int   Width();
		int   Left();					// ���o�ʵe�����W���� x �y��
		int   Top();					// ���o�ʵe�����W���� y �y��
		
		void  SetTopLeft(int, int);		// �N�ʵe�����W���y�в��� (x,y)
							// ���o�ʵe���e��
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

		
		bool	isBmpLoaded;				// �O�_�w�g���J�ϧ�
		bool	isValid;

		int		x, y;						// ��ܪ��y��
		int		action_index;
		string	action;
		string	faceTo;					// �ثe�����V

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
		#pragma region -- C plus plus �u���ܴ� --
		//�ڥL�� �b�o�̩� CBitmap �| �z��??
		//�F�AC plus plus �u���ܴ� �ާA����
		//��²�满��
		//1.����bvector�̭���CBitmap
		//2.CBitmap�i�H����Load
		//3.����b�o�̩�CBitmap �L�|�R��default operator=
		//�N��Aoverload operator= �L�|���A�L�k�s���A�ާA����
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
		void FadeIn(); //�H�J �C��alpha+3
		void FadeOut(); //�H�X �C��alpha-4
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
		CStatusBoard(CPoint, int, int);	// ��l�I, ��q, EQ
		//~CStatusBoard();

		void Load();
		void Initialize(CPoint, int, int);	// ��l�I, ��q, EQ
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
		int dHP, dEQ; // �@�������e��
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
		CBossBoard(CPoint, int);	// ��l�I, ��q �M�ӨS Copy Constructor �N�Τ���
		
		void Load();
		void Initialize(CPoint);	// ��l�I
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
		int dHP; // �@�������e��
		int hp;
		bool isShow;

	};


	#pragma endregion


}