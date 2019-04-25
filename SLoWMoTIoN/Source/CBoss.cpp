#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"
#include "CBall.h"
#include <sstream>
#include "CManager.h"
#include "CLibrary.h"
#include "CBoss.h"
#include <vector>
using namespace myLibrary;

namespace game_framework
{
	#pragma region - CBoss -
	CBoss::CBoss()
	{
		initHp = 0;
		initx = 0;
		inity = 0;
	}

	CBoss::CBoss(int _x, int _y, int _hp, BitmapPath _loadPath, COLORREF color)
	{
		initx = _x;
		inity = _y;
		initHp = _hp;
		loadPath = _loadPath;
		transparentColor = color;

		//Initialize();
	}

	CBoss::~CBoss()
	{
	}

	void CBoss::LoadBitmap()
	{
		//for (int i = 0; i < loadPath.number; i++)
		//{
		//	char* address = ConvertCharPointToString(loadPath.ziliaojia, loadPath.name, i);
		//	animation.AddBitmap(address, transparentColor);
		//	delete address;
		//}

		animation.LoadBitmap(loadPath.ziliaojia, loadPath.name, loadPath.number, transparentColor);

		animation.SetTopLeft(x, y);
		height = animation.Height();
		width = animation.Width();
	}

	void CBoss::Initialize()
	{
		#pragma region - 僅一次的載入圖片 -
		if (animation.IsNull())
		{
			LoadBitmap();
		}
		#pragma endregion

		inity = SIZE_Y - animation.Height();
		SetCurrentXY(initx, inity);
		SetXY(initx, inity);
		hp = initHp;
		layer.SetLayer(BOSS_LAYER);
	}

	void CBoss::SetXY(int _x, int _y)
	{
		x = _x;
		y = _y;
		animation.SetTopLeft(x, y);
	}
	void CBoss::SetCurrentXY(int _x, int _y)
	{
		currentX = _x;
		currentY = _y;

		/*int dx = CCamera::Instance()->GetX();
		SetXY(currentX - dx, y);
		animation.OnMove();*/
	}
	void CBoss::SetHp(int _hp)
	{
		hp = _hp;
	}
	void CBoss::SetIsAlive(bool flag)
	{
		IsAlive = flag;
	}
	void CBoss::MoveWithMap(string dir)
	{

		if (dir == "left")
			x += MOVE_DISTANCE;
		if (dir == "right")
			x -= MOVE_DISTANCE;

		SetXY(x, y);
	}

	void CBoss::OnMove()
	{
		int dx = CCamera::Instance()->GetX();
		SetXY(currentX - dx, y);
		animation.OnMove();
	}

	CAnimate * CBoss::GetAnimate()
	{
		return &animation;
	}
	#pragma endregion

	#pragma region - CXingting -
	CXingting::CXingting()
	{
		CBoss();
	}

	CXingting::CXingting(int _x, int _y, int _hp, BitmapPath _loadPath, COLORREF color) : CBoss(_x, _y, _hp, _loadPath, color)
	{
		
	}

	CXingting::~CXingting()
	{
		ClearBullet();
	}

	void CXingting::Initialize()
	{
		CBoss::Initialize();

		#pragma region - delete bullet -
		for (vector<CScallion*>::iterator level4iter = level4.begin(); level4iter != level4.end(); level4iter++)
		{
			delete *level4iter;
		}
		level4.clear();
		#pragma endregion

		goal_x = 320 - width / 2;
		goal_y = 0;

		shootLevel4_cd = CTimer(0.75);
		shoot_atk2_cd = CTimer(0.01);
		moveToGoal = CTimer(0.04);

		mode_Attack1 = true;
		mode_Attack2 = false;

		atkCounter = 0;
	}

	void CXingting::OnCycle(CRole *role)
	{
		OnMove();
		Attack(role);
	}

