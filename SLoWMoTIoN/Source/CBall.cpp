#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CBall.h"
#include "CEraser.h"
#include "CManager.h"


using namespace myLibrary;

namespace game_framework {

	#pragma region - ball -
	/////////////////////////////////////////////////////////////////////////////
	// CBall: Ball class
	/////////////////////////////////////////////////////////////////////////////

	CBall::CBall()
	{
		is_alive = true;
		x = y = dx = dy = index = delay_counter = 0;
	}

	//bool CBall::HitEraser(CEraser *eraser)
	//{
	//	// 檢測擦子所構成的矩形是否碰到球
	//	return HitRectangle(eraser->GetX1(), eraser->GetY1(),
	//		eraser->GetX2(), eraser->GetY2());
	//}

	//bool CBall::HitRectangle(int tx1, int ty1, int tx2, int ty2)
	//{
	//	int x1 = x + dx;				// 球的左上角x座標
	//	int y1 = y + dy;				// 球的左上角y座標
	//	int x2 = x1 + bmp.Width();	// 球的右下角x座標
	//	int y2 = y1 + bmp.Height();	// 球的右下角y座標
	//								//
	//								// 檢測球的矩形與參數矩形是否有交集
	//								//
	//	return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
	//}

	bool CBall::IsAlive()
	{
		return is_alive;
	}

	void CBall::LoadBitmap()
	{
		bmp.LoadBitmap(IDB_BALL, RGB(0, 0, 0));			// 載入球的圖形
		bmp_center.LoadBitmap(IDB_CENTER, RGB(0, 0, 0));	// 載入球圓心的圖形
	}

	void CBall::OnMove()
	{
		if (!is_alive)
			return;
		delay_counter--;
		if (delay_counter < 0) {
			delay_counter = delay;
			//
			// 計算球向對於圓心的位移量dx, dy
			//
			const int STEPS = 18;
			static const int DIFFX[] = { 35, 32, 26, 17, 6, -6, -17, -26, -32, -34, -32, -26, -17, -6, 6, 17, 26, 32, };
			static const int DIFFY[] = { 0, 11, 22, 30, 34, 34, 30, 22, 11, 0, -11, -22, -30, -34, -34, -30, -22, -11, };
			index++;
			if (index >= STEPS)
				index = 0;
			dx = DIFFX[index];
			dy = DIFFY[index];
		}
	}

	void CBall::SetDelay(int d)
	{
		delay = d;
	}

	void CBall::SetIsAlive(bool alive)
	{
		is_alive = alive;
	}

	void CBall::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CBall::OnShow()
	{
		if (is_alive) {
			bmp.SetTopLeft(x + dx, y + dy);
			bmp.ShowBitmap();
			bmp_center.SetTopLeft(x, y);
			bmp_center.ShowBitmap();
		}
	}
	#pragma endregion

	CScallion::CScallion()
	{
		dx = dy = index = delay_counter = 0;	//暫時無用，只是確保繼承不會出問題

		const int INIT_X = 0, INIT_Y = 0;
		const int GRAVITY = 4;
		x = INIT_X;
		y = INIT_Y;
		gravity = GRAVITY;
		layer.SetLayer(6);
		animation.SetTopLeft(INIT_X, INIT_Y);

		is_alive = true;
		LoadBitmap("Role", "scallion", 4);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
	}

	CScallion::CScallion(string ziliaojia, string name, int number, int _x, int _y, int f_x, int f_y)
	{
		
		const int INIT_X = _x, INIT_Y = _y;
		const int GRAVITY = 4;
		x = INIT_X;
		y = INIT_Y;
		gravity = GRAVITY;
		layer.SetLayer(6);

		is_alive = true;
		LoadBitmap(ziliaojia, name, number);
		animation.SetTopLeft(x, y);
		animation.ResetDelayTime(0.1);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
		
		SetInitVelocity(_x, _y, f_x, f_y);
	}

	CScallion::~CScallion()
	{
		//animation.ReleaseAnimate();
		//delete this;
	}

	void CScallion::Clear()
	{
		this->~CScallion();
	}

	void CScallion::LoadBitmap(string ziliaojia, string name, int number)
	{
		for (int i = 0; i < number; i++)
		{
			char *address = ConvertCharPointToString(ziliaojia, name, i);
			animation.AddBitmap(address, RGB(255, 255, 255));
			delete address;
		}
	}

	CAnimate * CScallion::GetAnimate()
	{
		return &animation;
	}

	void CScallion::OnMove()
	{
		if (!IsAlive())
			return;

		if (x > SIZE_X || x < 0 ||  y > SIZE_Y )	//超出螢幕
		{
			SetIsAlive(false);
			return;
		}

		#pragma region - 重力計算 -
		if (velocity_y > 0)
		{
			velocity_y -= gravity;
			y -= velocity_y;
		}
		else
		{
			velocity_y -= gravity;
			y -= velocity_y;
		}
		#pragma endregion


		animation.OnMove();
		animation.SetTopLeft(x + velocity_x, y);

		SetXY(x + velocity_x, y);

		
	}

	void CScallion::OnShow()
	{
		if (!IsAlive())
			return;
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}

	void CScallion::SetInitVelocity(int b_x, int b_y, int f_x, int f_y)
	{
		int dx = (f_x - b_x) / 5;
		int dy = -(f_y - b_y) / 5;

		velocity_x = dx;
		velocity_y = dy;
		
	}
	
	bool CScallion::IsCollision(CNPC passerby)
	{
		return false;
		return IsPointInRect(CPoint(x + animation.Width(), y), passerby.animation.GetRect());
	}

	bool CScallion::IsCollision(CNPC* passerby)
	{
		return IsRectCollision(animation.GetRect(), passerby->animation.GetRect());
	}

}