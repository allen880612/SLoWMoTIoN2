#pragma once
#include "CLibrary.h"
#include "Refactor.h"
#include <vector>
#include "CBall.h"
using namespace myLibrary;
namespace game_framework
{
	#pragma region - CBoss -
	class CBoss
	{
	public:
		CBoss();
		CBoss(int, int, int, BitmapPath, COLORREF); //x, y, hp, (¸ô®|, ÃC¦â)
		~CBoss();

		void LoadBitmap();

		void Initialize();
		void SetXY(int, int);
		void SetCurrentXY(int, int);
		void SetHp(int);
		void SetIsAlive(bool);
		void OnMove();
		void MoveWithMap(string);


		virtual void Attack1() { };

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
		void Attack1();
	private:
		vector<CScallion*>	level4;
		CTimer shootLevel4_cd;
	};
	#pragma endregion
}