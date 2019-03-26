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
#include "CLibrary.h"
#include "CManager.h"

using namespace myLibrary;
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEraser: Eraser class
	/////////////////////////////////////////////////////////////////////////////

	#pragma region - CEraser
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
		const int Y_POS = -160;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
		canMoving = true;

		layer.SetLayer(8);
		score = 0;
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

	int CEraser::GetScore()
	{
		return score;
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
		for (int i = 0; i < number; i++)
		{
			char* address = ConvertCharPointToString(ziliaojia, name, i);
			animation.AddBitmap(address, RGB(255, 255, 255));
			delete address;
		}

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
		animation.SetTopLeft(x, y);
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

	CAnimate* CEraser::GetAnimate()
	{
		return &animation;
	}
	#pragma endregion

	#pragma region - CRole -
	CRole::CRole() : CEraser()
	{
		isJumping = true;
		canJumping = false;
		isFire = false;
		const int INIT_VELOCITY = 40;				//設定初速度
		const int GRAVITY = 4;						//設定重力
		init_velocity = velocity = INIT_VELOCITY;
		gravity = GRAVITY;
		mouse_x, mouse_y = 0;

		shoot_cd = CTimer(0);						//初始化射擊冷卻時間
		//last_right_left = 'r';

		
	}
	CRole::~CRole()
	{
		for (vector<CScallion*>::iterator it = scallion.begin(); it != scallion.end(); it++)
		{
			delete (*it);
			(*it) = NULL;
		}
		vector<CScallion*>().swap(scallion);
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
			last_right_left = 'r';
			if (canMoving)
				x += STEP_SIZE;
		}
		if (isMovingLeft)
		{
			dir = 3;
			last_right_left = 'l';
			if (canMoving)
				x -= STEP_SIZE;
		}
		if (isMovingDown)
		{
			dir = 2;
			if (canMoving)
			y += STEP_SIZE;
			
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
		if (isFire)
		{
			Fire(mouse_x, mouse_y);
		}

		shoot_cd.CountDown();	//設置射擊的 CD時間
		animation.OnMove(dir);
		for (unsigned int i = 0; i < scallion.size(); i++)
		{
			scallion[i]->OnMove();
		}
	}

	void CRole::OnShow()
		{
			animation.SetTopLeft(x, y);
			animation.OnShow();

			for (unsigned int i = 0; i < scallion.size(); i++)
			{
				scallion[i]->OnShow();
			}
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

	void CRole::SetMouseXY(int _x, int _y)
	{
		mouse_x = _x;
		mouse_y = _y;
	}

	bool CRole::GetIsFire()
	{
		return isFire;
	}
	
	void CRole::SetIsFire(bool flag)
	{
		isFire = flag;
	}

	void CRole::Fire(int mx, int my)
	{		
		//ClearScallion();
		if (shoot_cd.IsTimeOut())
		{
			scallion.push_back(new CScallion("Role", "scallion", 4, GetX3(), GetY1(), mx, my));
			shoot_cd.ResetTime(0.33);
		}
	}

	void CRole::AddScore(int _score)
	{
		score += _score;
	}

	vector<CScallion*>* CRole::GetScallion()
	{
		return &scallion;
	}


	#pragma endregion

	#pragma region - CNPC -
	CNPC::CNPC() : CEraser()
	{
		initX = 0;
		x = initX;
		y = 300;
		layer.SetLayer(6);
		score = 0;
		isValid = true;
		//LoadBitmap("Role", "LUKA", 2);
	}

	CNPC::CNPC(int _x, int _y, string ziliaojia, string name, int number, int _score) : CEraser()
	{
		initX = _x;
		x = initX;
		y = _y;
		layer.SetLayer(6);
		score = _score;
		isValid = true;
		//LoadBitmap(ziliaojia, name, number);
	}

	void CNPC::SetXY(int _x, int _y)
	{
		CEraser::SetXY(_x, _y);
		animation.SetTopLeft(x, y);
	}

	bool CNPC::IsValid()
	{
		return isValid;
	}

	void CNPC::SetValid(bool flag)
	{
		isValid = flag;
	}

	void CNPC::SetScore(int _score)
	{
		score = _score;
	}

	CNPC::~CNPC()
	{
		
	}
	#pragma endregion
}