#pragma once
#include "CLibrary.h"
using namespace myLibrary;
namespace game_framework
{
	class CBoss
	{
	public:
		CBoss();
		CBoss(int, int, int, BitmapPath, COLORREF); //x, y, hp, (¸ô®|, ÃC¦â)
		~CBoss();

		void LoadBitmap(BitmapPath, COLORREF);

		void SetXY(int, int);
		void SetHp(int);


	protected:
		CAnimate animation;
		int hp;
		int x, y;
		int width, height;
		bool IsAlive = true;
	};
}