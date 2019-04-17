#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "Refactor.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CLibrary.h"
//以下include安心的
#include "game.h"
#include "MainFrm.h"
#include <string.h>
#include "mygame.h"
#include <time.h>
#include <fstream>
#include <sstream>

namespace myLibrary
{
	char* ConvertCharPointToString(string ziliaojia, string name, int number)
	{
		char* address;
		std::stringstream ss;
		ss << number;
		string file = ".\\" + ziliaojia + "\\" + name + "_" + ss.str() + ".bmp";

		address = new char[file.length() + 1];
		strcpy(address, file.c_str());

		return address;
	}

	void DeleteCharPoint(vector<char*> &addresses)
	{
		for (vector<char*>::iterator it = addresses.begin(); it != addresses.end(); it++)
		{
			if (NULL != *it)
			{
				delete *it;
				*it = NULL;
			}
		}
		addresses.clear();
	}

	int GetPostive(int k)
	{
		return k > 0 ? k : -k;
	}

	int GetRandom(int minNumber, int maxNumber)
	{
		//srand(time(NULL));
		int random = rand() % (maxNumber - minNumber + 1) + minNumber;

		return random;
	}

}
using namespace myLibrary;
namespace game_framework
{
	#pragma region - Function -
	int ScreenX(int mapx, int rolex)
	{
		return GetPostive(mapx) + rolex;
	}

	bool IsPointInRect(CPoint point, CRect rect)
	{
		return (rect.left <= point.x && point.x <= rect.right && rect.top <= point.y && rect.bottom);
	}

	bool IsRectCollision(CRect rect1, CRect rect2)
	{
		CPoint leftTop = CPoint(rect1.left, rect1.top);
		CPoint rightTop = CPoint(rect1.right, rect1.top);
		CPoint leftBottom = CPoint(rect1.left, rect1.bottom);
		CPoint rightBottom = CPoint(rect1.right, rect1.bottom);
		return (IsPointInRect(leftTop, rect2) || IsPointInRect(rightTop, rect2) ||
			IsPointInRect(leftBottom, rect2) || IsPointInRect(rightBottom, rect2));
	}

	bool IsRectInRect(CRect rect1, CRect rect2) //小 in 大
	{
		CPoint leftTop = CPoint(rect1.left, rect1.top);
		CPoint rightTop = CPoint(rect1.right, rect1.top);
		CPoint leftBottom = CPoint(rect1.left, rect1.bottom);
		CPoint rightBottom = CPoint(rect1.right, rect1.bottom);

		return (IsPointInRect(leftTop, rect2) && IsPointInRect(rightTop, rect2) &&
			IsPointInRect(leftBottom, rect2) && IsPointInRect(rightBottom, rect2));
	}
	#pragma endregion

	#pragma region - CAnimate -
	CAnimate::CAnimate()
	{
		x = y = bmp_index = bmp_amount = 0;
		SetValid(true);
		double wiatTime = (double)GetRandom(2, 4) / 5.0;
		delayTimer = CTimer(wiatTime);
		bmp.clear();
	}

	CAnimate::~CAnimate()
	{
		ReleaseAnimate();
	}

