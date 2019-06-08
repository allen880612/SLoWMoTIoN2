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

	CBoss::CBoss(int _x, int _y, int _hp, string _id, BitmapPath _loadPath)
	{
		initx = _x;
		inity = _y;
		initHp = _hp;
		loadPath = _loadPath;
		id = _id;
		//Initialize();
	}

	CBoss::~CBoss()
	{
		
	}

	void CBoss::LoadBitmap()
	{
		#pragma region - Load animateion and left, right animation -
		BitmapPath leftPath = loadPath;
		BitmapPath rightPath = loadPath;
		leftPath.ziliaojia += "\\L";
		rightPath.ziliaojia += "\\R";

		animation.LoadBitmap(leftPath.ziliaojia, leftPath.name, leftPath.number, leftPath.color);
		rightAnimate.LoadBitmap(rightPath.ziliaojia, rightPath.name, rightPath.number, rightPath.color);
		#pragma endregion
		animation.SetTopLeft(initx, inity);
		height = animation.Height();
		width = animation.Width();
		leftAnimate = animation;
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
		IsAlive = true;

		animation.SetValid(false);
		CLayerManager::Instance()->AddObject(&animation, layer.GetLayer());
		isEnd = false;
		AliveTime = CTimer(99.0);
	}

	void CBoss::InitializeDirAnimate(string dir,double resetTime)
	{
		animation.ResetDelayTime(resetTime);
		animation.SetTopLeft(initx, inity);
		leftAnimate.ResetDelayTime(resetTime);
		rightAnimate.ResetDelayTime(resetTime);
		leftAnimate.CopyAnimateInformation(&animation);
		rightAnimate.CopyAnimateInformation(&animation);

		faceTo = dir;
		if(faceTo == "left")
			animation = leftAnimate;
		else if(faceTo == "right")
			animation = rightAnimate;
	}

	void CBoss::SetFaceTo(CPoint point)
	{
		if (point.x >= GetCenterPoint().x) //right
		{
			rightAnimate.CopyAnimateInformation(&animation);
			animation = rightAnimate;
			faceTo = "right";
		}
		else
		{
			leftAnimate.CopyAnimateInformation(&animation);
			animation = leftAnimate;
			faceTo = "left";
		}
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

	void CBoss::CollisionScallion(CRole *role)
	{
		#pragma region - scallion collision this -
		vector<CScallion*>* scallion = role->GetScallion();
		for (vector<CScallion*>::iterator scallionk = scallion->begin(); scallionk != scallion->end();)
		{
			if ((*scallionk)->IsAlive() && (*scallionk)->IsCollision(this))
			{
				(*scallionk)->SetIsAlive(false);
				hp -= (*scallionk)->GetAtk();
			}

			if (!(*scallionk)->IsAlive())
			{
				delete *scallionk;
				*scallionk = NULL;
				scallionk = scallion->erase(scallionk);
			}
			else
			{
				scallionk++;
			}
		}
		#pragma endregion
	}

	void CBoss::OnMove()
	{
		int dx = CCamera::Instance()->GetX();
		SetXY(currentX - dx, currentY);
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
		//CBoss();
	}

	CXingting::CXingting(int _x, int _y, int _hp, string _id, BitmapPath _loadPath) : CBoss(_x, _y, _hp, _id,  _loadPath)
	{
		
	}

	CXingting::~CXingting()
	{
		ClearBullet();
	}

	void CXingting::Initialize()
	{
		CBoss::Initialize();

		ClearBullet();

		targetBlackhole = NULL;

		goal_x = 320 - width / 2;
		goal_y = 0;

		shootLevel4_cd = CTimer(0.75);
		shoot_atk2_cd = CTimer(0.033);
		shoot_atk3_cd = CTimer(2.5);

		moveToGoal = CTimer(0.04);
		mode_Attack2_timer = CTimer(8.0);
		mode_Attack4_CreateBlackHole = CTimer(0.5);
		mode4_AttackTime = 1.8; //mode4時 每1.8發射一顆黑洞

		angle_atk2 = 0;
		mode4_RoleMoveDir = 0;

		moveToGoalPoint = true;
		mode_Attack1 = true;
		mode_Attack2 = false;
		mode_Attack3 = false;
		mode_Attack4 = false;

		atkCounter = 0;

		AliveTime = CTimer(40.0);
	}

	void CXingting::OnCycle(CRole *role)
	{
		AliveTime.CountDown();
		if (AliveTime.IsTimeOut()) //時間到 杏庭自爆
		{
			if (IsAlive)
			{
				CDialogManager::Instance()->Start(DIALOG_DATA_VSXingting3);
				IsAlive = false;
				isEnd = true;
			}
		}

		if (!IsDead())
		{
			OnMove();
			Attack(role);
		}
	}

	void CXingting::OnMove()
	{
		if (moveToGoalPoint)
		{
			int move_dx = (currentX > goal_x) ? (initx - goal_x) / 50 : 0;
			int move_dy = (currentY > goal_y) ? (inity - goal_y) / 50 : 0;
			moveToGoal.CountDown();
			if (moveToGoal.IsTimeOut())
			{
				SetCurrentXY(currentX - move_dx, currentY - move_dy);
				moveToGoal.ResetTime();
			}
			if (currentX <= goal_x && currentY <= goal_y)
			{
				moveToGoalPoint = false;
				mode_Attack1 = false;
				mode_Attack2 = true;
				mode_Attack3 = true;

				//mode_Attack1 = false;
				//mode_Attack4 = true;
				atkCounter = 0;
				CDialogManager::Instance()->Start(DIALOG_DATA_VSXingting2);
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
			mode_Attack2_timer.CountDown();
			if (mode_Attack2_timer.IsTimeOut())
			{
				mode_Attack2 = false;
				mode_Attack3 = false;
				mode_Attack4 = true;
			}
		}
		if (mode_Attack3)
		{
			Attack3();
		}
		if (mode_Attack4)
		{
			Attack4(role);
		}
		Level4Collision(role);
	}

	void CXingting::Attack1()
	{
		shootLevel4_cd.CountDown();
		if (shootLevel4_cd.IsTimeOut())
		{
			for (int i = 0; i < 12; i++)
			{
				double speed = 15.0 / 2;
				int angle = (-150 + atkCounter * 15) + i * 30;
				double mx = sin(angle * (PI / 180.0)) * speed;
				double my = cos(angle * (PI / 180.0)) * speed;
				int drs = 3; //給一個倍數讓books不從中心點出現
				CPoint center = GetCenterPoint() + CPoint((int)mx * drs, -(int)my * drs);
				CScallion *newlevel4 = new CScallion(BitmapPath("RES\\Object\\books", "book", 4), center, CPoint(0, 0), 0); //先創建一個蔥的物件
				newlevel4->SetInitVelocity((int)mx, (int)my);
				level4.push_back(newlevel4); //將蔥放進vector
				shootLevel4_cd.ResetTime();
			}
		}
	}

	void CXingting::Attack2()
	{
		shoot_atk2_cd.CountDown();
		if (shoot_atk2_cd.IsTimeOut())
		{
			#pragma region - bullet parameter -
			double speed = 15.0 / 2;
			int angle = angle_atk2;
			double mx = sin(angle * (PI / 180.0)) * speed;
			double my = cos(angle * (PI / 180.0)) * speed;
			int drs = 3; //用意在給一個倍數讓books不從中心點出現
			CPoint center = GetCenterPoint() + CPoint((int)mx * drs, -(int)my * drs);
			#pragma endregion

			CScallion *newlevel4 = new CScallion(BitmapPath("RES\\Object\\books", "book", 4), center, CPoint(0, 0), 0); //先創建一個蔥的物件
			newlevel4->SetInitVelocity((int)mx, (int)my);
			level4.push_back(newlevel4); //將蔥放進vector
			shoot_atk2_cd.ResetTime();
			angle_atk2 += 31;
		}
	}

	void CXingting::Attack3()
	{
		shoot_atk3_cd.CountDown();
		if (shoot_atk3_cd.IsTimeOut())
		{
			for (int i = 0; i < 18; i++)
			{
				#pragma region - bullet parameter -
				double speed = 10.0 / 2;
				double angle = i * (360/18);
				double mx = sin(angle * (PI / 180.0)) * speed;
				double my = cos(angle * (PI / 180.0)) * speed;
				int drs = 3; //亂取的名字 用意在給一個倍數讓books不從中心點出現
				CPoint center = GetCenterPoint() + CPoint((int)my * drs, -(int)mx * drs);
				#pragma endregion
				CScallion *newlevel4 = new CScallion(BitmapPath("RES\\Object\\books", "book", 4), center, CPoint(0, 0), 0); //先創建一個蔥的物件
				newlevel4->SetInitVelocity((int)my, (int)mx);
				level4.push_back(newlevel4); //將蔥放進vector
				shoot_atk3_cd.ResetTime();
			}
		}

	}

	void CXingting::Attack4(CRole *role)
	{
		mode_Attack4_CreateBlackHole.CountDown();
		if (mode_Attack4_CreateBlackHole.IsTimeOut())
		{
			CPoint center = GetCenterPoint();
			CBlackHole *newBh = new CBlackHole(BitmapPath("RES\\Object\\blackhole", "blackhole", 1, RGB(0, 0, 0)), center + CPoint(-75, -68), CPoint(role->GetX1(), role->GetY1()), 0); //先創建一個蔥的物件

			blackhole.push_back(newBh);
			mode_Attack4_CreateBlackHole.ResetTime(mode4_AttackTime); //reset to 10s
		}
	}

	void CXingting::Clear()
	{
		ClearBullet();
		animation.SetValid(false);
	}

	void CXingting::ClearBullet()
	{
		for (vector<CBlackHole*>::iterator bkiter = blackhole.begin(); bkiter != blackhole.end(); bkiter++)
		{
			delete *bkiter;
			*bkiter = NULL;
		}
		for (vector<CScallion*>::iterator level4iter = level4.begin(); level4iter != level4.end(); level4iter++)
		{
			delete *level4iter;
			*level4iter = NULL;
		}
		level4.clear();
		blackhole.clear();
	}

	bool CXingting::IsDead()
	{
		return AliveTime.IsTimeOut();
	}

	void CXingting::Level4Collision(CRole *role)

	{
		for (vector<CBlackHole*>::iterator bkiter = blackhole.begin(); bkiter != blackhole.end();)
		{
			(*bkiter)->OnMove();
			if ((*bkiter)->IsAlive() && !role->IsCatched() && role->IsCollisionBlackHole((*bkiter)))
			{
				targetBlackhole = *bkiter;
				role->SetCatched(true);
				(*bkiter)->SetRole(role);
			}

			if ((*bkiter)->IsDead())
			{
				#pragma region - Create Bullet -
				int bulletNumber = 12;
				for (int i = 0; i < bulletNumber; i++)
				{
					#pragma region - bullet parameter -
					double speed = 15.0 / 2;
					double angle = i * (360 / bulletNumber);
					double mx = sin(angle * (PI / 180.0)) * speed;
					double my = cos(angle * (PI / 180.0)) * speed;
					int drs = 3; //亂取的名字 用意在給一個倍數讓books不從中心點出現
					CPoint center = GetCenterPoint() + CPoint((int)my * drs, -(int)mx * drs);
					#pragma endregion
					CScallion *newlevel4 = new CScallion(BitmapPath("RES\\Object\\books", "book", 4), (*bkiter)->GetCenterPoint(), CPoint(0, 0), 0); //先創建一個蔥的物件
					newlevel4->SetInitVelocity((int)mx, (int)my);
					level4.push_back(newlevel4); //將蔥放進vector
				}
				#pragma endregion
				targetBlackhole = NULL;
				role->SetCatched(false);
			}
				
			if (!(*bkiter)->IsAlive())
			{
				delete *bkiter;
				*bkiter = NULL;
				bkiter = blackhole.erase(bkiter);
			}
			else
			{
				bkiter++;
			}
		}
		for (vector<CScallion*>::iterator level4iter = level4.begin(); level4iter != level4.end();)
		{
			(*level4iter)->OnMove();
			if ((*level4iter)->IsAlive() && role->IsCollisionLevel4(*level4iter))
			{
				(*level4iter)->SetIsAlive(false);
				role->SubHp();
			}
			
			if(!(*level4iter)->IsAlive())
			{
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

	#pragma region - CFaicaiSeed -
	CFacaiSeed::CFacaiSeed()
	{
	}

	CFacaiSeed::CFacaiSeed(int _x, int _y, int _hp, string _id, BitmapPath _loadPath) : CBoss(_x, _y, _hp, _id, _loadPath)
	{
		
	}
	
	CFacaiSeed::~CFacaiSeed()
	{
		Clear();
	}

	void CFacaiSeed::Initialize()
	{
		CBoss::Initialize();
		InitializeDirAnimate("right", 0.05);
		#pragma region - Init ray -
		ray = NULL;
		rayStartTime.ResetTime(2.5); //預設一秒後發射
		rayStayTime.ResetTime(2.0); //預設持續兩秒
		fly = false;
		flyY = 1;
		#pragma endregion
		Clear();

		movingTime = CTimer(0.2);
		shootCoinTimer = CTimer(0.2); //每0.2s發射1波金幣
		attackRoleTimer = CTimer(0.4); //硬值時間0.4s
		subRoleHp_NoEQ = CTimer(0.6); //每0.6s掉一次血
		AliveTime = CTimer(99.0);
	}

	void CFacaiSeed::OnCycle(CRole *role)
	{
		#pragma region - boss deaa - 
		if (hp <= 0) //boss dead
		{
			CDialogManager::Instance()->Start("roleWinFacaiSeed");
			SetIsAlive(false);
		}
		#pragma endregion

		#pragma region - Boss Alive -
		if (!IsDead())
		{
			attackRoleTimer.CountDown();
			if(ray == NULL) //射線中不轉換
				SetFaceTo(CPoint(role->GetX3(), role->GetY3()));
			Attack(role);
			OnMove();
			Collision(role);
		}
		#pragma endregion

		#pragma region - role dead -
		if (role->IsDead() || AliveTime.IsTimeOut())
		{
			CDialogManager::Instance()->Start("roleLoseFacaiSeed");
			ClearBullet();
			fly = true;
		}
		#pragma endregion
	}

	void CFacaiSeed::Attack(CRole *role)
	{
		Attack1(role);
		if(ray == NULL)
			Attack2();
		Attack3();
	}

	void CFacaiSeed::OnMove()
	{
		#pragma region - rays animation -
		if (ray != NULL)
		{
			ray->OnMove();
		}
		#pragma endregion

		#pragma region - dont have ray - move -
		else
		{
			int dx = 1;
			if (faceTo == "right")
			{
				SetCurrentXY(currentX + dx, currentY);
			}
			else if (faceTo == "left")
			{
				SetCurrentXY(currentX - dx, currentY);
			}
		}
		#pragma endregion
		
		CBoss::OnMove();
	}

	void CFacaiSeed::Clear()
	{
		ClearBullet();

		animation.SetValid(false);
	}

	void CFacaiSeed::EndProcess()
	{
		#pragma region - role dead and this fly -
		if (fly)
		{
			SetCurrentXY(currentX, currentY - flyY);
			flyY++;
			CBoss::OnMove();
			if (GetRightBottomPoint().y <= -10)
			{
				fly = false;
				isEnd = true;
				Clear();
			}
		}
		#pragma endregion
	}

	void CFacaiSeed::Attack1(CRole *role)
	{
		if (role->IsZZ())
		{
			subRoleHp_NoEQ.CountDown();
			if (subRoleHp_NoEQ.IsTimeOut())
			{
				role->SubHp();
				subRoleHp_NoEQ.ResetTime();
			}
		}
	}

	void CFacaiSeed::Attack2()
	{
		shootCoinTimer.CountDown();
		if (shootCoinTimer.IsTimeOut())
		{
			for (int i = 0; i < 4; i++)
			{
				double speed = 27.0;
				int angle = coinAngle[i];
				double mx = sin(angle * (PI / 180.0)) * speed;
				double my = cos(angle * (PI / 180.0)) * speed;
				double drs = 2.5;
				CPoint center = GetCreateCoinPoint() + CPoint((int)(my * drs), -(int)(mx * drs));
				CScallion *newcoin = new CScallion(BitmapPath("RES\\Object\\coin", "coin", 4, RGB(255, 255, 255)), center, CPoint(0, 0), 1); //先創建一個蔥的物件
				newcoin->SetInitVelocity((int)my, (int)mx);
				coinVector.push_back(newcoin);
				shootCoinTimer.ResetTime();
			}
		}
	}

	void CFacaiSeed::Attack3()
	{
		rayStartTime.CountDown();
		if (rayStartTime.IsTimeOut())
		{
			if (ray == NULL)
			{
				if (faceTo == "right")
				{
					ray = new CRay(BitmapPath("RES\\Object\\Ray", "ray", 5, RGB(214, 214, 214)), CPoint(GetRightBottomPoint().x, GetLeftTopPoint().y + 40));
				}
				else
				{
					ray = new CRay(BitmapPath("RES\\Object\\Ray", "ray", 5, RGB(214, 214, 214)), CPoint(GetLeftTopPoint().x - 531, GetLeftTopPoint().y + 40));
				}
			}

			rayStayTime.CountDown(); //開始計算持續時間
			if (rayStayTime.IsTimeOut()) //delete ray
			{
				if (ray != NULL)
				{
					delete ray;
					ray = NULL;
				}
				rayStayTime.ResetTime();
				rayStartTime.ResetTime();
			}
		}
	}
	void CFacaiSeed::ClearBullet()
	{
		#pragma region - clear coin -
		for (vector<CScallion*>::iterator coiniter = coinVector.begin(); coiniter != coinVector.end(); coiniter++)
		{
			delete *coiniter;
			*coiniter = NULL;
		}
		coinVector.clear();
		#pragma endregion

		#pragma region - clear ray -
		if (ray != NULL)
		{
			delete ray;
			ray = NULL;
		}
		#pragma endregion
	}

	void CFacaiSeed::Collision(CRole *role)
	{
		#pragma region - coin collision role -
		for (vector<CScallion*>::iterator cointiter = coinVector.begin(); cointiter != coinVector.end();)
		{
			(*cointiter)->OnMove();
			if ((*cointiter)->IsAlive() && role->IsCollisionLevel4(*cointiter))
			{
				(*cointiter)->SetIsAlive(false);
				role->SubHp();
			}

			if (!(*cointiter)->IsAlive())
			{
				delete *cointiter;
				*cointiter = NULL;
				cointiter = coinVector.erase(cointiter);
			}
			else
			{
				cointiter++;
			}
		}
		#pragma endregion

		#pragma region - ray collision role -
		if (ray != NULL)
		{
			if (ray->GetAttackValid() && role->IsCollisionRay(ray))
			{
				if (attackRoleTimer.IsTimeOut())
				{
					role->SubHp();
					attackRoleTimer.ResetTime();
				}
			}
		}
		#pragma endregion

		#pragma region - this collision role -
		if (IsRectCollision(role->GetAction()->GetRect(), animation.GetRect()))
		{
			if (attackRoleTimer.IsTimeOut())
			{
				role->SubEq();
				attackRoleTimer.ResetTime();
			}
		}
		#pragma endregion

		#pragma region - this collsion scallion -
		CollisionScallion(role);
		#pragma endregion
	}
	CPoint CFacaiSeed::GetCreateCoinPoint()
	{
		CPoint center = GetCenterPoint();
		int dx = 45;
		if (faceTo == "left")
		{
			center += CPoint(dx, 0);
		}
		else
		{
			center -= CPoint(dx, 0);
		}
		return center;
	}
	#pragma endregion

}