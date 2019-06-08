#pragma once
#include "CLibrary.h"
#include "CEraser.h"
#include "CBoss.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供繞圈圈的球
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	#pragma region - ball -
	class CBall
	{
	public:
		CBall();
		//bool HitEraser(CEraser *eraser);						// 是否碰到擦子
		bool IsAlive();											// 是否活著
		void LoadBitmap();										// 載入圖形

		void OnMove();											// 移動
		void OnShow();											// 將圖形貼到畫面
		void SetCurrentXY(double, double);
		void SetXY(int nx, int ny);								// 設定圓心的座標
		void SetIsAlive(bool alive);							// 設定是否活著
		void SetDelay(int d);									// 設定旋轉的速度
		CPoint GetCenterPoint() { return CPoint(x + animation.Width() / 2, y + animation.Height() / 2); };

		void LoadBitmap(string, string, int);
		void LoadBitmap(BitmapPath);
		CAnimate* GetAnimate();
		CLayer layer;
	protected:
		CAnimate animation;
		double currentX, currentY;     // /*當前的座標*/ //
		int x, y;					// 圓心的座標
		int dx, dy;					// 球距離圓心的位移量
		int index;					// 球的「角度」，0-17表示0-360度
		int delay_counter;			// 調整旋轉速度的計數器
		int delay;					// 旋轉的速度
		bool is_alive;				// 是否活著
	private:
		//bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
	};
#pragma endregion

	#pragma region - CScallion -
	class CScallion : public CBall
	{
	public:
		CScallion();
		CScallion(BitmapPath, CPoint, CPoint, int = 2);	//給兩CPoint + int 為參數時，第一個為目前座標，第二個為滑鼠座標，最後為重力參數
		CScallion(BitmapPath, CPoint, int, int);			//若不給重力參數，第一個CPoint不變，但後面變為「x初速, y初速」
		//~CScallion();

		void OnMove();
		void OnShow();
		void SetInitVelocity(int, int, int, int, int = 5);
		void SetInitVelocity(int, int);
		bool IsCollision(CPasserby);
		bool IsCollision(CPasserby*);
		bool IsCollision(CBoss*);
		bool IsCollision(CRect);
		void Clear();
		void Initialize(CPoint);
		int  GetAtk() { return atk; };
		void SetAtk(int k) { atk = k; };

	protected:
		int atk = 1000;
		int velocity_x;
		int velocity_y;
		int gravity;
		int direction;			//丟出時面對的方向
	};
	#pragma endregion

	#pragma region - CBlackHole -
	class CBlackHole : public CScallion
	{
	public:
		CBlackHole();
		CBlackHole(BitmapPath, CPoint, CPoint, int = 4);	//給兩CPoint + int 為參數時，第一個為目前座標，第二個為滑鼠座標，最後為重力參數
		CBlackHole(BitmapPath, CPoint, int, int);			//若不給重力參數，第一個CPoint不變，但後面變為「x初速, y初速」
		CRect GetCenterRect() { return centerRect; };
		void OnMove();
		void SetRole(CRole* _r) { role = _r; };
		int GetX3() { return x + animation.Width() / 2; };
		bool IsDead() { return isDead; };
		void Initialize();
	private:
		CTimer selfBang; //自爆timer
		CRect centerRect;
		CRect centerRectSize;
		CRole *role;
		int csize = 100;
		bool isDead;
	};
	#pragma endregion

	#pragma region - CRay -
	class CRay
	{
	public:
		CRay();
		CRay(BitmapPath, CPoint, double = 0.1); //動畫路徑, 起始位置, 每個影格間隔時間
		~CRay();

		void OnMove();

		bool GetValid() { return animation.GetValid(); };
		void SetValid(bool f) { animation.SetValid(f); };
		CAnimate* GetAnimate() { return &animation; };
		void SetXY(int _x, int _y) {
			x = _x;  y = _y;
			animation.SetTopLeft(x, y);
		}
		bool GetAttackValid() { return AttackValid; };
	private:
		CAnimate animation;
		int x, y;
		bool AttackValid; //攻擊有效
	};
	#pragma endregion
}