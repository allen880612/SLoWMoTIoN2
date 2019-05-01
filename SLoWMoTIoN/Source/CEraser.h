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
		int GetScore();
		int  Height();
		int  Width();
		bool GetMovingLeft();           // 方方是否在往左走
		bool GetMovingRight();          // 方方是否再往右走
		void Initialize();				// 設定擦子為初始值
		void LoadBitmap();				// 載入圖形
		// 由路徑載入圖形(資料結, name, 張數)
		void LoadBitmap(string, string, int, COLORREF);
		void LoadBitmap(BitmapPath);
		
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
		~CNPC();
		void Initialize();
		void SetCurrentXY(int, int);
		void SetXY();
		void OnMove();
		virtual void RoleCollision() {}; //碰撞到後要做的事情

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
	class CRole : public CEraser
	{
		friend class CScallion;
	public:
		CRole();
		~CRole();
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
		//void LoadBitmap();

		void Initialize();

		bool GetIsFire();
		void SetIsFire(bool);
		void Fire(int, int);			//傳入滑鼠座標
		
		bool GetDrop();
		int GetVelocity();

		#pragma region - Collision -
		bool IsCollisionBoss(CBoss*);
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
		void ResetCollisionRect();
	private:
		//unsigned AUDIO_THROW;\
		//unsigned AUDIO_JUMP;
		CAction  action;
		CInteger hp_left;
		CInteger scoreInteger;

		#pragma region - init information-
		int inithp = 20;
		bool isLoaded;
		#pragma endregion

		int hp;
		
		bool isCatched;

	};
	#pragma endregion

	#pragma region - CPasserby -
	class CPasserby : public CEraser
	{
		friend class CScallion;
	public:
		CPasserby();
		CPasserby(int, int, string, string, int, int);
		void SetXY(int, int);
		void SetScore(int);
		void SetMoving();
		void OnMove();
		//void Initialize();
		~CPasserby();
	private:
		int initX;
		CTimer moveTimer;
		CTimer stopTimer;
		CTimer recreateTimer;
		int move_distance = 2;
	};
	#pragma endregion

	#pragma region - CNPC - No.1 -
	//CNPC1 > 普通的NPC (對話NPC)
	class CNPC1 : public CNPC
	{
	public:
		CNPC1();
		CNPC1(CPoint, BitmapPath, string, string);
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
		CNPC3(CPoint, BitmapPath, string, string, string);
		~CNPC3();
		void RoleCollision();
	private:
		string openTxt;
		string openMusic;
	};
	#pragma endregion

}