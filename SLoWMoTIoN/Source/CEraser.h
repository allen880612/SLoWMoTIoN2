#pragma once
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
		int  Height();
		int  Width();
		bool GetMovingLeft();           // 方方是否在往左走
		bool GetMovingRight();          // 方方是否再往右走
		void Initialize();				// 設定擦子為初始值
		void LoadBitmap();				// 載入圖形
		// 由路徑載入圖形(資料結, name, 張數)
		void LoadBitmap(string, string, int);
		
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

	protected:
		CAnimate animation;			// 擦子的動畫
		int height, width;			// 擦子的高、寬
		int x, y;					// 擦子左上角座標

		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		bool canMoving;				// 是否可以移動
	};
	#pragma endregion

	#pragma region - CRole -
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

		bool GetIsFire();
		void SetIsFire(bool);
		void Fire(int, int);			//傳入滑鼠座標
		void SetFirePosition(int, int);
		

	protected:
		vector<CScallion>	scallion;
		bool isJumping;				// 是否正在跳躍
		bool canJumping;			// 是否可以跳躍
		bool isFire;				// 是否正在射擊
		int init_velocity;			// 往上的初速度
		int velocity;				// 速度
		int gravity;				// 重力
		char last_right_left;
		int mouse_x, mouse_y;
		CTimer shoot_cd;
	};
	#pragma endregion

	#pragma region - CNPC -
	class CNPC : public CEraser
	{
		friend class CScallion;
	public:
		CNPC();
		CNPC(int, int, string, string, int);
		void SetXY(int, int);
		bool IsValid();
		~CNPC();
	private:
		int initX;
		bool isValid;
	};
	#pragma endregion
}