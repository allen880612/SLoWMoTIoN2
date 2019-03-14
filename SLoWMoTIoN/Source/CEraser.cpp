#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEraser: Eraser class
	/////////////////////////////////////////////////////////////////////////////

	CEraser::CEraser()
	{
		Initialize();
	}

	int CEraser::GetX1()
	{
		return x;
	}

	int CEraser::GetY1()
	{
		return y;
	}

	int CEraser::GetX2()
	{
		return x + animation.Width();
	}

	int CEraser::GetY2()
	{
		return y + animation.Height();
	}

	bool CEraser::GetMovingLeft()
	{
		return isMovingLeft;
	}

	bool CEraser::GetMovingRight()
	{
		return isMovingRight;
	}

	int CEraser::GetLayer()
	{
		return layer;
	}

	void CEraser::SetLayer(int _layer)
	{
		if (_layer >= 10)
			_layer = 9;
		else if (_layer < 0)
			_layer = 0;

		layer = _layer;
	}

	void CEraser::Initialize()
	{
		const int X_POS = 280;
		const int Y_POS = 400;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
		layer = 8;
	}

	void CEraser::LoadBitmap()
	{
		char *role[13] = { ".\\Role\\IDB_ROLE_0.bmp", ".\\Role\\IDB_ROLE_1.bmp", ".\\Role\\IDB_ROLE_2.bmp",
			".\\Role\\IDB_ROLE_3.bmp", ".\\Role\\MIKU_4.bmp", ".\\Role\\MIKU_5.bmp",
			".\\Role\\MIKU_6.bmp", ".\\Role\\IDB_ROLE_7.bmp", ".\\Role\\IDB_ROLE_8.bmp",
			".\\Role\\IDB_ROLE_9.bmp", ".\\Role\\MIKU_10.bmp", ".\\Role\\MIKU_11.bmp",
			".\\Role\\MIKU_12.bmp" };

		for (int i = 0; i < 13; i++)
		{
			animation.AddBitmap(role[i], RGB(255, 255, 255));
		}
	}

	void CEraser::OnMove()
	{
		const int STEP_SIZE = 20;
		int dir = -1;			//¦s¨ú

		if (isMovingUp)
		{
			dir = 0;
			y -= STEP_SIZE;
		}
		if (isMovingRight)
		{
			dir = 1;
			x += STEP_SIZE;
		}
		if (isMovingDown)
		{
			dir = 2;
			y += STEP_SIZE;
		}
		if (isMovingLeft)
		{
			dir = 3;
			x -= STEP_SIZE;
		}
		animation.onMove(dir);
	}

	void CEraser::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CEraser::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CEraser::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CEraser::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CEraser::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void CEraser::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}

	CAnimation* CEraser::GetAnimation()
	{
		animation.SetTopLeft(x, y);
		return &animation;
	}
}