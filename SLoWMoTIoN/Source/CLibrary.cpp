#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "Refactor.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CLibrary.h"
#include "CManager.h"
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
		string file = ".\\" + ziliaojia + "\\" + name;

		if (number == -1)
		{
			file += ".bmp";
		}
		else
		{
			file += "_" + ss.str() + ".bmp";
		}


		address = new char[file.length() + 1];
		strcpy(address, file.c_str());

		return address;
	}

	char * ConvertCharPointToString(string str)
	{
		char* address;
		string file = str;

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
		return ((rect.left <= point.x) && 
			    (point.x <= rect.right) && 
			    (rect.top <= point.y) && 
			    (point.y <= rect.bottom));
	}

	//a 碰到 b
	bool rectCollision(CRect rect1, CRect rect2)
	{
		CPoint leftTop = CPoint(rect1.left, rect1.top);
		CPoint rightTop = CPoint(rect1.right, rect1.top);
		CPoint leftBottom = CPoint(rect1.left, rect1.bottom);
		CPoint rightBottom = CPoint(rect1.right, rect1.bottom);

		return (IsPointInRect(leftTop, rect2) || IsPointInRect(rightTop, rect2) ||
			IsPointInRect(leftBottom, rect2) || IsPointInRect(rightBottom, rect2));
	}

	//a 碰到 b 或 b 碰到 a
	bool IsRectCollision(CRect rect1, CRect rect2)
	{
		return rectCollision(rect1, rect2) || rectCollision(rect2, rect1);
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

	void CAnimate::LoadBitmap(BitmapPath loadpath)
	{
		for (int i = 0; i < loadpath.number; i++)
		{
			char* address = ConvertCharPointToString(loadpath.ziliaojia, loadpath.name, i);
			AddBitmap(address, loadpath.color);
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

	void CAnimate::SetIndex(int _index)
	{
		bmp_index = _index;
	}

	int CAnimate::GetIndex()
	{
		return bmp_index;
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
	void CDialog::Initialize()
	{
		IsTriggered = false;
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

	#pragma region - Camera -
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

	#pragma region - Button -
	CButton::CButton()
	{
		x = y = 0;
	}

	CButton::CButton(const CButton & button)
	{
		*this = button;
	}

	CButton::CButton(BitmapPath _loadpath, CPoint initPos, bool initState)
	{
		//LoadBitmap(_loadpath);
		loadpath = _loadpath;
		Initialize(initPos, initState);
	}

	CButton::~CButton()
	{
	}

	int CButton::GetX()
	{
		return x;
	}

	int CButton::GetY()
	{
		return y;
	}

	void CButton::SetXY(int _x, int _y)
	{
		x = _x;
		y = _y;
		animation.SetTopLeft(x, y);
	}

	void CButton::SetState(bool flag)
	{
		state = flag;
	}

	bool CButton::GetState()
	{
		return state;
	}

	void CButton::LoadBitmap(BitmapPath _loadpath)
	{
		animation.LoadBitmap(_loadpath.ziliaojia, _loadpath.name, _loadpath.number, loadpath.color);
	}

	void CButton::OnMove()
	{
		if (state)	//ON
		{
			animation.SetIndex(1);
		}
		else		//OFF
		{
			animation.SetIndex(0);
		}
		animation.SetTopLeft(x, y);
	}

	void CButton::OnShow()
	{
		animation.OnShow();
	}

	void CButton::Initialize()
	{
		if (animation.IsNull())
		{
			LoadBitmap(loadpath);
		}
		//SetState(true);
		SetXY(x, y);
		animation.SetTopLeft(x, y);
	}

	void CButton::Initialize(CPoint pos, bool flag)
	{
		if (animation.IsNull())
		{
			LoadBitmap(loadpath);
		}
		SetState(flag);
		SetXY(pos.x, pos.y);
	}

	CAnimate * CButton::GetAnimate()
	{
		return &animation;
	}

	void CButton::CollisonMouse(CPoint mouse)
	{
		if (IsPointInRect(mouse, GetAnimate()->GetRect()))
		{
			if (!GetState())					//只有第一次進入Button有音效
				CAudio::Instance()->Play("SOUND_JUMP");
			SetState(true);
		}
		else
			SetState(false);
	}

	void CButton::operator=(const CButton& button)
	{
		state = button.state;
		loadpath = button.loadpath;
		x = button.x;
		y = button.y;
	}
	#pragma endregion

	#pragma region - CInteger
	//CMovingBitmap CInteger::digit[11];

	CInteger::CInteger(int digits)
		: NUMDIGITS(digits)
	{
		isBmpLoaded = false;
	}

	void CInteger::Initialize(CPoint init_pos, int init_num, int init_digitNum)
	{
		SetInteger(init_num, init_digitNum);
		SetTopLeft(init_pos.x, init_pos.y);
		layer.SetLayer(INTERFACE_LAYER);

	}

	void CInteger::Add(int x)
	{
		n += x;
	}

	int CInteger::GetInteger()
	{
		return n;
	}

	void CInteger::LoadBitmap()
	{
		//
		// digit[i]為class varibale，所以必須避免重複LoadBitmap
		//
		if (!isBmpLoaded) {
			int d[11] = { IDB_0,IDB_1,IDB_2,IDB_3,IDB_4,IDB_5,IDB_6,IDB_7,IDB_8,IDB_9,IDB_MINUS };
			for (int i = 0; i < 11; i++)
				digit[i].LoadBitmap(d[i], RGB(0, 0, 0));
			isBmpLoaded = true;
		}
	}

	void CInteger::LoadBitmap(string ziliaojia, string name)
	{
		if (!isBmpLoaded)
		{
			for (int i = 0; i < 11; i++)
			{
				char* address = ConvertCharPointToString(ziliaojia, name, i);
				digit[i].LoadBitmap(address, RGB(255, 255, 255));
				delete address;
			}
			isBmpLoaded = true;
		}

	}

	void CInteger::SetInteger(int i, int digitals_num)
	{
		n = i;
		NUMDIGITS = digitals_num;
	}

	void CInteger::SetTopLeft(int nx, int ny)		// 將動畫的左上角座標移至 (x,y)
	{
		x = nx; y = ny;
	}

	void CInteger::ShowBitmap()
	{
		GAME_ASSERT(isBmpLoaded, "CInteger: 請先執行LoadBitmap，然後才能ShowBitmap");
		int nx;		// 待顯示位數的 x 座標
		int MSB;	// 最左邊(含符號)的位數的數值
		if (n >= 0) {
			MSB = n;
			nx = x + digit[0].Width()*(NUMDIGITS - 1);
		}
		else {
			MSB = -n;
			nx = x + digit[0].Width()*NUMDIGITS;
		}
		for (int i = 0; i < NUMDIGITS; i++) {
			int d = MSB % 10;
			MSB /= 10;
			//number[NUMDIGITS - i - 1] = digit[d];
			//number[NUMDIGITS - i - 1].SetTopLeft(nx, y);
			digit[d].SetTopLeft(nx, y);
			digit[d].ShowBitmap();
			nx -= digit[d].Width();
		}
		if (n < 0) { // 如果小於0，則顯示負號
			
			digit[10].SetTopLeft(nx, y);
			digit[10].ShowBitmap();
		}
	}

	void CInteger::ShowBitmap(double factor)
	{
		GAME_ASSERT(isBmpLoaded, "CInteger: 請先執行LoadBitmap，然後才能ShowBitmap");
		int nx;		// 待顯示位數的 x 座標
		int MSB;	// 最左邊(含符號)的位數的數值
		if (n >= 0) {
			MSB = n;
			nx = x + digit[0].Width()*(NUMDIGITS - 1);
		}
		else {
			MSB = -n;
			nx = x + digit[0].Width()*NUMDIGITS;
		}
		for (int i = 0; i < NUMDIGITS; i++) {
			int d = MSB % 10;
			MSB /= 10;
			digit[d].SetTopLeft(nx, y);
			digit[d].ShowBitmap();
			nx -= digit[d].Width();
		}
		if (n < 0) { // 如果小於0，則顯示負號
			digit[10].SetTopLeft(nx, y);
			digit[10].ShowBitmap(factor);
		}
	}

	#pragma endregion

	#pragma region - CAction -
	CAction::CAction()
	{
		x = y = 0;
		SetValid(true);
		double wiatTime = 0.1;
		delayTimer = CTimer(wiatTime);
	}

	void CAction::OnMove(string _nowAction)
	{

		delayTimer.CountDown();

		if (!delayTimer.IsTimeOut())
		{
			return;
		}
		else
		{
			delayTimer.ResetTime();
		}
		
		if (action != _nowAction)	//切換動作
		{
			action_index = 0;
			action = _nowAction;
		}
		else
		{
			//現在動作 + 方向，從map取出對應 Action Vector
			nowAction = &(paser[action + faceTo]);

			if (action_index < (int)(nowAction->size() - 1))
			{
				action_index++;
			}
			else
			{
				action_index = 0;
			}
		}

		nowBitmap = &((paser[_nowAction + faceTo])[action_index]);
		nowBitmap->SetTopLeft(x, y);
		//(nowAction->begin() + action_index)->SetTopLeft(x, y);


		//狗才用iterator
		/*vector<CMovingBitmap>::iterator bmp_iter = nowAction->begin() + action_index;
		bmp_iter->SetTopLeft(x, y);
		bmp_iter->ShowBitmap();*/
	}

	void CAction::OnShow()
	{
		//(nowAction->begin() + action_index)->ShowBitmap();
		nowBitmap->ShowBitmap();
	}

	void CAction::SetAction(string _action)
	{
		action = _action;
	}

	string CAction::GetAction()
	{
		return action;
	}

	void CAction::Initialize()
	{
		nowAction = NULL;
		//nowAction->clear();

		nowBitmap = NULL;
		
		action = "idle";
		faceTo = "_R";
		action_index = 0;
		if (!paser.empty())
		{
			nowAction = &(paser[action + faceTo]);
			nowBitmap = &((paser[action + faceTo])[0]);
		}
	}

	//ex : LoadAction("idle", BitmapPath("..\\RES\\miku\\idle", "idle", 19));
	void CAction::LoadAction(string _action, BitmapPath loadpath)
	{
		#pragma region Load action
		vector<CMovingBitmap> vector_R(loadpath.number);
		vector<CMovingBitmap> vector_L(loadpath.number);

		for (int i = 0; i < loadpath.number; i++)
		{
			char* address_R = ConvertCharPointToString(loadpath.ziliaojia + "\\R", loadpath.name, i);
			char* address_L = ConvertCharPointToString(loadpath.ziliaojia + "\\L", loadpath.name + "_L", i);

			vector_R[i].LoadBitmap(address_R, loadpath.color);
			vector_L[i].LoadBitmap(address_L, loadpath.color);

			delete address_R;
			delete address_L;
		}
		paser[_action + "_R"] = vector_R;
		paser[_action + "_L"] = vector_L;
		#pragma endregion

		#pragma region fuck yor *>*
		/*vector<CMovingBitmap*> vector_R(loadpath.number);
		vector<CMovingBitmap*> vector_L(loadpath.number);

		for (int i = 0; i < loadpath.number; i++)
		{

			char* address_R = ConvertCharPointToString(loadpath.ziliaojia + "\\R", loadpath.name, i);
			char* address_L = ConvertCharPointToString(loadpath.ziliaojia + "\\L", loadpath.name + "_L", i);

			vector_R[i] = new CMovingBitmap();
			vector_L[i] = new CMovingBitmap();

			vector_R[i]->LoadBitmap(address_R, loadpath.color);
			vector_L[i]->LoadBitmap(address_L, loadpath.color);;

			delete address_R;
			delete address_L;
		}
		paser[_action + "_R"] = vector_R;
		paser[_action + "_L"] = vector_L;*/
		#pragma endregion

		
	}

	int CAction::Height()
	{
		if (nowAction != NULL)
		{
			return 0;
		}

		return (nowAction->begin())->Height();
	}

	int CAction::Width()
	{
		if (nowAction != NULL)
		{
			return 0;
		}

		return (nowAction->begin())->Width();
	}

	int CAction::Left()
	{

		if (nowAction != NULL)
		{
			return 0;
		}

		return x;
	}

	int CAction::Top()
	{
		if (nowAction != NULL)
		{
			return 0;
		}

		return y;
	}

	void CAction::SetTopLeft(int _x, int _y)
	{
		x = _x;
		y = _y;
		nowBitmap->SetTopLeft(x, y);
	}

	bool CAction::IsNull()
	{
		return (nowAction == NULL || nowAction->size() <= 0);
	}

	void CAction::SetValid(bool _valid)
	{
		isValid = _valid;
	}

	bool CAction::GetValid()
	{
		return isValid;
	}

	void CAction::SetIndex(int _index)
	{
		action_index = _index;
	}

	int CAction::GetIndex()
	{
		return action_index;
	}

	void CAction::SetFaceTo(string _faceTo)
	{
		faceTo = _faceTo;
	}

	string CAction::GetFaceTo()
	{
		return faceTo;
	}

	CMovingBitmap* CAction::GetNowBitmap()
	{
		return nowBitmap;
	}
	#pragma endregion

	#pragma region - rmtgjowi -
	CEnd::CEnd()
	{
		endName = "";
	}

	CEnd::CEnd(string _endName)
	{
		endName = _endName;
		LoadEnd();
	}

	CEnd::~CEnd()
	{
	}
	string CEnd::GetBmpPath(int index)
	{
		if (index < 0 || index >= (int)bmpPath.size())
		{
			return END_EOF;
		}
		else
		{
			return bmpPath[index];
		}
	}
	void CEnd::LoadEnd()
	{
		if (endName == END_NAME_WINXINGTING)
		{

		}
		else if (endName == END_NAME_LOSEXINGTING)
		{
			bmpPath.push_back("RES\\End\\LoseXingting\\bmp\\WinXingting_0.bmp");
			bmpPath.push_back("RES\\End\\LoseXingting\\bmp\\WinXingting_1.bmp");
			bmpPath.push_back("RES\\End\\LoseXingting\\bmp\\WinXingting_2.bmp");
		}
	}
	#pragma endregion

	#pragma region - CToumeiImage -
	CToumeiImage::CToumeiImage()
	{
		SetAlpha(0);
	}

	CToumeiImage::~CToumeiImage()
	{
	}
	void CToumeiImage::SetAlpha(int _a)
	{
		alpha = _a;

		if (alpha <= 0)
			alpha = 0;
		else if (alpha >= 255)
			alpha = 255;
	}

	void CToumeiImage::FadeIn()
	{
		SetAlpha(alpha + dFadeInValue);
	}

	void CToumeiImage::FadeOut()
	{
		SetAlpha(alpha + dFadeInValue);
	}
	#pragma endregion
}