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
#include "CBoss.h"
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

		layer.SetLayer(5);
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

	int CEraser::GetY3()
	{
		return y + animation.Height() / 2;
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
		animation.LoadBitmap(ziliaojia, name, number, color);
		animation.SetTopLeft(x, y);
		height = animation.Height();
		width = animation.Width();
	}

	void CEraser::LoadBitmap(BitmapPath loadpath)
	{
		animation.LoadBitmap(loadpath.ziliaojia, loadpath.name, loadpath.number, loadpath.color);
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
		isLoaded = false;
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
		const int STEP_SIZE = move_distance;
		int dir = 0;

		if (isMovingUp)
		{
			dir = 1;
			if (canJumping)
			{
				CAudio::Instance()->Play("SOUND_JUMP");
				isJumping = true;
				//y -= STEP_SIZE;
			}
		}
		if (isMovingRight)
		{
			dir = 2;
			if (canMoving)
				x += STEP_SIZE;
		}
		if (isMovingDown)
		{
			dir = 3;
			if (canMoving)
				y += STEP_SIZE;
		}
		if (isMovingLeft)
		{
			dir = 4;
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
		if (isFire)
		{
			Fire(mouse_x, mouse_y);
		}

		shoot_cd.CountDown();	//設置射擊的 CD時間

		animation.SetTopLeft(x, y);
		decisionPoint.SetTopLeft(GetX3() - 5, GetY3());

		animation.OnMove(dir);
		
		#pragma region -- Reset collision rect --
		//collisionRect.left = animation.GetRect().left - move_distance;
		//collisionRect.right = animation.GetRect().right + move_distance;
		//collisionRect.top = animation.GetRect().top - move_distance;
		//collisionRect.bottom = animation.GetRect().bottom + move_distance;
		#pragma endregion

		for (unsigned int i = 0; i < scallion.size(); i++)
		{
			scallion[i]->OnMove();
		}
	}

	void CRole::OnShow()
	{
		hp_left.ShowBitmap();
		scoreInteger.ShowBitmap();
		/*animation.SetTopLeft(x, y);
		animation.OnShow();*/
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

	void CRole::Fire(int mouseX, int mouseY)
	{		
		if (shoot_cd.IsTimeOut())
		{
			CAudio::Instance()->Play("SOUND_THROW");
			CScallion *newCScallion = new CScallion(BitmapPath("RES\\Object\\Scallions", "scallion", 4), CPoint(GetX3(), GetY1()), CPoint(mouseX, mouseY)); //先創建一個蔥的物件

			//CScallion *newCScallion = new CScallion("Role\\books", "book", 4, GetX3(), GetY1(), mx, my); //先創建一個蔥的物件
			scallion.push_back(newCScallion); //將蔥放進vector
			shoot_cd.ResetTime(0.33);
		}
	}

	bool CRole::GetDrop()
	{
		return velocity <= 0;
	}

	int CRole::GetVelocity()
	{
		return velocity;
	}

	#pragma region -- Collision --
	bool CRole::IsCollisionBoss(CBoss *boss)
	{
		ResetCollisionRect();

		return IsRectCollision(collisionRect, boss->GetAnimate()->GetRect());
	}

	bool CRole::IsCollisionLevel4(CScallion *level4)
	{
		return IsRectCollision(decisionPoint.GetRect(), level4->GetAnimate()->GetRect());
	}
	
	bool CRole::IsCollisionNPC(CNPC *npc)
	{
		return IsRectCollision(animation.GetRect(), npc->GetAnimate()->GetRect());
	}

	bool CRole::IsCollisionBlackHole(CBlackHole *blackHole)
	{
		//return IsRectCollision(animation.GetRect(), blackHole->GetAnimate()->GetRect());
		return IsRectCollision(decisionPoint.GetRect(), blackHole->GetCenterRect());
	}

	bool CRole::IsCollisionBlackHoleCenter(CBlackHole *blackHole)
	{
		return IsRectCollision(decisionPoint.GetRect(), blackHole->GetCenterRect());
	}
	#pragma endregion

	void CRole::AddScore(int _score)
	{
		score += _score;
		scoreInteger.Add(_score);
	}

	void CRole::SubHp()
	{
		hp--;
		hp_left.Add(-1);

		if (hp <= 0)
		{
			if (!DEBUG_MODE)
			{

			}
		}
	}

	/*void CRole::LoadBitmap()
	{
		animation.LoadBitmap(BitmapPath("RES\\Role\\miku", "MIKU", 13, RGB(255, 255, 255)));
		hp_left.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
		scoreInteger.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
		decisionPoint.LoadBitmap("RES\\Role\\miku\\cursor.bmp", RGB(214, 214, 214));

	}*/

	void CRole::Initialize()
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
		SetValid(true);

		if ( !isLoaded)
		{
			hp_left.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
			scoreInteger.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
			isLoaded = true;
		}

		hp_left.Initialize(CPoint(20, 100), inithp, 2);
		scoreInteger.Initialize(CPoint(500, 0), 0, 3);

		collisionRect = CRect(CPoint(x - move_distance, y - move_distance), CPoint(x + Width() + move_distance, y + Height() + move_distance));

		if (animation.IsNull())
		{
			LoadBitmap("RES\\Role\\miku", "MIKU", 13, RGB(255, 255, 255));
			decisionPoint.LoadBitmap("RES\\Role\\miku\\cursor.bmp", RGB(214, 214, 214));
		}

		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
		CLayerManager::Instance()->AddObject(&decisionPoint, layer.GetLayer() + 1);

		hp = inithp;
		isCatched = false;
	}

	vector<CScallion*>* CRole::GetScallion()
	{
		return &scallion;
	}

	void CRole::ResetCollisionRect()
	{
		collisionRect = animation.GetRect();
		int dx = move_distance - 9;
		if (isMovingRight)
		{
			collisionRect.right += dx;
		}
		else if (isMovingLeft)
		{
			collisionRect.left -= dx;
		}
	}


	#pragma endregion

	#pragma region - CPasserby -
	CPasserby::CPasserby() : CEraser()
	{
		initX = 0;
		x = initX;
		y = 300;
		layer.SetLayer(4);
		score = 0;
		moveTimer = CTimer(0.5);
		stopTimer = CTimer(1.5);
		SetValid(false);
		recreateTimer = CTimer((double)GetRandom(1, 4) / 2.0);
		//animation.ResetWaitTime((double)GetRandom(2, 4) / 5.0);
		//LoadBitmap("Role", "LUKA", 2);
	}

	CPasserby::CPasserby(int _x, int _y, string ziliaojia, string name, int number, int _score) : CEraser()
	{
		initX = _x;
		x = initX;
		y = _y;
		layer.SetLayer(4);
		LoadBitmap(ziliaojia, name, number, RGB(255, 255, 255));
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());

		score = _score;
		moveTimer = CTimer(0.5);
		stopTimer = CTimer(1.5);
		SetValid(false);
		recreateTimer = CTimer((double)GetRandom(1, 4) / 2.0);
	}

	void CPasserby::SetXY(int _x, int _y)
	{
		initX = _x;
		y = _y;

		int dx = CCamera::Instance()->GetX();
		x = initX - dx;
		animation.SetTopLeft(x, y);
	}

	void CPasserby::SetScore(int _score)
	{
		score = _score;
	}

	void CPasserby::SetMoving()
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
	}

	void CPasserby::OnMove()
	{
		const int STEP_SIZE = move_distance;

		if (isMovingUp)
		{
			
		}
		if (isMovingRight)
		{
			if (canMoving)
				initX += STEP_SIZE;
		}
		if (isMovingDown)
		{
			if (canMoving)
				y += STEP_SIZE;
		}
		if (isMovingLeft)
		{
			if (canMoving)
				initX -= STEP_SIZE;
		}

		SetXY(initX, y);
		animation.OnMove();
	}

	CPasserby::~CPasserby()
	{
		
	}
	#pragma endregion

	#pragma region - CNPC -
	CNPC::CNPC() : CEraser()
	{
	}

	CNPC::CNPC(CPoint _point, BitmapPath _loadPath, string _id)
	{
		initPoint = _point;
		initLoadPath = _loadPath;
		id = _id;
	}

	void CNPC::Initialize()
	{
		#pragma region - load animation (only once) -
		if (animation.IsNull())
		{
			LoadBitmap(initLoadPath);
		}
		#pragma endregion

		SetCurrentXY(initPoint.x, initPoint.y);

		layer.SetLayer(NPC_LAYER);
		animation.SetValid(false);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
	}

	void CNPC::SetCurrentXY(int _x, int _y)
	{
		currentX = _x;
		currentY = _y;
		SetXY();
	}

	void CNPC::SetXY()
	{
		x = currentX;
		y = currentY;

		int dx = CCamera::Instance()->GetX();
		x = currentX - dx;
		animation.SetTopLeft(x, y);
	}

	void CNPC::OnMove()
	{
		int dx = CCamera::Instance()->GetX();
		SetCurrentXY(currentX, currentY);
	}

	CNPC::~CNPC()
	{

	}
	#pragma endregion

	#pragma region - CNPC1 - No.1 -
	CNPC1::CNPC1()
	{

	}

	CNPC1::CNPC1(CPoint _point, BitmapPath _loadPath, string _id, string _txt) : CNPC(_point, _loadPath, _id)
	{
		txt = _txt;
	}

	CNPC1::~CNPC1()
	{

	}

	void CNPC1::RoleCollision()
	{
		if (false) // ko no if wa 例外敘述
		{
			
		}
		else
		{
			CDialogManager::Instance()->Start(txt);
		}
	}
	#pragma endregion

	#pragma region - CNPC3 - No.3 -
	CNPC3::CNPC3()
	{
	}

	CNPC3::CNPC3(CPoint _point, BitmapPath _loadPath, string _id, string _music, string _txt) : CNPC(_point, _loadPath, _id)
	{
		openMusic = _music;
		openTxt = _txt;
	}

	CNPC3::~CNPC3()
	{
	}

	void CNPC3::RoleCollision()
	{
		CAudio::Instance()->Stop("MUSIC_GAMEING");
		CAudio::Instance()->Play(openMusic);

		CDialogManager::Instance()->OpenMusicPlayer(openMusic);
		CDialogManager::Instance()->Start(openTxt);
	}
	#pragma endregion
}