	void CAnimate::OnMove()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");

		
		if (delayTimer.IsTimeOut())
		{
			delayTimer.ResetTime();

			if (bmp_index < (int)bmp.size() - 1)
			{
				bmp_index++;
			}
			else
			{
				bmp_index = 0;
			}
		}
		else
		{
			delayTimer.CountDown();
		}
		//bitmaps[bmp_index]->SetTopLeft(x, y);
		bmp[bmp_index].SetTopLeft(x, y);
	}

	void CAnimate::OnMove(int dir)
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		int upperLimit = (bmp_amount - 1) / 4;
		if (dir != 0)	//Move
		{
			if (bmp_index >= (dir - 1) * upperLimit + 1 && bmp_index < upperLimit * dir)
			{
				bmp_index++;
			}
			else
				bmp_index = (dir - 1) * upperLimit + 1;
		}
		else //static
		{
			bmp_index = 0;
		}
		// dir = 1 , 1  - 3		(dir - 1) * upperLimit + 1	/ upperLimit * dir - (upperLimit - 1)
		// dir = 2 , 4  - 6
		// dir = 3 , 7  - 9
		// dir = 4 , 10 - 12

		//bitmaps[bmp_index]->SetTopLeft(x, y);
		//bmp_index %= bmp.size();
		//if (dir == 1) //Top
		//{
		//	if (bmp_index >= 1 && bmp_index < 2)
		//		bmp_index++;
		//	else
		//		bmp_index = 1;
		//}
		//else if (dir == 2) //Right
		//{
		//	if (bmp_index >= 4 && bmp_index < 6)
		//		bmp_index++;
		//	else
		//		bmp_index = 4;
		//}
		//else if (dir == 3) // Down
		//{
		//	if (bmp_index >= 7 && bmp_index < 9)
		//		bmp_index++;
		//	else
		//		bmp_index = 7;
		//}
		//else if (dir == 4) // Left
		//{
		//	if (bmp_index >= 10 && bmp_index < 12)
		//		bmp_index++;
		//	else
		//		bmp_index = 10;
		//}
		bmp[bmp_index].SetTopLeft(x, y);
	}

	void CAnimate::LoadBitmap(vector<char*> bmps, COLORREF colorkey)
	{
		for (vector<char*>::iterator bitmap = bmps.begin(); bitmap != bmps.end(); bitmap++)
		{
			//bitmaps.push_back(AddBitmap(*(bitmap), colorkey));
			AddBitmap(*(bitmap), colorkey);
		}
	}

	void CAnimate::LoadBitmap(string ziliaojia, string name, int number, COLORREF transparentColor)
	{
		for (int i = 0; i < number; i++)
		{
			char* address = ConvertCharPointToString(ziliaojia, name, i);
			AddBitmap(address, transparentColor);
			delete address;
		}
	}

	CMovingBitmap* CAnimate::AddBitmap(int IDB_BITMAP, COLORREF colorkey)
	{
		CMovingBitmap add_bmp;
		add_bmp.LoadBitmap(IDB_BITMAP, colorkey);
		bmp.push_back(add_bmp);
		bmp_amount++;
		//Reset();
		return &(bmp[bmp.size() - 1]);

	}

	CMovingBitmap* CAnimate::AddBitmap(char *filename, COLORREF colorkey)
	{
		CMovingBitmap add_bmp;
		add_bmp.LoadBitmap(filename, colorkey);
		bmp.push_back(add_bmp);
		bmp_amount++;
		//Reset();
		return &(bmp[bmp.size() - 1]);
	}

	void CAnimate::Reset()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
	}

	void CAnimate::SetTopLeft(int nx, int ny)
	{
		x = nx, y = ny;
		//bitmaps[bmp_index]->SetTopLeft(x, y);
		bmp[bmp_index].SetTopLeft(x, y);

	}

	void CAnimate::OnShow()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded before they are shown.");
		//bitmaps[bmp_index]->ShowBitmap();
		bmp[bmp_index].ShowBitmap();
	}

	int CAnimate::Top()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return y;
	}

	int CAnimate::Left()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return x;
	}

	int CAnimate::Height()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		//return bitmaps[bmp_index]->Height();
		return bmp[bmp_index].Height();

	}

	int CAnimate::Width()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		//return bitmaps[bmp_index]->Width();
		return bmp[bmp_index].Width();

	}

	bool CAnimate::IsNull()
	{
		return bmp.size() == 0;
	}

	void CAnimate::ReleaseAnimate()
	{
		bmp.clear();
		vector<CMovingBitmap>().swap(bmp);
	}

	void CAnimate::SetValid(bool flag)
	{
		isValid = flag;
	}

	bool CAnimate::GetValid()
	{
		return isValid;
	}

	void CAnimate::ResetDelayTime(double _time)
	{
		delayTimer.ResetTime(_time);
	}

	CRect CAnimate::GetRect()
	{
		return bmp[bmp_index].GetRect();
	}