	void CXingting::OnMove()
	{
		if (mode_Attack1)
		{
			int move_dx = (initx - goal_x) / 20;
			int move_dy = (inity - goal_y) / 20;
			moveToGoal.CountDown();
			if (moveToGoal.IsTimeOut())
			{
				SetCurrentXY(currentX - move_dx, currentY - move_dy);
				moveToGoal.ResetTime();
			}
			if (currentX <= goal_x || currentY <= goal_y)
			{
				mode_Attack1 = false;
				mode_Attack2 = true;
				atkCounter = 0;
			}
		}

		int dx = CCamera::Instance()->GetX();
		SetXY(currentX - dx, currentY);
		animation.OnMove();
	}

	void CXingting::Attack(CRole *role)
	{
		if (mode_Attack1)
		{
			Attack1();
		}
		if (mode_Attack2)
		{
			Attack2();
		}
		Level4Collision(role);
	}

	//void CXingting::Attack1()
	//{
	//	shootLevel4_cd.CountDown();
	//	if (shootLevel4_cd.IsTimeOut())
	//	{
	//		for (int i = 0; i < 5; i++)
	//		{
	//			//CScallion *newlevel4 = new CScallion("Role\\books", "book", 4, 450, 360, 150 + i * 30, 360 - i * 75); //先創建一個蔥的物件
	//			level4.push_back(new CScallion(BitmapPath("RES\\Object\\books", "book", 4), CPoint(450, 360), CPoint(150 + i * 30, 360 - i * 75), 0)); //將蔥放進vector
	//			shootLevel4_cd.ResetTime();
	//		}
	//	}
	//}

	void CXingting::Attack1()
	{
		shootLevel4_cd.CountDown();
		if (shootLevel4_cd.IsTimeOut())
		{
			for (int i = 0; i < 12; i++)
			{
				double speed = 15.0;
				int angle = (-150 + atkCounter * 15) + i * 30;
				double mx = sin(angle * (PI / 180.0)) * speed;
				double my = cos(angle * (PI / 180.0)) * speed;
				int drs = 3; //亂取的名字 用意在給一個倍數讓books不從中心點出現
				CPoint center = GetCenterPoint() + CPoint((int)mx * drs, -(int)my * drs);
				CScallion *newlevel4 = new CScallion(BitmapPath("RES\\Object\\books", "book", 4), center, CPoint(0, 0), 0); //先創建一個蔥的物件
				newlevel4->SetInitVelocity((int)mx, (int)my);
				level4.push_back(newlevel4); //將蔥放進vector
				shootLevel4_cd.ResetTime();
			}
			atkCounter++;
		}
	}

	void CXingting::Attack2()
	{
		shoot_atk2_cd.CountDown();
		if (shoot_atk2_cd.IsTimeOut())
		{
			#pragma region - bullet parameter -
			double speed = 15.0;
			int angle = (-150 + atkCounter * 15) + i * 30;
			double mx = sin(angle * (PI / 180.0)) * speed;
			double my = cos(angle * (PI / 180.0)) * speed;
			int drs = 3; //亂取的名字 用意在給一個倍數讓books不從中心點出現
			CPoint center = GetCenterPoint() + CPoint((int)mx * drs, -(int)my * drs);
			#pragma endregion

			
			CScallion *newlevel4 = new CScallion(BitmapPath("RES\\Object\\books", "book", 4), center, CPoint(0, 0), 0); //先創建一個蔥的物件
			newlevel4->SetInitVelocity((int)mx, (int)my);
			level4.push_back(newlevel4); //將蔥放進vector
			shootLevel4_cd.ResetTime();
			atkCounter++;
		}
	}

	void CXingting::ClearBullet()
	{
		for (vector<CScallion*>::iterator level4iter = level4.begin(); level4iter != level4.end(); level4iter++)
		{
			delete *level4iter;
			*level4iter = NULL;
		}
		level4.clear();
	}

	void CXingting::Level4Collision(CRole *role)
	{
		for (vector<CScallion*>::iterator level4iter = level4.begin(); level4iter != level4.end();)
		{
			(*level4iter)->OnMove();
			if (role->IsCollisionLevel4(*level4iter))
			{
				(*level4iter)->SetIsAlive(false);
				role->SubHp();
				delete *level4iter;
				*level4iter = NULL;
				level4iter = level4.erase(level4iter);
			}
			else
			{
				level4iter++;
			}
		}

	}
	
	#pragma endregion

}