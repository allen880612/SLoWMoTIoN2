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
		//bmp.LoadBitmap(IDB_BALL, RGB(0, 0, 0));			// 載入球的圖形
		//bmp_center.LoadBitmap(IDB_CENTER, RGB(0, 0, 0));	// 載入球圓心的圖形
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
		if (!animation.IsNull())
			animation.SetTopLeft(x, y);
	}

	void CBall::OnShow()
	{
		/*if (is_alive) {
		bmp.SetTopLeft(x + dx, y + dy);
		bmp.ShowBitmap();
		bmp_center.SetTopLeft(x, y);
		bmp_center.ShowBitmap();
		}*/
	}
	void CBall::SetCurrentXY(double cx, double cy)
	{
		currentX = cx;
		currentY = cy;

		int dx = CCamera::Instance()->GetX();
		int nx = (int)(currentX) - dx;
		int ny = (int)(currentY);
		SetXY(nx, ny);
	}

	void CBall::LoadBitmap(string ziliaojia, string name, int number)
	{
		/*for (int i = 0; i < number; i++)
		{
			char *address = ConvertCharPointToString(ziliaojia, name, i);
			animation.AddBitmap(address, RGB(255, 255, 255));
			delete address;
		}*/
		animation.LoadBitmap(ziliaojia, name, number);
	}

	void CBall::LoadBitmap(BitmapPath loadpath)
	{
		animation.LoadBitmap(loadpath.ziliaojia, loadpath.name, loadpath.number, loadpath.color);
	}


	CAnimate * CBall::GetAnimate()
	{
		return &animation;
	}

	#pragma endregion

	#pragma region - CScallion -
	CScallion::CScallion()
	{
		dx = dy = index = delay_counter = 0;	//暫時無用，只是確保繼承不會出問題

		const int INIT_X = 0, INIT_Y = 0;
		const int GRAVITY = 4;
		currentX = INIT_X;
		currentY = INIT_Y;
		x = INIT_X;
		y = INIT_Y;
		gravity = GRAVITY;
		layer.SetLayer(BULLET_LAYER);
		animation.SetTopLeft(INIT_X, INIT_Y);

		is_alive = true;
		LoadBitmap("Role", "scallion", 4);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
	}

	CScallion::CScallion(BitmapPath loadpath, CPoint point, CPoint finalPoint, int _gravity) //gravity 預設 4 可不指定
	{
		gravity = _gravity;
		LoadBitmap(loadpath);
		Initialize(point);

		SetInitVelocity(point.x, point.y, finalPoint.x, finalPoint.y);
	}

	CScallion::CScallion(BitmapPath loadpath, CPoint point, int initV_x, int initV_y)
	{
		gravity = 0;
		LoadBitmap(loadpath);
		Initialize(point);
		velocity_x = initV_x;
		velocity_y = initV_y;
	}

	//CScallion::~CScallion()
	//{
	//	//animation.ReleaseAnimate();
	//	//delete this;
	//}

	void CScallion::Clear()
	{
		this->~CScallion();
	}

	void CScallion::Initialize(CPoint initPos)
	{
		const int INIT_X = initPos.x;
		const int INIT_Y = initPos.y;

		currentX = x = INIT_X;
		currentY = y = INIT_Y;
		
		is_alive = true;
		animation.SetTopLeft(x, y);
		animation.ResetDelayTime(0.1);

		layer.SetLayer(BULLET_LAYER);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
	}

	void CScallion::OnMove()
	{
		if (!IsAlive())
			return;

		if (x > SIZE_X || x < 0 || y > SIZE_Y || y < -300)	//超出螢幕
		{
			SetIsAlive(false);
			animation.SetValid(false);
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

		SetXY(x + velocity_x, y);
		//SetCurrentXY(currentX + velocity_x, currentY);

	}

	void CScallion::OnShow()
	{
		if (!IsAlive())
			return;
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}

	void CScallion::SetInitVelocity(int vx, int vy)
	{
		velocity_x = vx;
		velocity_y = vy;
	}

	void CScallion::SetInitVelocity(int b_x, int b_y, int f_x, int f_y, int fix_velocity)
	{
		int dx = (f_x - b_x) / fix_velocity / 2;
		int dy = -(f_y - b_y) / fix_velocity / 2;

		velocity_x = dx;
		velocity_y = dy;
	}



	bool CScallion::IsCollision(CPasserby passerby)
	{
		return false;
		return IsPointInRect(CPoint(x + animation.Width(), y), passerby.animation.GetRect());
	}

	bool CScallion::IsCollision(CPasserby* passerby)
	{
		return IsRectCollision(animation.GetRect(), passerby->animation.GetRect());
	}
	bool CScallion::IsCollision(CBoss *boss)
	{
		return IsRectCollision(animation.GetRect(), boss->GetAnimate()->GetRect());
	}
	bool CScallion::IsCollision(CRect rect)
	{
		return IsRectCollision(animation.GetRect(), rect);
	}
	#pragma endregion

	//#pragma region - CBullet -
	//CBullet::CBullet()
	//{
	//}

	//CBullet::CBullet(BitmapPath _loadpath, int _x, int _y, int _mx, int _my)
	//{
	//	currentX = x = _x;
	//	currentY = y = _y;

	//	layer.SetLayer(BULLET_LAYER);

	//	LoadBitmap(_loadpath.ziliaojia, _loadpath.name, _loadpath.number);

	//	animation.SetTopLeft(x, y);
	//	animation.ResetDelayTime(0.1);
	//	CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
	//}
	//
	//CBullet::~CBullet()
	//{
	//}

	//void CBullet::OnMove()
	//{
	//	if (x > SIZE_X || x < 0 || y > SIZE_Y)	//超出螢幕
	//	{
	//		SetIsAlive(false);
	//		animation.SetValid(false);
	//		return;
	//	}

	//	animation.OnMove();

	//	SetXY(x + dx, y + dy);
	//}
	//#pragma endregion

	#pragma region -  -
	CBlackHole::CBlackHole() : CScallion()
	{
	}

	CBlackHole::CBlackHole(BitmapPath loadpath, CPoint point, CPoint finalPoint, int _gravity) : CScallion(loadpath, point, finalPoint, _gravity)
	{
		SetInitVelocity(point.x, point.y, finalPoint.x, finalPoint.y, 25);
		Initialize();
	}

	CBlackHole::CBlackHole(BitmapPath loadpath, CPoint point, int initV_x, int initV_y) : CScallion(loadpath, point, initV_x, initV_y)
	{
		Initialize();
	}

	void CBlackHole::Initialize()
	{
		centerRectSize = CRect(0, 0, csize, csize);
		role = NULL;
		selfBang = CTimer(1.8);
		isDead = false;
	}

	void CBlackHole::OnMove()
	{
		selfBang.CountDown();
		if (selfBang.IsTimeOut())
		{
			isDead = true;
			SetIsAlive(false);
			role = NULL;
			return;
		}

		if (y + animation.Height() < SIZE_Y)
		{
			if (!IsAlive())
				return;

			if (x > SIZE_X || x + animation.Width() + 87 < 0 || y > SIZE_Y || y < -300)	//超出螢幕
			{
				SetIsAlive(false);
				animation.SetValid(false);
				return;
			}

			#pragma region - 重力計算 -
			if (velocity_y > 0)
			{
				velocity_y -= gravity;
				currentY -= velocity_y;
			}
			else
			{
				velocity_y -= gravity;
				currentY -= velocity_y;
			}
			#pragma endregion

			animation.OnMove();
			SetCurrentXY(currentX + velocity_x, currentY);
		}
		animation.OnMove();

		int centerRectWidth = centerRectSize.right;
		int centerRectHeight = centerRectSize.bottom;
		int centerPointX = x + animation.Width() / 2;
		int centerPointY = y + animation.Height() / 2;

		centerRect = CRect(centerPointX - centerRectWidth / 2,   //left
			               centerPointY - centerRectHeight / 2,  //top
			               centerPointX + centerRectWidth / 2,   //right
			               centerPointY + centerRectHeight / 2); //bottom

		if (role != NULL)
		{
			int ddrsx = (role->GetX3() - centerPointX) / 8;
			role->SetXY(role->GetX1() - ddrsx, role->GetY1());
		}
	}


	#pragma endregion

	#pragma region - CRay -
	CRay::CRay()
	{
	}

	CRay::CRay(BitmapPath loadPath, CPoint Fpoint, double resetTime)
	{
		animation.LoadBitmap(loadPath.ziliaojia, loadPath.name, loadPath.number, loadPath.color);
		SetValid(false);
		SetXY(Fpoint.x, Fpoint.y);
		AttackValid = false;
		animation.ResetDelayTime(resetTime);
		CLayerManager::Instance()->AddObject(&animation, 6);
	}

	CRay::~CRay()
	{
	}

	void CRay::OnMove()
	{
		animation.SetValid(true);
		if (animation.GetIndex() < animation.GetIndexSize() - 1)
		{
			animation.OnMove();
		}
		else
		{
			AttackValid = true;
		}
	}
	#pragma endregion
}