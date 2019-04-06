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
		SetXY(0, 0);
		hp = 0;
	}

	CBoss::CBoss(int _x, int _y, int _hp, BitmapPath loadPath, COLORREF color)
	{
		SetXY(_x, _y);
		hp = _hp;
		LoadBitmap(loadPath, color);
	}

	CBoss::~CBoss()
	{
	}

	void CBoss::LoadBitmap(BitmapPath loadPath, COLORREF color)
	{
		for (int i = 0; i < loadPath.number; i++)
		{
			char* address = ConvertCharPointToString(loadPath.ziliaojia, loadPath.name, i);
			animation.AddBitmap(address, color);
			delete address;
		}

		animation.SetTopLeft(x, y);
		height = animation.Height();
		width = animation.Width();
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
}