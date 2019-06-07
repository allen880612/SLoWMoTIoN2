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

	#pragma region - CEraser -
	CEraser::CEraser()
	{
		Initialize();
	}

	CEraser::~CEraser()
	{
		//animation.ReleaseAnimate();
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

		//animation.LoadBitmap(role, RGB(255, 255, 255));
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

	/*void CEraser::LoadAction(string _action, BitmapPath loadpath)
	{
		action.LoadAction(_action, loadpath);
	}*/

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
		//animation.SetTopLeft(x, y);
		//animation.OnShow();
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

	int CRole::GetX2()
	{
		return x + action.Width();
	}

	int CRole::GetY2()
	{
		return y + action.Height();
	}

	int CRole::GetY3()
	{
		return y + action.Height() / 2;
	}

	int CRole::GetX3()
	{
		return x + action.Width() / 2;
	}

	int CRole::Height()
	{
		return action.Height();
	}

	int CRole::Width()
	{
		return action.Width();
	}

	void CRole::SetXY(int _x, int _y)
	{
		x = _x;
		y = _y;
		action.SetTopLeft(_x, _y);
	}

	bool CRole::GetValid()
	{
		return action.GetValid();
	}

	void CRole::SetValid(bool _flag)
	{
		action.SetValid(_flag);
	}

	void CRole::Load()
	{
		LoadAction("idle", BitmapPath("RES\\Role\\miku\\idle", "idle", 19, RGB(150, 200, 250)));
		LoadAction("run", BitmapPath("RES\\Role\\miku\\run", "run", 7, RGB(150, 200, 250)));
		LoadAction("jump", BitmapPath("RES\\Role\\miku\\jump", "jump", 7, RGB(150, 200, 250)));
		//hp_left.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
		scoreInteger.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
		decisionPoint.LoadBitmap("RES\\Role\\miku\\cursor.bmp", RGB(214, 214, 214));

		blood.LoadBitmap("RES\\UI\\status\\blood.bmp", RGB(214, 214, 214));
		blood_frame.LoadBitmap("RES\\UI\\status\\bar_frame.bmp", RGB(214, 214, 214));
		EQ.LoadBitmap("RES\\UI\\status\\blood.bmp", RGB(214, 214, 214));
		EQ_frame.LoadBitmap("RES\\UI\\status\\bar_frame.bmp", RGB(214, 214, 214));

		avatar.LoadBitmap("RES\\UI\\status\\avatar.bmp", RGB(214, 214, 214));
		avatar_frame.LoadBitmap("RES\\UI\\status\\avatar_frame.bmp", RGB(214, 214, 214));
	}

	void CRole::LoadAction(string _action, BitmapPath _loadpath)
	{
		action.LoadAction(_action, _loadpath);
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
				CAudio::Instance()->Play("jump");
				isJumping = true;
				//y -= STEP_SIZE;
			}
		}
		if (isMovingRight)
		{
			dir = 2;
			if (canMoving)
				x += STEP_SIZE;

			action.SetFaceTo("_R");
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

			action.SetFaceTo("_L");
		}
		if (isJumping)
		{
			if (canJumping)
			{
				velocity = init_velocity;
				canJumping = false;
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

		shoot_cd.CountDown();	//�]�m�g���� CD�ɶ�

		//animation.SetTopLeft(x, y);
		//animation.OnMove(dir);
		if (isJumping)
		{
			decisionPoint.SetTopLeft(GetX3() - 5, GetY3() + 16);
			//decisionPoint.SetTopLeft(GetX3() - 5, GetY3() + 5);
		}
		else
		{
			decisionPoint.SetTopLeft(GetX3() - 5, GetY3());
		}

		action.SetTopLeft(x, y);
		if (isJumping)
		{
			action.OnMove("jump");
			action.SetTopLeft(x, y - (action.GetActionHeight("jump") - action.GetActionHeight("idle")));
		}
		else if (isMovingLeft || isMovingRight)
		{
			action.OnMove("run");
		}
		else
		{
			action.OnMove("idle");
		}
		
		#pragma region -- Reset collision rect --
		/*collisionRect.left = action.GetRect().left - move_distance;
		collisionRect.right = action.GetRect().right + move_distance;
		collisionRect.top = action.GetRect().top - move_distance;
		collisionRect.bottom = action.GetRect().bottom + move_distance;*/
		#pragma endregion

		for (unsigned int i = 0; i < scallion.size(); i++)
		{
			scallion[i]->OnMove();
		}
	}

	void CRole::OnShow()
	{
		//hp_left.ShowBitmap();
		scoreInteger.ShowBitmap();
		//action.OnShow();
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
			CAudio::Instance()->Play("throw");
			CScallion *newCScallion = new CScallion(BitmapPath("RES\\Object\\Scallions", "scallion", 2), CPoint(GetX3(), GetY1()), CPoint(mouseX, mouseY)); //���Ыؤ@�ӽ�������

			//CScallion *newCScallion = new CScallion("Role\\books", "book", 4, GetX3(), GetY1(), mx, my); //���Ыؤ@�ӽ�������
			scallion.push_back(newCScallion); //�N����ivector
			shoot_cd.ResetTime(0.33);
		}
	}

	void CRole::SetDrop()
	{
		SetMovingJump(true);
		SetCanJumping(false);
		SetVelocity(0);
	}

	bool CRole::GetDrop()
	{
		return velocity < 0;
	}

	int CRole::GetVelocity()
	{
		return velocity;
	}

	bool CRole::IsMoving()
	{
		return (isMovingRight || isMovingLeft);
	}

	#pragma region -- Collision --
	bool CRole::IsCollisionBoss(CBoss *boss)
	{
		ResetCollisionRect();

		return IsRectCollision(collisionRect, boss->GetAnimate()->GetRect());
	}

	bool CRole::IsCollisionBlock(CBlock *block) //get iterator in mygame.cpp
	{
		ResetCollisionRect();

		return IsRectCollision(collisionRect, block->bmp.GetRect());
	}

	bool CRole::IsRoleOnBlock(CBlock *block)
	{
		ResetCollisionRect();

		return IsRectCollision(collisionDownRect, block->bmp.GetRect());
	}

	bool CRole::IsCollisionBlockOnJumping(CBlock *block)
	{
		ResetCollisionRect();

		return IsRectCollision(collisionTopRect, block->bmp.GetRect());
	}

	bool CRole::IsCollisionDoor(CDoor *door)
	{
		ResetCollisionRect();

		return IsRectCollision(collisionDoorRect, door->bmp.GetRect());
	}

	bool CRole::IsCollisionLevel4(CScallion *level4)
	{
		return IsRectCollision(decisionPoint.GetRect(), level4->GetAnimate()->GetRect());
	}
	
	bool CRole::IsCollisionNPC(CNPC *npc)
	{
		return IsRectCollision(action.GetRect(), npc->GetAnimate()->GetRect());
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
	
		int deltaHP = blood.Width() / inithp;
		blood.SetTopLeft(blood.Left() - deltaHP, blood.Top());

		//hp_left.Add(-1);

		if (hp <= 0)
		{
			isDead = true;
		}
	}

	void CRole::Initialize()
	{
		action.Initialize();

		#pragma region  Normal Setting
		CEraser::Initialize();
		isJumping = true;
		canJumping = false;
		isFire = false;
		const int INIT_VELOCITY = 30;				//�]�w��t��
		const int GRAVITY = 2;						//�]�w���O
		init_velocity = velocity = INIT_VELOCITY;
		gravity = GRAVITY;
		mouse_x, mouse_y = 0;
		x = 0;
		y = -Height();
		score = 0;

		shoot_cd = CTimer(0);						//��l�Ʈg���N�o�ɶ�
		hp = inithp;
		isCatched = false;
		isDead = false;
		SetValid(true);
		#pragma endregion

		#pragma region �ש󤣥γo�o���F
		if (!isLoaded)
		{
			isLoaded = true;
		}
		#pragma endregion

		avatar.SetTopLeft(0, 0);
		avatar_frame.SetTopLeft(0, 0);

		const int BLOOD_START = avatar_frame.Width();
		const int PADDING_Y = 15;
		blood_frame.SetTopLeft(BLOOD_START, PADDING_Y);
		blood.SetTopLeft(BLOOD_START, PADDING_Y);
		EQ_frame.SetTopLeft(BLOOD_START, blood_frame.Height() + PADDING_Y);
		EQ.SetTopLeft(BLOOD_START, blood_frame.Height() + PADDING_Y);

		//hp_left.Initialize(CPoint(20, 100), inithp, 2);
		scoreInteger.Initialize(CPoint(500, 0), 0, 3);

		collisionRect = CRect(CPoint(x - move_distance, y - move_distance), CPoint(x + Width() + move_distance, y + Height() + move_distance));

		CLayerManager::Instance()->AddObject(&decisionPoint, layer.GetLayer() + 1);
		CLayerManager::Instance()->AddObject(&action, layer.GetLayer());

		CLayerManager::Instance()->AddObject(&blood, INTERFACE_LAYER - 1);
		CLayerManager::Instance()->AddObject(&blood_frame, INTERFACE_LAYER);
		CLayerManager::Instance()->AddObject(&EQ, INTERFACE_LAYER - 1);
		CLayerManager::Instance()->AddObject(&EQ_frame, INTERFACE_LAYER);
		CLayerManager::Instance()->AddObject(&avatar_frame, INTERFACE_LAYER);
		CLayerManager::Instance()->AddObject(&avatar, INTERFACE_LAYER);


	}

	vector<CScallion*>* CRole::GetScallion()
	{
		return &scallion;
	}

	void CRole::ResetCollisionRect()
	{
		#pragma region - collision Rect - Right and Left -
		collisionRect = action.GetRect();
		int dx = move_distance;
		collisionRect.bottom -= 10;
		if (isMovingRight)
		{
			//Test�a��վ㩹�k/�������P�_�e�� (�쥻�H���e�� + dx�A�令�q�����I��_���e�� + dx)
			collisionRect.left = GetX3(); //Test
			collisionRect.right += dx;
		}
		else if (isMovingLeft)
		{
			collisionRect.left -= dx;
			collisionRect.right = GetX3(); //Test
		}
		#pragma endregion
		
		#pragma region - collision Rect - Down -
		collisionDownRect = action.GetRect();
		//�վ� �U��P�_��������סA�H���Y�/�k���e��
		collisionDownRect.top = GetY2() - 5; //�վ�P�_������
		collisionDownRect.right -= dx * 3;
		collisionDownRect.left += dx * 3;
		int dy = move_distance ;
		if (GetDrop())
		{
			collisionDownRect.bottom += dy;
		}
		#pragma endregion

		#pragma region - collision Rect - Top -
		collisionTopRect = action.GetRect();
		//�Y�/�k���e��
		collisionTopRect.right -= dx * 3;
		collisionTopRect.left += dx * 3;
		collisionTopRect.bottom = GetY3(); //�վ�P�_�������
		if (!GetMovingJump()) //�S���D
		{
			collisionTopRect.top -= dy * 3;
		}
		else if (GetMovingJump() && !GetDrop()) //���D��
		{
			collisionTopRect.top += dy * 3;
		}
		#pragma endregion

		#pragma region - collsion Rect - Door -
		collisionDoorRect = decisionPoint.GetRect();
		collisionDoorRect.top -= 20;
		collisionDoorRect.bottom += 20;
		collisionDoorRect.left -= 20;
		collisionDoorRect.right += 20;
		#pragma endregion
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

	CPasserby::CPasserby(int _x, int _y) : CEraser()
	{
		initX = _x;
		x = initX;
		y = _y;
		layer.SetLayer(4);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());

		score = 0;
		moveTimer = CTimer(0.5);
		stopTimer = CTimer(1.5);
		SetValid(true);
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

	#pragma region - CPasserby1 - Luka -
	CLuka::CLuka() : CPasserby()
	{
	}
	CLuka::CLuka(int _x, int _y) : CPasserby(_x, _y)
	{
		SetScore(10);
		LoadBitmap("RES\\Role\\NPC\\LUKA", "LUKA", 2, RGB(214, 214, 214));
	}
	CLuka::~CLuka()
	{
	}
	#pragma endregion

	#pragma region - CPasserby2 - Rin -
	CRin::CRin() : CPasserby()
	{
	}
	CRin::CRin(int _x, int _y) : CPasserby(_x, _y)
	{
		SetScore(15);
		LoadBitmap("RES\\Role\\NPC\\RIN", "RIN", 2, RGB(214, 214, 214));
	}
	CRin::~CRin()
	{
	}
	#pragma endregion

	#pragma region - CPasserby3 - mushroom -
	CMushroom::CMushroom() : CPasserby()
	{
	}
	CMushroom::CMushroom(int _x, int _y) : CPasserby(_x, _y)
	{
		SetScore(20);
		LoadBitmap("RES\\Role\\NPC\\mushroom\\L", "mushroom", 5, RGB(214, 214, 214));
		leftAnimate = animation;
		rightAnimate.LoadBitmap("RES\\Role\\NPC\\mushroom\\R", "mushroom", 5, RGB(214, 214, 214));

		leftAnimate.ResetDelayTime(0.1);
		rightAnimate.ResetDelayTime(0.1);

		leftAnimate.CopyAnimateInformation(&animation);
		rightAnimate.CopyAnimateInformation(&animation);
		animation = leftAnimate;
		faceto = "left";
	}
	CMushroom::~CMushroom()
	{
	}
	void CMushroom::OnMove()
	{
		if (GetMovingLeft() && faceto != "left")
		{
			leftAnimate.CopyAnimateInformation(&animation);
			animation = leftAnimate;
			faceto = "left";
		}
		else if (GetMovingRight() && faceto != "right")
		{
			rightAnimate.CopyAnimateInformation(&animation);
			animation = rightAnimate;
			faceto = "right";
		}
		CPasserby::OnMove();
	}
	#pragma endregion

	#pragma endregion

	#pragma region - CNPC -
	CNPC::CNPC() : CEraser()
	{
	}

	CNPC::CNPC(CPoint _point, BitmapPath _loadPath, string _id, double resetTime)
	{
		initPoint = _point;
		initLoadPath = _loadPath;
		id = _id;
		initResetTime = resetTime;
	}

	void CNPC::Initialize()
	{
		#pragma region - load animation (only once) -
		if (animation.IsNull())
		{
			BitmapPath leftPath = initLoadPath;
			leftPath.ziliaojia += "\\L\\";

			BitmapPath rightPath = initLoadPath;
			rightPath.ziliaojia += "\\R\\";

			LoadBitmap(leftPath);
			leftAnimate = animation;
			rightAnimate.LoadBitmap(rightPath);

			leftAnimate.ResetDelayTime(initResetTime);
			rightAnimate.ResetDelayTime(initResetTime);
			leftAnimate.CopyAnimateInformation(&animation);
			rightAnimate.CopyAnimateInformation(&animation);
			faceTo = "left";
		}
		#pragma endregion

		SetCurrentXY(initPoint.x, initPoint.y);

		layer.SetLayer(NPC_LAYER);
		animation.SetValid(false);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
	}

	void CNPC::SetValid(bool flag)
	{
		animation.SetValid(flag);
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

	void CNPC::LookRole(CPoint rolePoint)
	{
		if (currentX < rolePoint.x && faceTo != "right") //look right
		{
			rightAnimate.CopyAnimateInformation(&animation);
			animation = rightAnimate;
			faceTo = "right";
		}
		else if (currentX >= rolePoint.x && faceTo != "left")
		{
			leftAnimate.CopyAnimateInformation(&animation);
			animation = leftAnimate;
			faceTo = "left";
		}
	}

	void CNPC::OnCycle(CPoint rolePoint)
	{
		LookRole(rolePoint);
		OnMove();
	}

	void CNPC::OnMove()
	{
		int dx = CCamera::Instance()->GetX();
		SetCurrentXY(currentX, currentY);
		animation.OnMove();
	}

	CNPC::~CNPC()
	{

	}
	#pragma endregion

	#pragma region - CNPC1 - No.1 -
	CNPC1::CNPC1()
	{

	}

	CNPC1::CNPC1(CPoint _point, BitmapPath _loadPath, string _id, string _txt, double resetTime) : CNPC(_point, _loadPath, _id, resetTime)
	{
		txt = _txt;
	}

	CNPC1::~CNPC1()
	{
		
	}

	void CNPC1::RoleCollision()
	{
		if (false) // ko no if wa �ҥ~�ԭz
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

	CNPC3::CNPC3(CPoint _point, BitmapPath _loadPath, string _id, string _music, string _txt) : CNPC(_point, _loadPath, _id, 0.1)
	{
		openMusic = _music;
		openTxt = _txt;
		//animation.ResetDelayTime(0.1);
		isLoadHeadPhoneIcon = false;
	}

	CNPC3::~CNPC3()
	{

	}

	void CNPC3::RoleCollision()
	{
		CAudio::Instance()->Stop("SLoWMoTIoN_Game");
		CAudio::Instance()->Play(openMusic, true);

		CDialogManager::Instance()->OpenMusicPlayer(openMusic);
		CDialogManager::Instance()->Start(openTxt);
	}
	void CNPC3::Initialize()
	{
		CNPC::Initialize();
		if (!isLoadHeadPhoneIcon)
		{
			headphoneIcon.LoadBitmap("RES\\NPC\\headphone3.bmp", RGB(255, 255, 255));
			isLoadHeadPhoneIcon = true;
		}
		headphoneIcon.SetTopLeft(GetX3() - headphoneIcon.Width() / 2, GetY1() - headphoneIcon.Height() - 10);
		headphoneIcon.SetValid(false);
		CLayerManager::Instance()->AddObject(&headphoneIcon, layer.GetLayer());
	}

	void CNPC3::SetValid(bool flag)
	{
		animation.SetValid(flag);
		headphoneIcon.SetValid(flag);
	}
	void CNPC3::OnMove()
	{
		CNPC::OnMove();
		headphoneIcon.SetTopLeft(GetX3() - headphoneIcon.Width() / 2, GetY1() - headphoneIcon.Height() - 10);
	}
	#pragma endregion
}