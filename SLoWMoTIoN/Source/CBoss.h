#pragma once
#include "CLibrary.h"
#include "Refactor.h"
using namespace myLibrary;
namespace game_framework
{
	class CBoss
	{
	public:
		CBoss();
		CBoss(int, int, int, BitmapPath, COLORREF); //x, y, hp, (¸ô®|, ÃC¦â)
		~CBoss();

		void LoadBitmap();

		void Initialize();
		void SetXY(int, int);
		void SetHp(int);
		void SetIsAlive(bool);
		
		void MoveWithMap(string);
		CLayer layer;
		
		CAnimate* GetAnimate();

	protected:
		#pragma region Init Information
		int initx, inity, initHp;
		BitmapPath loadPath;
		COLORREF transparentColor;
		#pragma endregion
		
		CAnimate animation;
		
		int hp;
		int x, y;
		int width, height;
		bool IsAlive = true;
	};
}