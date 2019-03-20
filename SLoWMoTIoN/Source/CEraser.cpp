#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"
#include <sstream>
#include "CLibrary.h"
using namespace myLibrary;
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEraser: Eraser class
	/////////////////////////////////////////////////////////////////////////////

	CEraser::CEraser()
	{
		Initialize();
	}

	CEraser::~CEraser()
	{
		animation.ReleaseAnimate();
	}

	void CEraser::Initialize()
	{
		const int X_POS = 0;
		const int Y_POS = 320;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
		canMoving = true;

		layer.SetLayer(8);
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

	int CEraser::GetX3()
	{
		return x + animation.Width() / 2;
	}

	int CEraser::Height()
	{
		return height;
	}

	int CEraser::Width()
	{
		return width;
	}

	bool CEraser::GetMovingLeft()
	{
		return isMovingLeft;
	}
	
	bool CEraser::GetMovingRight()
	{
		return isMovingRight;
	}

	void CEraser::LoadBitmap()
	{
		vector<char*> role = { ".\\Role\\IDB_ROLE_0.bmp", ".\\Role\\IDB_ROLE_1.bmp", ".\\Role\\IDB_ROLE_2.bmp",
			".\\Role\\IDB_ROLE_3.bmp", ".\\Role\\MIKU_4.bmp", ".\\Role\\MIKU_5.bmp",
			".\\Role\\MIKU_6.bmp", ".\\Role\\IDB_ROLE_7.bmp", ".\\Role\\IDB_ROLE_8.bmp",
			".\\Role\\IDB_ROLE_9.bmp", ".\\Role\\MIKU_10.bmp", ".\\Role\\MIKU_11.bmp",
			".\\Role\\MIKU_12.bmp" };

		animation.LoadBitmap(role, RGB(255, 255, 255));
	}

	void CEraser::LoadBitmap(string ziliaojia, string name, int number)
	{
		vector<char*> addresses;
		for (int i = 0; i < number; i++)
		{
			addresses.push_back(ConvertCharPointToString(ziliaojia, name, i));
		}
		animation.LoadBitmap(addresses, RGB(255, 255, 255));
		DeleteCharPoint(addresses);
		/*for (vector<char*>::iterator it = addresses.begin(); it != addresses.end(); it++)
		{
			if (NULL != *it)
			{
				delete *it;
				*it = NULL;
			}
		}		
		addresses.clear();*/
		

		height = animation.Height();
		width = animation.Width();
	}

	void CEraser::OnMove()
	{
		const int STEP_SIZE = 20;
		int dir = -1;			

		if (isMovingUp)
		{
			dir = 0;
		}
		if (isMovingRight)
		{
			dir = 1;
			if (canMoving)
				x += STEP_SIZE;
		}
		if (isMovingDown)
		{
			dir = 2;
			if (canMoving)
				y += STEP_SIZE;
		}
		if (isMovingLeft)
		{
			dir = 3;
			if (canMoving)
				x -= STEP_SIZE;
		}

		animation.OnMove(dir);
	}

	void CEraser::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
		canMoving = flag | isMovingLeft | isMovingRight | isMovingUp;
	}

	void CEraser::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
		canMoving = flag | isMovingDown | isMovingRight | isMovingUp;
	}

	void CEraser::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
		canMoving = flag | isMovingDown | isMovingLeft | isMovingUp;
	}

	void CEraser::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
		canMoving = flag | isMovingLeft | isMovingRight | isMovingDown;
	}

	void CEraser::SetCanMoving(bool _canMoving)
	{
		canMoving = _canMoving;
	}

	bool CEraser::GetCanMoving()
	{
		return canMoving;
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

	CAnimate* CEraser::GetAnimate()
	{
		animation.SetTopLeft(x, y);
		return &animation;
	}

	#pragma region - CRole -
	CRole::CRole() : CEraser()
	{
		isJumping = false;
		canJumping = true;
		const int INIT_VELOCITY = 40;				//設定初速度
		const int GRAVITY = 4;						//設定重力
		init_velocity = velocity = INIT_VELOCITY;
		gravity = GRAVITY;
	}
	CRole::~CRole()
	{

	}

	void CRole::OnMove()
	{
		const int STEP_SIZE = 20;
		int dir = -1;

		if (isMovingUp)
		{
			dir = 0;
			if (canMoving && canJumping)
			{
				isJumping = true;
				//y -= STEP_SIZE;
			}
		}
		if (isMovingRight)
		{
			dir = 1;
			if (canMoving)
				x += STEP_SIZE;
		}
		if (isMovingDown)
		{
			dir = 2;
			if (canMoving)
				y += STEP_SIZE;
		}
		if (isMovingLeft)
		{
			dir = 3;
			if (canMoving)
				x -= STEP_SIZE;
		}
		if (isJumping)
		{
			if (canJumping)
			{
				velocity = init_velocity;
			}
			else
			{
				//jump
				if (velocity > 0)
				{
					velocity -= gravity;
					y -= velocity;
				}
				else
				{
					velocity -= gravity;
					y -= velocity;
				}
			}
		}

		animation.OnMove(dir);
	}

	bool CRole::GetMovingJump()
	{
		return isJumping;
	}

	void CRole::SetMovingJump(bool flag)
	{
		isJumping = flag;
	}

	bool CRole::GetCanJumping()
	{
		return canJumping;
	}

	void CRole::SetCanJumping(bool flag)
	{
		canJumping = flag;
	}
	#pragma endregion

	#pragma region - CNPC -
	CNPC::CNPC() : CEraser()
	{
		InitX = 0;
		x = InitX;
		layer.SetLayer(6);

		//LoadBitmap("Role", "LUKA", 2);
	}
	CNPC::CNPC(int _x, string ziliaojia, string name, int number) : CEraser()
	{
		InitX = _x;
		x = InitX;
		layer.SetLayer(6);

		//LoadBitmap(ziliaojia, name, number);
	}
	void CNPC::SetXY(int _x, int _y)
	{
		CEraser::SetXY(_x, _y);
		animation.SetTopLeft(x, y);
	}
	CNPC::~CNPC()
	{

	}
	#pragma endregion
}