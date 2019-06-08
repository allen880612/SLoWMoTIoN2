#pragma once
#include "CLibrary.h"
#include "Refactor.h"
#include <vector>
using namespace myLibrary;
namespace game_framework
{
	#pragma region - CBoss -
	class CScallion; //???????????????????????我明明沒有在scallion引入boss???????????
					 //難到是傳說中的三重引入?????????????????????????????????????
	class CRole;
	class CBlackHole;
	class CBoss
	{
	public:
		CBoss();
		CBoss(int, int, int, string, BitmapPath); //x, y, hp, (路徑, 顏色)
		virtual ~CBoss();

		CPoint GetLeftTopPoint() { return CPoint(currentX, currentY); };
		CPoint GetCenterPoint() { return CPoint(currentX + width / 2, currentY + height / 2); };
		CPoint GetRightBottomPoint() { return CPoint(currentX + width, currentY + height); };

		void LoadBitmap();

		void virtual Initialize();
		void InitializeDirAnimate(string, double = 0.1);
		void SetFaceTo(CPoint);
		void SetXY(int, int);
		void SetCurrentXY(int, int);
		void SetHp(int);
		void SetIsAlive(bool);
		bool GetAlive() { return IsAlive; };
		string GetID() { return id; };
		int GetHp() { return hp; };
		int GetInitHp() { return initHp; };
		void OnMove();
		void MoveWithMap(string);
		void CollisionScallion(CRole *);

		virtual void OnCycle(CRole*) {};
		virtual void Attack(CRole*) {};

		virtual void Clear() {};

		virtual vector<CScallion*>* GetBullet() { return nullptr; };
		bool IsDead() { return !IsAlive; };

		CTimer* GetAliveTimer() { return &AliveTime; };

		CLayer layer;

		CAnimate* GetAnimate();
		string GetBossId() { return id; };
	protected:
		#pragma region Init boss Information
		int initx, inity, initHp;
		BitmapPath loadPath;
		COLORREF transparentColor;
		#pragma endregion

		CAnimate animation;
		CAnimate leftAnimate;
		CAnimate rightAnimate;
		string faceTo;

		int hp;
		int currentX, currentY;
		int x, y;
		int width, height;
		bool IsAlive = true;
		string id;
		CTimer AliveTime;
		const double PI = 4 * atan(1.0);
	};
	#pragma endregion

	#pragma region - CXingting -
	class CXingting : public CBoss
	{
	public:
		CXingting();
		CXingting(int, int, int, string, BitmapPath);
		~CXingting();
		void Initialize();
		void OnCycle(CRole*);
		void Attack(CRole*);
		void OnMove();
		void Clear();
		void ClearBullet();
		bool IsDead();
	private:
		vector<CScallion*>	level4;
		vector<CBlackHole*> blackhole;
		CBlackHole *targetBlackhole;
		void Level4Collision(CRole*);

		CTimer shootLevel4_cd;
		CTimer shoot_atk2_cd;
		CTimer shoot_atk3_cd;

		CTimer moveToGoal;
		CTimer mode_Attack2_timer;
		CTimer mode_Attack4_CreateBlackHole;

		int goal_x, goal_y;
		int angle_atk2;
		double mode4_AttackTime;
		int mode4_RoleMoveDir; // 1 = 左 2 = 右 3 = 不能動

		void Attack1();
		void Attack2();
		void Attack3();
		void Attack4(CRole*);

		bool moveToGoalPoint;
		bool mode_Attack1;
		bool mode_Attack2;
		bool mode_Attack3;
		bool mode_Attack4;
		int atkCounter;
	};
	#pragma endregion

	#pragma region - CFaicaiSeed -
	class CRay;
	class CFacaiSeed : public CBoss
	{
	public:
		CFacaiSeed();
		CFacaiSeed(int, int, int, string, BitmapPath);
		~CFacaiSeed();
		void Initialize();

		void OnCycle(CRole*);
		void Attack(CRole*);
		void OnMove();
		void Clear();
	private:
		void Attack1(CRole*);
		void Attack2();
		void Attack3(); //Ray attack

		void Collision(CRole*);
		CPoint GetCreateCoinPoint(); //射出金幣的起始點

		CTimer movingTime;
		CTimer shootCoinTimer;
		CTimer attackRoleTimer; //role 硬值時間
		CTimer subRoleHp_NoEQ;

		vector<CScallion*>	coinVector;
		int coinAngle[4] = { 45, 75, 105, 135 };
		#pragma region - ray -
		CTimer rayStartTime;
		CTimer rayStayTime;
		CRay *ray;
		#pragma endregion
	};
	#pragma endregion

}