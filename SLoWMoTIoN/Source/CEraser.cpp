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

	void CEraser::LoadBitmap(string ziliaojia, string name, int number, COLORREF color)
	{
		for (int i = 0; i < number; i++)
		{
			char* address = ConvertCharPointToString(ziliaojia, name, i);
			animation.AddBitmap(address, color);
			delete address;
		}
		
		animation.SetTopLeft(x, y);
		height = animation.Height();
		width = animation.Width();
	}

	void CEraser::OnMove()
	{
		const int STEP_SIZE = move_distance;
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

	void CEraser::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
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

	bool CEraser::GetValid()
	{
		return animation.GetValid();
	}

	void CEraser::SetValid(bool flag)
	{
		animation.SetValid(flag);
	}

	void CEraser::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	CAnimate* CEraser::GetAnimate()
	{
		animation.SetTopLeft(x, y);
		return &animation;
	}
	#pragma endregion

	#pragma region - CRole -
	CRole::CRole() : CEraser()
	{
		//Initialize();		
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
				CAudio::Instance()->Play(AUDIO_JUMP);
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

		animation.SetTopLeft(x, y);

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
		if (shoot_cd.IsTimeOut())
		{
			CAudio::Instance()->Play(AUDIO_THROW);
			CScallion *newCScallion = new CScallion("Role", "scallion", 4, GetX3(), GetY1(), mx, my); //先創建一個蔥的物件
			scallion.push_back(newCScallion); //將蔥放進vector
			shoot_cd.ResetTime(0.33);
		}
	}

	void CRole::AddScore(int _score)
	{
		score += _score;
	}

	void CRole::Initialize(unsigned _AUDIO_THROW, unsigned _AUDIO_JUMP)
	{
		CEraser::Initialize();
		isJumping = true;
		canJumping = false;
		isFire = false;
		const int INIT_VELOCITY = 40;				//設定初速度
		const int GRAVITY = 4;						//設定重力
		init_velocity = velocity = INIT_VELOCITY;
		gravity = GRAVITY;
		mouse_x, mouse_y = 0;
		x = 0;
		y = -Height();
		score = 0;
		shoot_cd = CTimer(0);						//初始化射擊冷卻時間
		AUDIO_THROW = _AUDIO_THROW;
		AUDIO_JUMP = _AUDIO_JUMP;

		SetValid(true);

		if (animation.IsNull())
		{
			LoadBitmap("Role", "MIKU", 13, RGB(255, 255, 255));
		}
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
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
		moveTimer = CTimer(0.5);
		stopTimer = CTimer(1.5);
		SetValid(false);
		recreateTimer = CTimer((double)GetRandom(1, 4) / 2.0);
		//animation.ResetWaitTime((double)GetRandom(2, 4) / 5.0);
		//LoadBitmap("Role", "LUKA", 2);
	}

	CNPC::CNPC(int _x, int _y, string ziliaojia, string name, int number, int _score) : CEraser()
	{
		initX = _x;
		x = initX;
		y = _y;
		layer.SetLayer(6);
		LoadBitmap(ziliaojia, name, number, RGB(255, 255, 255));
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());

		score = _score;
		moveTimer = CTimer(0.5);
		stopTimer = CTimer(1.5);
		SetValid(false);
		recreateTimer = CTimer((double)GetRandom(1, 4) / 2.0);
	}

	void CNPC::SetXY(int _x, int _y)
	{
		CEraser::SetXY(_x, _y);
		animation.SetTopLeft(x, y);
	}

	void CNPC::SetScore(int _score)
	{
		score = _score;
	}

	void CNPC::SetMoving()
	{
		if (GetValid() == false /*&& !recreateTimer.IsTimeOut()*/)
		{
			recreateTimer.CountDown();
			if (recreateTimer.IsTimeOut())
			{
				SetValid(true);
				//CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
			}
			else
			{
				return;
			}
		}
		if ( !moveTimer.IsTimeOut())
		{
			moveTimer.CountDown();
		}
		else
		{
			SetCanMoving(false);
			
			stopTimer.CountDown();
			if (stopTimer.IsTimeOut())
			{
				int randomNumber = GetRandom(1, 10);
				if (randomNumber % 2 == 0)
				{
					SetMovingLeft(true);
					SetMovingRight(false);
				}
				else
				{
					SetMovingRight(true);
					SetMovingLeft(false);
				}
				double resetStopTime = GetRandom(1, 10) / 10;
				SetCanMoving(true);
				moveTimer.ResetTime(0.1);
				stopTimer.ResetTime(resetStopTime);
			}
		}
		animation.SetTopLeft(x, y);
	}

	void CNPC::OnMove()
	{
		const int STEP_SIZE = move_distance;

		if (isMovingUp)
		{
			
		}
		if (isMovingRight)
		{
			if (canMoving)
				x += STEP_SIZE;
		}
		if (isMovingDown)
		{
			if (canMoving)
				y += STEP_SIZE;
		}
		if (isMovingLeft)
		{
			if (canMoving)
				x -= STEP_SIZE;
		}
		animation.SetTopLeft(x, y);
		animation.OnMove();
	}

	CNPC::~CNPC()
	{
		
	}
	#pragma endregion
}