#pragma endregion

	#pragma region - CLayer -
	CLayer::CLayer()
	{
		layer = 0;
	}

	CLayer::~CLayer()
	{
	}

	int CLayer::GetLayer()
	{
		return layer;
	}

	void CLayer::SetLayer(int _layer)
	{
		if (_layer >= MAX_LAYER_NUMBER)
			_layer = MAX_LAYER_NUMBER - 1;
		else if (_layer < 0)
			_layer = 0;
		layer = _layer;
	}
	#pragma endregion

	#pragma region - timer -

	CTimer::CTimer() //default constructor
	{
		ResetTime(99.0);
	}

	CTimer::CTimer(int _time)
	{
		ResetTime(_time);
		time = (int)time;
	}

	CTimer::CTimer(double _time) //給予初始時間
	{
		ResetTime(_time);
	}

	CTimer::~CTimer()
	{

	}

	void CTimer::CountDown()
	{
		time--;
	}

	int CTimer::GetTime(int k)
	{
		return (int)time / (int)reflash;
	}

	double CTimer::GetTime()
	{
		return time / reflash;
	}

	bool CTimer::IsTimeOut()
	{
		return time <= 0.00;
	}

	void CTimer::ResetTime(double _resetTime)
	{
		time = initTime = _resetTime * reflash;
	}

	void CTimer::ResetTime()
	{
		time = initTime;
	}

	#pragma region -- 窩4絕ㄉni最好4bu咬打開ㄊ拉 --
	void CTimer::operator=(CTimer _timer)
	{
		time = _timer.time;
		initTime = _timer.time;
	}
	#pragma endregion

	#pragma endregion

	#pragma region - Dialog -
	CDialog::CDialog()
	{
	}

	CDialog::CDialog(string txtPath, string _txt, bool _CanReTrigger)
	{
		path = txtPath;
		mode = _txt;
		IsTriggered = false;
		LoadTxt(); // load txt

		CanReTrigger = _CanReTrigger;
	}

	CDialog::~CDialog()
	{
	}
	bool CDialog::GetTriggered()
	{
		return IsTriggered;
	}
	void CDialog::SetTriggered()
	{
		IsTriggered = !CanReTrigger;
	}
	string CDialog::GetAvatar(unsigned int step)
	{
		if (step <= avatar.size())
			return avatar[step];
		else
			return "RREF";
	}
	string CDialog::GetDialogTxt(unsigned int step)
	{
		if (step <= txt.size())
			return txt[step];
		else
			return "RREF";
	}
	int CDialog::GetTxtSize()
	{
		return txt.size();
	}
	string CDialog::GetMode()
	{
		return mode;
	}

	void CDialog::LoadTxt()
	{
		fstream dialogTxt; //一個txt的文本資料
		dialogTxt.open(path); //open txt
		
		int index = 0;
		string dialogData; //文本資料的一行 (index = 偶數 > 儲存avatar, index = 奇數 > 儲存對話文字)
		while (dialogTxt >> dialogData)
		{
			if (!(index & 1)) //even
			{
				avatar.push_back(dialogData);
			}
			else //odd
			{
				txt.push_back(dialogData);
			}
			index++;
		}
	}

	#pragma endregion

	#pragma region Camera
	CCamera CCamera::camera;

	CCamera::CCamera()
	{
		Initialize();
	}

	CCamera::~CCamera()
	{
	}

	void CCamera::Initialize()
	{
		x = y = 0;
		canMoving = true;
	}

	void CCamera::Reset()
	{
		x = y = 0;
		canMoving = true;
	}

	void CCamera::SetCameraBoundary(int _left, int _right)
	{
		max_left = _left;
		max_right = _right;
	}

	CCamera * CCamera::Instance()
	{
		return &camera;
	}

	int CCamera::GetX()
	{
		return x;
	}

	int CCamera::GetY()
	{
		return y;
	}

	void CCamera::SetXY(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	void CCamera::AddX(int _dx)
	{
		if(canMoving)
			x += _dx;
	}

	void CCamera::AddY(int _dy)
	{
		if (canMoving)
			y += _dy;
	}

	void CCamera::SetCanMoving(bool flag)
	{
		canMoving = flag;
	}

	#pragma endregion
}