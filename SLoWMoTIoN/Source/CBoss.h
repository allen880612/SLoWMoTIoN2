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
		~CBoss();

		void LoadBitmap();

		void virtual Initialize();
		void SetXY(int, int);
		void SetCurrentXY(int, int);
		void SetHp(int);
		void SetIsAlive(bool);
		bool GetAlive() { return IsAlive; };
		void OnMove();
		void MoveWithMap(string);
		
		CPoint GetCenterPoint() { return CPoint(x + width / 2, y + height / 2); };

		virtual void OnCycle(CRole*) {};
		virtual void Attack(CRole*) {};

		virtual void Clear() {};

		virtual vector<CScallion*>* GetBullet() { return nullptr; };
		bool IsDead() { return !IsAlive; };

		virtual CTimer* GetAliveTimer() { return nullptr; };

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

		int hp;
		int currentX, currentY;
		int x, y;
		int width, height;
		bool IsAlive = true;
		string id;

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
		void OnMove();
		void Attack(CRole*);
		void Clear();
		void ClearBullet();
		bool IsDead();
		CTimer* GetAliveTimer() { return &AliveTime; };
	private:
		vector<CScallion*>	level4;
		vector<CBlackHole*> blackhole;
		CBlackHole *targetBlackhole;
		void Level4Collision(CRole*);
		const double PI = 4 * atan(1.0);
		CTimer shootLevel4_cd;
		CTimer shoot_atk2_cd;
		CTimer shoot_atk3_cd;

		CTimer moveToGoal;
		CTimer mode_Attack2_timer;
		CTimer mode_Attack4_CreateBlackHole;

		CTimer AliveTime;

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
}