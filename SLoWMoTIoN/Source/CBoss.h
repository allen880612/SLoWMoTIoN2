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
	class CBoss
	{
	public:
		CBoss();
		CBoss(int, int, int, BitmapPath, COLORREF); //x, y, hp, (路徑, 顏色)
		~CBoss();

		void LoadBitmap();

		void virtual Initialize();
		void SetXY(int, int);
		void SetCurrentXY(int, int);
		void SetHp(int);
		void SetIsAlive(bool);
		void OnMove();
		void MoveWithMap(string);
		
		CPoint GetCenterPoint() { return CPoint(x + width / 2, y + height / 2); };

		virtual void OnCycle(CRole*) {};
		virtual void Attack(CRole*) {};

		virtual void ClearBullet() {};

		virtual vector<CScallion*>* GetBullet() { return nullptr; };

		CLayer layer;

		CAnimate* GetAnimate();

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

	};
	#pragma endregion

	#pragma region - CXingting -
	class CXingting : public CBoss
	{
	public:
		CXingting();
		CXingting(int, int, int, BitmapPath, COLORREF);
		~CXingting();
		void Initialize();
		void OnCycle(CRole*);
		void OnMove();
		void Attack(CRole*);
		void ClearBullet();
	private:
		vector<CScallion*>	level4;
		void Level4Collision(CRole*);
		const double PI = 4 * atan(1.0);
		CTimer shootLevel4_cd;
		CTimer shoot_atk2_cd;
		CTimer moveToGoal;

		int goal_x, goal_y;

		void Attack1();
		void Attack2();

		bool mode_Attack1;
		bool mode_Attack2;

		int atkCounter;
	};
	#pragma endregion
}