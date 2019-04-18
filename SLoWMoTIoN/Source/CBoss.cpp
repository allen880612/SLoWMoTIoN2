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

		shootLevel4_cd = CTimer(1.0);
	}

	void CXingting::Attack1()
	{
		shootLevel4_cd.CountDown();
		if (shootLevel4_cd.IsTimeOut())
		{
			for (int i = 0; i < 5; i++)
			{
				CScallion *newlevel4 = new CScallion("Role\\books", "book", 4, 450, 360, 150 + i * 30, 360 - i * 75); //先創建一個蔥的物件
				level4.push_back(newlevel4); //將蔥放進vector
				shootLevel4_cd.ResetTime();
			}
		}

		for (vector<CScallion*>::iterator level4iter = level4.begin(); level4iter != level4.end(); level4iter++)
		{
			(*level4iter)->OnMove();
		}
	}
	#pragma endregion

}