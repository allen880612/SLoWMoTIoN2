#pragma once
#include "CLibrary.h"
#include "CEraser.h"

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
		void SetCurrentXY(int, int);
		void SetXY(int nx, int ny);								// 設定圓心的座標
		void SetIsAlive(bool alive);							// 設定是否活著
		void SetDelay(int d);									// 設定旋轉的速度

		void LoadBitmap(string, string, int);
		CAnimate* GetAnimate();
		CLayer layer;
	protected:
		CAnimate animation;
		int currentX, currentY;     // /*當前的座標*/ //
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

	class CScallion : public CBall
	{
	public:
		CScallion();
		CScallion(string, string, int, int, int, int, int);
		~CScallion();

		void OnMove();
		void OnShow();
		void SetInitVelocity(int, int, int, int);
		bool IsCollision(CPasserby);
		bool IsCollision(CPasserby*);
		void Clear();

	protected:

		int velocity_x;
		int velocity_y;
		int gravity;
		int direction;			//丟出時面對的方向
	};
}