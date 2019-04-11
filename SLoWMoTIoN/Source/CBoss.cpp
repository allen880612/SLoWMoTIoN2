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
using namespace myLibrary;

namespace game_framework
{
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
	}

	CBoss::~CBoss()
	{
	}

	void CBoss::LoadBitmap()
	{
		for (int i = 0; i < loadPath.number; i++)
		{
			char* address = ConvertCharPointToString(loadPath.ziliaojia, loadPath.name, i);
			animation.AddBitmap(address, transparentColor);
			delete address;
		}

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
	CAnimate * CBoss::GetAnimate()
	{
		return &animation;
	}
}