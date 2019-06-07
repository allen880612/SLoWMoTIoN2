#pragma once
#include "CBoss.h"
#include "CLibrary.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的擦子
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////
	
	#pragma region - CEraser -
	class CEraser
	{
	public:
		CEraser();
		~CEraser();
		int  GetX1();					// 擦子左上角 x 座標
		int  GetY1();					// 擦子左上角 y 座標
		int  GetX3();                   // 中新點 X 座標
		int  GetX2();					// 擦子右下角 x 座標
		int  GetY2();					// 擦子右下角 y 座標
		int  GetY3();                   // 中心點 Y 座標
		int  GetScore();
		int  Height();
		int  Width();
		bool GetMovingLeft();           // 方方是否在往左走
		bool GetMovingRight();          // 方方是否再往右走
		void Initialize();				// 設定擦子為初始值
		void LoadBitmap();				// 載入圖形
		// 由路徑載入圖形(資料結, name, 張數)
		void LoadBitmap(string, string, int, COLORREF);
		void LoadBitmap(BitmapPath);

		//void LoadAction(string, BitmapPath);	//載入動應動作
		
		void OnMove();					// 移動擦子
		void OnShow();					// 將擦子圖形貼到畫面
		CAnimate* GetAnimate();
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetXY(int nx, int ny);		// 設定擦子左上角座標

		void SetCanMoving(bool);
		bool GetCanMoving();
		CLayer layer;

		bool GetValid();
		void SetValid(bool);

	protected:
		CAnimate animation;			// 擦子的動畫
		//CAction  action;

		int height, width;			// 擦子的高、寬
		int x, y;					// 擦子左上角座標
		int score;

		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		bool canMoving;				// 是否可以移動
		

	private:
		int move_distance = MOVE_DISTANCE;
	};
	#pragma endregion

	#pragma region - CNPC -
	class CNPC : public CEraser
	{
	public:
		CNPC();
		CNPC(CPoint, BitmapPath, string);
		virtual ~CNPC();
		
		void SetCurrentXY(int, int);
		void SetXY();
		virtual void OnMove();
		virtual void Initialize();
		virtual void RoleCollision() {}; //碰撞到後要做的事情
		virtual void SetValid(bool);

	protected:
		#pragma region - init information -
		CPoint initPoint;
		BitmapPath initLoadPath;
		#pragma endregion

		int currentX, currentY;
		string id;
	};
	#pragma endregion

	#pragma region - CRole -
	class CBlackHole;
	class CBlock;
	class CDoor;
	class CRole : public CEraser
	{
		friend class CScallion;
	public:
		CRole();
		~CRole();
		int  GetX3();                   // 中新點 X 座標
		int  GetX2();					// 擦子右下角 x 座標
		int  GetY2();					// 擦子右下角 y 座標
		int  GetY3();                   // 中心點 Y 座標
		int  Height();
		int  Width();
		void SetXY(int _x, int _y);		// 設定擦子左上角座標
		bool GetValid();
		void SetValid(bool);

		void Load();
		void LoadAction(string, BitmapPath);	//載入動應動作

		void OnMove();
		void OnShow();
		bool GetMovingJump();
		void SetMovingJump(bool);
		bool GetCanJumping();
		void SetCanJumping(bool);
		void SetMouseXY(int, int);
		void AddScore(int);
		void SubHp();
		int  GetHp() { return hp; }; //我懶ㄌ

		void Initialize();

		bool GetIsFire();
		void SetIsFire(bool);
		void Fire(int, int);			//傳入滑鼠座標
		
		void SetDrop();
		bool GetDrop();
		int GetVelocity();
		void SetVelocity(int v) { velocity = v; };

		bool IsMoving();
		bool IsDead() { return isDead; };
		CAction* GetAction() { return &action; };

		#pragma region - Collision -
		bool IsCollisionBoss(CBoss*);

		#pragma region -- Collision - Block --
		bool IsCollisionBlock(CBlock*); //一般左右 跟block碰撞
		bool IsRoleOnBlock(CBlock*); //站在方塊上
		bool IsCollisionBlockOnJumping(CBlock*); //跳起來撞到方塊
		#pragma endregion

		bool IsCollisionDoor(CDoor*);

		bool IsCollisionLevel4(CScallion*);

		bool IsCollisionNPC(CNPC*);

		bool IsCollisionBlackHole(CBlackHole*);
		bool IsCollisionBlackHoleCenter(CBlackHole*);
		#pragma endregion

		vector<CScallion*>* GetScallion();
		

		void SetCatched(bool flag) { isCatched = flag; };
		bool IsCatched() { return isCatched; };

	protected:
		vector<CScallion*>	scallion;
		bool isJumping;				// 是否正在跳躍
		bool canJumping;			// 是否可以跳躍
		bool isFire;				// 是否正在射擊
		int init_velocity;			// 往上的初速度
		int velocity;				// 速度
		int gravity;				// 重力
		char last_right_left;
		int mouse_x, mouse_y;
		int move_distance = MOVE_DISTANCE; //每次移動的距離
		CTimer shoot_cd;

		CMovingBitmap decisionPoint;

		CRect collisionRect;
		CRect collisionDownRect;
		CRect collisionTopRect;
		CRect collisionDoorRect;

		void ResetCollisionRect();
	private:
		CAction  action;
		//CInteger hp_left;
		CInteger scoreInteger;
		CMovingBitmap blood;
		CMovingBitmap EQ;
		CMovingBitmap avatar;

		CMovingBitmap blood_frame;
		CMovingBitmap EQ_frame;
		CMovingBitmap avatar_frame;
		#pragma region - init information -
		int inithp = 20;
		bool isLoaded;
		string now_action;
		#pragma endregion

		int hp;
		
		bool isCatched;
		bool isDead;

	};
	#pragma endregion

	#pragma region - CPasserby -
	class CPasserby : public CEraser
	{
		friend class CScallion;
	public:
		CPasserby();
		CPasserby(int, int);
		void SetXY(int, int);
		void SetScore(int);
		void SetMoving();
		virtual void OnMove();
		//void Initialize();
		virtual ~CPasserby();
	protected:
		int initX;
		CTimer moveTimer;
		CTimer stopTimer;
		CTimer recreateTimer;
		int move_distance = 2;

		CAnimate leftAnimate;
		CAnimate rightAnimate;
		string faceto;
	};

	#pragma region - CPasserby1 - LUKA -
	class CLuka : public CPasserby
	{
	public:
		CLuka();
		CLuka(int, int);
		~CLuka();
	};
	#pragma endregion

	#pragma region - CPasserby2 - RIN -
	class CRin : public CPasserby
	{
	public:
		CRin();
		CRin(int, int);
		~CRin();
	};
	#pragma endregion

	#pragma region - CPasserby3 - mushroom -
	class CMushroom : public CPasserby
	{
	public:
		CMushroom();
		CMushroom(int, int);
		void OnMove();
		~CMushroom();
	
	};
	#pragma endregion

	#pragma endregion

	#pragma region - CNPC - No.1 -
	//CNPC1 > 普通的NPC (對話NPC)
	class CNPC1 : public CNPC
	{
	public:
		CNPC1();
		CNPC1(CPoint, BitmapPath, string, string); //座標 路徑 id 對話文本
		CNPC1(CPoint, BitmapPath, string, string, double); //座標 路徑 id 對話文本 一個動畫的影格的時間
		~CNPC1();
		void RoleCollision();
	private:
		string txt;
	};
	#pragma endregion

	#pragma region - CNPC - No.2 -

	#pragma endregion


	#pragma region - CNPC - No.3 -
	//CNPC3 > 音樂播放NPC
	class CNPC3 : public CNPC
	{
	public:
		CNPC3();
		CNPC3(CPoint, BitmapPath, string, string, string); //座標 路徑 ID 音樂 對話文本
		~CNPC3();
		void RoleCollision();
		void Initialize();
		void SetValid(bool);
		void OnMove();
	private:
		string openTxt;
		string openMusic;
		CMovingBitmap headphoneIcon;
		bool isLoadHeadPhoneIcon;
	};
	#pragma endregion


}