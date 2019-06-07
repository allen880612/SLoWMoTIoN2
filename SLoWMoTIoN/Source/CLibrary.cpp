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
#include "dirent.h"
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

	COLORREF ConvertStringToColor(string _color)
	{
		if (_color == "red")
			return RGB(255, 0, 0);
		else if (_color == "green")
			return RGB(0, 255, 0);
		else if (_color == "blue")
			return RGB(0, 0, 255);
		else if (_color == "white")
			return RGB(255, 255, 255);
		else if (_color == "black")
			return RGB(0, 0, 0);

		else if (_color == "txtColor")
			return DIALOG_TEXT_COLOR;
		return RGB(255, 255, 255); //white
	}

	bool ConvertStringToBoolen(string str)
	{
		return str == "true";
	}

	void getFolderFile(string folderPath, vector<string> *file) //得到資料夾下的所有檔名
	{
		DIR *fp; // create folder point
		fp = opendir(folderPath.c_str());
		struct dirent *folderp;
		int k = 0;
		while ((folderp = readdir(fp)) != NULL)
		{
			if (k >= 2)
			{
				string newPath = string(folderp->d_name);
				file->push_back(newPath);
			}
			k++;
		}
		closedir(fp);
	}

	int getFolerFileNumber(string folderPath)
	{
		DIR *fp; // create folder point
		fp = opendir(folderPath.c_str());
		struct dirent *folderp;
		int k = 0;
		while ((folderp = readdir(fp)) != NULL)
		{
			k++;
		}
		closedir(fp);
		return k - 2;
	}

	string getFileName(string file)
	{
		return file.substr(0, file.length() - 4);
	}

	vector<string> SplitString(string str)
	{
			stringstream ss;
			#pragma region - init stringstream -
			ss.str("");
			ss.clear();
			#pragma endregion
			ss << str;
			vector<string> lineInfo;
			#pragma region - split string -
			while (ss)
			{
				string templine;
				ss >> templine;
				if (templine != "")
				{
					lineInfo.push_back(templine);
				}
			}
			return lineInfo;
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

	int ConvertStringToInt(string str)
	{
		stringstream ss;
		int cs;
		ss << str;
		ss >> cs;
		return cs;
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

	void CAnimate::LoadBitmap(vector<string> bmps, COLORREF colorkey)
	{
		for (unsigned int i = 0; i < bmps.size(); i++)
		{
			//bitmaps.push_back(AddBitmap(*(bitmap), colorkey));
			char *address = ConvertCharPointToString(bmps[i]);
			AddBitmap(address, colorkey);
			delete address;
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
		if (_index < (int)bmp.size())
		{
			bmp_index = _index;
		}
		else
		{
			bmp_index = 0;
		}
	}

	int CAnimate::GetIndex()
	{
		return bmp_index;
	}

	void CAnimate::CopyAnimateInformation(CAnimate *copyAnimate)
	{
		SetValid(copyAnimate->GetValid());
		SetTopLeft(copyAnimate->Top(), copyAnimate->Left());
		SetIndex(copyAnimate->GetIndex());
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

	#pragma region - CDialog -
	CDialog::CDialog()
	{
	}

	CDialog::CDialog(string txtPath, string _txt)
	{
		path = txtPath;
		mode = _txt;
		IsTriggered = false;
		LoadTxt(); // load txt
	}

	//CDialog::CDialog(string txtPath, string _txt, bool _CanReTrigger)
	//{
	//	path = txtPath;
	//	mode = _txt;
	//	IsTriggered = false;
	//	LoadTxt(); // load txt

	//	CanReTrigger = _CanReTrigger;
	//}

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
		if (step < avatar.size())
			return avatar[step];
		else
			return "RREF";
	}
	string CDialog::GetDialogTxt(unsigned int step)
	{
		if (step < txt.size())
			return txt[step];
		else
			return "RREF";
	}
	COLORREF CDialog::GetDialogColor(unsigned int step)
	{
		if (step < color.size())
			return color[step];

		return ConvertStringToColor("txtColor");
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
		string dialogData; //文本資料的一行 

		getline(dialogTxt, dialogData); //先讀取一行 "初始設定"
		CanReTrigger = ConvertStringToBoolen(dialogData); //設定對話可否重複觸發

		while (getline(dialogTxt, dialogData)) //get line
		{
			#pragma region - get Split stirng -
			vector<string> lineInfo = SplitString(dialogData);
			#pragma endregion

			#pragma region - save txt information -
			COLORREF txtColor = ConvertStringToColor(lineInfo.size() >= 3 ? lineInfo[2] : "txtColor"); //get print txt color
			avatar.push_back(lineInfo[0]);
			txt.push_back(lineInfo[1]);
			color.push_back(txtColor);
			#pragma endregion

		}

		dialogTxt.close();
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
		name = "none";
		state = true;
	}

	CButton::CButton(const CButton & button)
	{
		*this = button;
		name = button.name;
	}

	CButton::CButton(BitmapPath _loadpath, CPoint initPos, bool initState, bool _needCollision)
	{
		//LoadBitmap(_loadpath);
		loadpath = _loadpath;
		name = _loadpath.name;
		needCollision = _needCollision;
		x = initPos.x;
		y = initPos.y;
		SetState(initState);
		//Initialize(initPos, initState);
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

	int CButton::Width()
	{
		return animation.Width();
	}

	int CButton::Height()
	{
		return animation.Height();
	}

	void CButton::LoadBitmap()
	{
		if (animation.IsNull())
		{
			if(loadpath.path.empty())
				animation.LoadBitmap(loadpath.ziliaojia, loadpath.name, loadpath.number, loadpath.color);
			else
				animation.LoadBitmap(loadpath.path, loadpath.color);
		}
	}

	void CButton::LoadBitmap(BitmapPath _loadpath)
	{
		if (loadpath.path.empty())
			animation.LoadBitmap(_loadpath.ziliaojia, _loadpath.name, _loadpath.number, _loadpath.color);
		else
			animation.LoadBitmap(_loadpath.path, _loadpath.color);
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
			LoadBitmap(loadpath);

		SetValid(true);
		SetState(state);
		SetXY(x, y);

		animation.SetTopLeft(x, y);
		CLayerManager::Instance()->AddObject(&animation, INTERFACE_LAYER);
	}

	void CButton::Initialize(CPoint pos, bool flag)
	{
		if (animation.IsNull())
			LoadBitmap(loadpath);
		
		SetValid(true);
		SetState(flag);
		SetXY(pos.x, pos.y);

		animation.SetTopLeft(x, y);
		//CLayerManager::Instance()->AddObject(&animation, INTERFACE_LAYER); //先註解掉，目前這句看起來沒差
	}

	void CButton::SetValid(bool _flag)
	{
		animation.SetValid(_flag);
	}

	bool CButton::GetValid()
	{
		return animation.GetValid();
	}

	CAnimate * CButton::GetAnimate()
	{
		return &animation;
	}

	void CButton::CollisonMouse(CPoint _m)
	{
		if (!needCollision)
			return;
		
		if (IsCollisionMouse(_m))
		{
			if (!GetState())					//只有第一次進入Button有音效
				CAudio::Instance()->Play("jump");
			SetState(true);
		}
		else
			SetState(false);
	}

	void CButton::ClickButton()
	{
		SetState(!GetState());
	}


	bool CButton::IsCollisionMouse(CPoint _m)
	{
		return IsPointInRect(_m, animation.GetRect());
	}

	void CButton::operator=(const CButton& button)
	{
		loadpath = button.loadpath;
		name = button.loadpath.name;
		needCollision = button.needCollision;
		Initialize(CPoint(button.x, button.y), button.state);
		
	}
	#pragma endregion

	#pragma region - CInteger -
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

		delayAdapter["idle"] = delay_idle;
		delayAdapter["run"] = delay_run;
		delayAdapter["jump"] = delay_jump;

		nowAction = NULL;
		nowAction = NULL;
		double waitTime = delay_idle;
		delayTimer = CTimer(waitTime);
	}

	void CAction::OnMove(string _nowAction)
	{
		delayTimer.CountDown();

		if (action != _nowAction)	//切換動作
		{
			delayTimer.ResetTime(delayAdapter[_nowAction]);
			SetAction(_nowAction);
		}

		if (!delayTimer.IsTimeOut())	//延遲
		{
			return;
		}
		else
		{
			delayTimer.ResetTime();
		}
		
		
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
		action_index = 0;
		action = _action;
		nowAction = &(paser[action + faceTo]);
		nowBitmap = &((paser[_action + faceTo])[action_index]);
		nowBitmap->SetTopLeft(x, y);
	}

	string CAction::GetAction()
	{
		return action;
	}

	void CAction::Initialize()
	{
		nowAction = NULL;
		nowBitmap = NULL;
		
		action = "jump";
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
		if (nowBitmap == NULL)
		{
			return 0;
		}

		return nowBitmap->Height();
	}

	int CAction::Width()
	{
		if (nowBitmap == NULL)
		{
			return 0;
		}

		return nowBitmap->Width();
	}

	int CAction::Left()
	{

		if (nowBitmap == NULL)
		{
			return 0;
		}

		return x;
	}

	int CAction::Top()
	{
		if (nowBitmap == NULL)
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
		return (nowAction == NULL || nowBitmap == NULL || nowAction->size() <= 0);
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

	CRect CAction::GetRect()
	{
		return nowBitmap->GetRect();
	}

	int CAction::GetActionHeight(string _action)
	{
		if (paser.count(_action) && !paser[_action].empty())
		{
			return paser[_action].begin()->Height();
		}
		return 108;
	}
	#pragma endregion

	#pragma region - CEnd -
	CEnd::CEnd()
	{
		isGet = false;
		endName = "";
	}

	CEnd::CEnd(string _endName)
	{
		endName = _endName;
		isGet = false;
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
	string CEnd::GetTxt(int index)
	{
		if (index < 0 || index >= (int)txt.size())
		{
			return END_EOF;
		}
		else
		{
			return txt[index];
		}
	}
	void CEnd::LoadEnd()
	{
		LoadBmpTxt(endName);
	}

	void CEnd::LoadBmpTxt(string _endName)
	{
		string loadbmpFolderPath = "RES\\End\\" + _endName + "\\bmp\\";
		string loadtxtFolderPath = "RES\\End\\" + _endName + "\\txt\\";
		
		getFolderFile(loadbmpFolderPath, &bmpPath);

		getFolderFile(loadtxtFolderPath, &txt);

		for (unsigned int i = 0; i < bmpPath.size(); i++)
		{
			bmpPath[i] = loadbmpFolderPath + bmpPath[i];
		}

		for (unsigned int i = 0; i < txt.size(); i++)
		{
			CDialogManager::Instance()->LoadDialog(txt[i], loadtxtFolderPath + txt[i]);
		}
	}

	void CEnd::LoadBmpTxt(string _endName, int _bmpAmount, int _txtAmount)
	{
		string loadbmpPath = "RES\\End\\" + _endName + "\\bmp\\" + _endName + "_"; // tail is 0.bmp
		string loadtxtPath = "RES\\End\\" + _endName + "\\txt\\" + _endName + "_";

		for (int i = 0; i < _bmpAmount; i++)
		{
			stringstream ss;
			ss << i;
			string _bmpath = loadbmpPath + ss.str() + ".bmp";
			bmpPath.push_back(_bmpath);
		}

		//txt 在 CDialog (map) 中的 index (string) 格式: 
		//ex: WinXingting_0
		//ex: WinXingting_1
		//ex: WinXingting_2
		for (int i = 0; i < _txtAmount; i++)
		{
			stringstream ss;
			ss << i;
			string _txtpath = loadtxtPath + ss.str() + ".txt";
			CDialogManager::Instance()->LoadDialog(endName + "_" + ss.str(), _txtpath);
			txt.push_back(endName + "_" + ss.str());
		}

	}
	#pragma endregion

	#pragma region - CToumeiImage -
	CToumeiImage::CToumeiImage()
	{
		SetAlpha(0);
		dFadeInValue = 3;
		dFadeOutValue = -4;
	}

	CToumeiImage::CToumeiImage(int _dFadeInValue, int _dFadeOutValue)
	{
		SetAlpha(0);
		SetFadeInOut(_dFadeInValue, _dFadeOutValue);
	}

	CToumeiImage::CToumeiImage(string _loadbmpPath, int _dFadeInValue, int _dFadeOutValue)
	{
		CToumeiImage(_dFadeInValue, _dFadeOutValue);
		bmpLoadPath = _loadbmpPath;
		SetBmp(_loadbmpPath);
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

	void CToumeiImage::SetBmp(string _path)
	{
		//char *address = ConvertCharPointToString(_path);
		bmp.DeleteObject();
		bmp.m_hObject = LoadImage(NULL, _path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		//delete address;
	}

	void CToumeiImage::SetFadeInOut(int dFIV, int dFOV)
	{
		dFadeInValue = dFIV;
		dFadeOutValue = dFOV;
	}

	void CToumeiImage::FadeIn()
	{
		SetAlpha(alpha + dFadeInValue);
	}

	void CToumeiImage::FadeOut()
	{
		SetAlpha(alpha + dFadeOutValue);
	}

	void CToumeiImage::DrawImage()
	{
		DrawBitmap(&bmp, alpha);
	}
	#pragma endregion

	#pragma region - CWindows -
	
	#pragma region windows
	CWindows::CWindows()
	{
		x = y = 0;
		isLoaded = false;
		isOpen = false;
		closeButton = new CButton(BitmapPath("RES\\Button", "close", 2, RGB(214, 214, 214)), CPoint(0, 0), false, true);
	}

	CWindows::CWindows(CPoint _p)
	{
		SetXY(_p);
		isLoaded = false;
		isOpen = false;
		//closeButton = CButton(BitmapPath("RES\\Button", "close", 2, RGB(214, 214, 214)), CPoint(0, 0), false, true);

	}

	CWindows::~CWindows()
	{
		Clear();
	}

	void CWindows::LoadResource()
	{
		//closeButton = new CButton(BitmapPath("RES\\Button", "close", 2, RGB(214, 214, 214)), CPoint(0, 0), false, true);
		closeButton->LoadBitmap();
		background.LoadBitmap("RES\\Windows", "EndingWindows", RGB(214, 214, 214));
	}

	void CWindows::Initialize(CPoint _initP)
	{

		SetXY(_initP);

		const int CLSBTN_INIT_X = x + background.Width() - closeButton->Width() - 10;
		const int CLSBTN_INIT_Y = y + 10;

		closeButton->Initialize(CPoint(CLSBTN_INIT_X, CLSBTN_INIT_Y), false);
		background.SetTopLeft(x, y);
		background.SetValid(false);
		closeButton->SetValid(false);
	}

	void CWindows::Clear()
	{
		delete closeButton;
	}

	void CWindows::Open()
	{
		if (!isOpen)
			isOpen = true;
	}

	void CWindows::Close()
	{
		if (isOpen)
			isOpen = false;
	}

	bool CWindows::IsCollisionClose(CPoint _m)
	{
		return closeButton->IsCollisionMouse(_m);
		//return closeButton.IsCollisionMouse(_m) & isOpen;
	}

	bool CWindows::IsOpen()
	{
		return isOpen;
	}

	void CWindows::SetCloseButton(CPoint _p)
	{
		closeButton->SetXY(_p.x, _p.y);
		//closeButton.SetXY(_p.x, _p.y);
	}

	void CWindows::SetXY(CPoint _p)
	{
		x = _p.x;
		y = _p.y;
	}

	void CWindows::CollisionClose(CPoint _p)
	{
		closeButton->CollisonMouse(_p);
	}

	void CWindows::OnCycle()
	{
		if (!IsOpen())
		{
			//background.SetValid(false);
			//closeButton->SetValid(false);
			return;
		}

		closeButton->OnMove();
		//closeButton.OnMove();
		background.SetTopLeft(x, y);

		//background.SetValid(false);
		//closeButton->SetValid(false);
		////closeButton.SetValid(false);
	}

	void CWindows::OnShow()
	{
		if (!IsOpen())
		{
			return;
		}

		background.ShowBitmap();
		closeButton->OnShow();
		//closeButton.OnShow();
	}
	#pragma endregion

	#pragma region scroll windows

	CScrollWindows::CScrollWindows():CWindows()
	{
		rowNum = 2;
		colNum = 4;
		#pragma region - Create endName -
		fstream haveEnd;
		haveEnd.open("RES\\End\\HaveEnd.txt", ios::in);
		string lineData;
		while (getline(haveEnd, lineData))
		{
			vector<string> lineInfo = SplitString(lineData);
			endName.push_back(lineInfo[0]);
		}
		haveEnd.close();
		#pragma endregion
	}

	CScrollWindows::~CScrollWindows()
	{

	}

	void CScrollWindows::OnScrolling(short _s)
	{
		
		int move = (int)_s;
		move /= 12;

		const int NOW_BOTTOM = (endingVector[rowNum - 1][colNum - 1]).GetAnimate()->GetRect().bottom;
		const int LAST_TOP = (endingVector[rowNum - 1][colNum - 1]).GetAnimate()->Top();

		if (LAST_TOP - move < cover.GetRect().bottom || NOW_BOTTOM - move > limit_buttom)
		{
			return;
		}

		for (int r = 0; r < rowNum; r++)
		{
			for (int c = 0; c < colNum; c++)
			{
				int orgin_x = endingVector[r][c].GetAnimate()->Left();
				int orgin_y = endingVector[r][c].GetAnimate()->Top();
				endingVector[r][c].SetXY(orgin_x, orgin_y - move);
			}
		}
		//SetCloseButton(CPoint(closeButton->GetX(), closeButton->GetY() - move));
	}

	string CScrollWindows::GetCollisionButtonName(CPoint mPoint)
	{
		for (unsigned r = 0; r < endingVector.size(); r++)
		{
			for (unsigned c = 0; c < endingVector[r].size(); c++)
			{
				if (endingVector[r][c].GetState() && IsPointInRect(mPoint, endingVector[r][c].GetAnimate()->GetRect())) //已解鎖 + 有碰撞
				{
					return endingVector[r][c].GetName();
				}
			}
		}
		return "NoButtonClick";
	}

	void CScrollWindows::LoadResource()
	{
		CWindows::LoadResource();
		cover.LoadBitmap("RES\\Windows", "EndingWindows_Cover", RGB(214, 214, 214));

		for (int r = 0; r < rowNum; r++)
		{
			vector<CButton> columnBitmaps(colNum);
			for (int c = 0; c < colNum; c++)
			{
				int endID = r + c;

				if (c != 0)
					endID++;

				#pragma region - Create loadString - 
				vector<string> loadString;
				loadString.push_back("RES\\End\\EndImg\\end_lock.bmp");
				loadString.push_back("RES\\End\\EndImg\\end_" + to_string(endID) + "_0.bmp");
				#pragma endregion

				//columnBitmaps[c].LoadBitmap("RES\\End\\EndImg", "end_" + to_string(endID), 1, RGB(214, 214, 214));
				columnBitmaps[c] = CButton(BitmapPath(loadString, RGB(214, 214, 214)), CPoint(x, y), false, true);
				
				columnBitmaps[c].SetName(endID < (int)endName.size() ? endName[endID] : "lock");

				columnBitmaps[c].SetValid(false);
				//columnBitmaps.push_back(CButton(BitmapPath(loadString, RGB(214, 214, 214)), CPoint(x, y), false, true));
			}

			endingVector.push_back(columnBitmaps);
			
			columnBitmaps.clear();
		}

		img_height = (endingVector[0][0]).Height();
		img_width =  (endingVector[0][0]).Width();

	}

	void CScrollWindows::Initialize(CPoint _p)
	{
		//const int HEIGHT = endingVector[0][0].Height();
		//const int WIDTH = endingVector[0][0].Width();
		CWindows::Initialize(_p);
		cover.SetTopLeft(_p.x, _p.y);

		// set image initial position
		img_x = x + 50;
		img_y = cover.GetRect().bottom + 10;

		// set padding
		const int PADDING_X = 10;
		const int PADDING_Y = 10;

		//set limit of scrolling
		limit_top = img_y;
		limit_buttom = img_y + colNum*(img_height + PADDING_Y);

		for (int r = 0; r < rowNum; r++)
		{
			for (int c = 0; c < colNum; c++)
			{
				endingVector[r][c].SetXY(img_x + r*(img_width + PADDING_X), img_y + c*(img_height + PADDING_Y));
				#pragma region - set get end -
				bool flag = CEndManager::Instance()->IsPassEnd(endingVector[r][c].GetName());
				endingVector[r][c].SetState(CEndManager::Instance()->IsPassEnd(endingVector[r][c].GetName()));
				#pragma endregion

			}
		}

		
	}

	//void CScrollWindows::Clear()
	//{
	//	CWindows::Clear();
	//}

	void CScrollWindows::Close()
	{
		isOpen = false;
		Initialize(CPoint(x, y));
	}

	void CScrollWindows::OnCycle()
	{
		CWindows::OnCycle();
		if (!IsOpen())
		{
			return;
		}

		cover.SetTopLeft(x, y);

		for (int r = 0; r < rowNum; r++)
		{
			for (int c = 0; c < colNum; c++)
			{
				endingVector[r][c].OnMove();
			}
		}

	}

	void CScrollWindows::OnShow()
	{

		if (!IsOpen())
		{
			return;
		}

		background.ShowBitmap();

		for (int r = 0; r < rowNum; r++)
		{
			for (int c = 0; c < colNum; c++)
			{
				if (endingVector[r][c].GetAnimate()->GetRect().bottom <= background.GetRect().bottom && endingVector[r][c].GetAnimate()->Top() >= y)
					endingVector[r][c].OnShow();
			}
		}

		cover.ShowBitmap();
		closeButton->OnShow();
	}

	#pragma endregion


	#pragma endregion
}