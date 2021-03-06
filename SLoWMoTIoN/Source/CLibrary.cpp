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

  char* ConvertCharPointToString(string str)
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

  void getFolderFile(string folderPath, vector<string>* file) //得到資料夾下的所有檔名
  {
    DIR* fp; // create folder point
    fp = opendir(folderPath.c_str());
    struct dirent* folderp;
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
    DIR* fp; // create folder point
    fp = opendir(folderPath.c_str());
    struct dirent* folderp;
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

  bool IsRectInRect(CRect rect1, CRect rect2) //矩形碰撞
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
    //GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
    if (bmp.size() == 0)
      return;

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

    bmp[bmp_index].SetTopLeft(x, y);
  }

  void CAnimate::OnMove(int dir)
  {
    //GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
    if (bmp.size() == 0)
      return;

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

    bmp[bmp_index].SetTopLeft(x, y);
  }

  void CAnimate::LoadBitmap(vector<string> bmps, COLORREF colorkey)
  {
    for (unsigned int i = 0; i < bmps.size(); i++)
    {
      char* address = ConvertCharPointToString(bmps[i]);
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
    return &(bmp[bmp.size() - 1]);
  }

  CMovingBitmap* CAnimate::AddBitmap(char* filename, COLORREF colorkey)
  {
    CMovingBitmap add_bmp;
    add_bmp.LoadBitmap(filename, colorkey);
    bmp.push_back(add_bmp);
    bmp_amount++;
    return &(bmp[bmp.size() - 1]);
  }

  void CAnimate::Reset()
  {
    GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
  }

  void CAnimate::SetTopLeft(int nx, int ny)
  {
    x = nx, y = ny;
    bmp[bmp_index].SetTopLeft(x, y);
  }

  void CAnimate::OnShow()
  {
    //GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded before they are shown.");
    if (bmp_index < (int)bmp.size())
    {
      bmp[bmp_index].ShowBitmap();
    }
  }

  int CAnimate::Top()
  {
    //GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
    if (bmp.size() != 0)
    {
      return y;
    }
    else
    {
      return 0;
    }
  }

  int CAnimate::Left()
  {
    //GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
    if (bmp.size() != 0)
    {
      return x;
    }
    else
    {
      return 0;
    }
  }

  int CAnimate::Height()
  {
    //GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
    if (bmp.size() != 0)
    {
      return bmp[bmp_index].Height();
    }
    else
    {
      return 0;
    }
  }

  int CAnimate::Width()
  {
    //GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
    if (bmp.size() != 0)
    {
      return bmp[bmp_index].Width();
    }
    else
    {
      return 0;
    }
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

  void CAnimate::CopyAnimateInformation(CAnimate* copyAnimate)
  {
    SetValid(copyAnimate->GetValid());
    SetTopLeft(copyAnimate->Left(), copyAnimate->Top());
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

  #pragma region -- operator overload --
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

  CCamera* CCamera::Instance()
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
    if (canMoving)
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
    loadpath = _loadpath;
    name = _loadpath.name;
    needCollision = _needCollision;
    x = initPos.x;
    y = initPos.y;
    SetState(initState);
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
      if (loadpath.path.empty())
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
  }

  void CButton::SetValid(bool _flag)
  {
    animation.SetValid(_flag);
  }

  bool CButton::GetValid()
  {
    return animation.GetValid();
  }

  CAnimate* CButton::GetAnimate()
  {
    return &animation;
  }

  void CButton::CollisonMouse(CPoint _m)
  {
    if (!needCollision)
      return;

    if (IsCollisionMouse(_m))
    {
      if (!GetState()) //只有第一次進入Button有音效
        CAudio::Instance()->Play("btn_collision_3");

      SetState(true);
    }
    else
      SetState(false);
  }

  void CButton::ClickButton()
  {
    SetState(!GetState());
    CAudio::Instance()->Play("btn_click_3");
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
  CInteger::CInteger(int digits)
    : NUMDIGITS(digits)
  {
    isBmpLoaded = false;
  }

  void CInteger::Initialize(CPoint init_pos, int init_num, int init_digitNum)
  {
    SetInteger(init_num);
    SetTopLeft(init_pos.x, init_pos.y);
    NUMDIGITS = init_digitNum;
    SetValid(true);
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

  void CInteger::SetInteger(int i)
  {
    n = i;
  }

  void CInteger::SetTopLeft(int nx, int ny)		// 將動畫的左上角座標移至 (x,y)
  {
    x = nx;
    y = ny;
  }

  void CInteger::ShowBitmap()
  {
    GAME_ASSERT(isBmpLoaded, "CInteger: 請先執行LoadBitmap，然後才能ShowBitmap");
    int nx;		// 待顯示位數的 x 座標
    int MSB;	// 最左邊(含符號)的位數的數值

    if (n >= 0) {
      MSB = n;
      nx = x + digit[0].Width() * (NUMDIGITS - 1);
    }
    else {
      MSB = -n;
      nx = x + digit[0].Width() * NUMDIGITS;
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
      digit[10].ShowBitmap();
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
    nowBitmap = NULL;
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
  }

  void CAction::OnShow()
  {
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
    bmp.DeleteObject();
    bmp.m_hObject = LoadImage(NULL, _path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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

  #pragma region - Windows -

  #pragma region - CWindow -
  CWindow::CWindow()
  {
    x = y = 0;
    isLoaded = false;
    isOpen = false;
    closeButton = new CButton(BitmapPath("RES\\Button", "close", 2, RGB(214, 214, 214)), CPoint(0, 0), false, true);
  }

  CWindow::CWindow(CPoint _p)
  {
    SetXY(_p);
    isLoaded = false;
    isOpen = false;
  }

  CWindow::~CWindow()
  {
    Clear();
  }

  void CWindow::LoadResource()
  {
    closeButton->LoadBitmap();
    background.LoadBitmap("RES\\Windows", "EndWindow_2", RGB(214, 214, 214));
  }

  void CWindow::Initialize(CPoint _initP)
  {
    SetXY(_initP);
    const int CLSBTN_INIT_X = x + background.Width() - closeButton->Width() - 10;
    const int CLSBTN_INIT_Y = y + 10;
    closeButton->Initialize(CPoint(CLSBTN_INIT_X, CLSBTN_INIT_Y), false);
    background.SetTopLeft(x, y);
    background.SetValid(false);
    closeButton->SetValid(false);
    CLayerManager::Instance()->AddObject(&background, INTERFACE_LAYER);
    CLayerManager::Instance()->AddObject(closeButton->GetAnimate(), INTERFACE_LAYER);
    isOpen = false;
  }

  void CWindow::Clear()
  {
    if (closeButton != NULL)
    {
      delete closeButton;
      closeButton = NULL;
    }
  }

  void CWindow::Open()
  {
    if (!isOpen)
    {
      isOpen = true;
      CAudio::Instance()->Play("open_window");
    }
  }

  void CWindow::Close()
  {
    if (isOpen)
    {
      isOpen = false;
      CAudio::Instance()->Play("page_close");
    }
  }

  bool CWindow::IsCollisionClose(CPoint _m)
  {
    return closeButton->IsCollisionMouse(_m) && isOpen;
  }

  bool CWindow::IsOpen()
  {
    return isOpen;
  }

  void CWindow::SetCloseButton(CPoint _p)
  {
    closeButton->SetXY(_p.x, _p.y);
  }

  void CWindow::SetXY(CPoint _p)
  {
    x = _p.x;
    y = _p.y;
  }

  void CWindow::CollisionClose(CPoint _p)
  {
    closeButton->CollisonMouse(_p);
  }

  void CWindow::OnCycle()
  {
    background.SetValid(isOpen);
    closeButton->SetValid(isOpen);

    if (!IsOpen())
    {
      return;
    }

    closeButton->OnMove();
    background.SetTopLeft(x, y);
  }

  void CWindow::OnShow()
  {
    if (!IsOpen())
    {
      return;
    }

    background.ShowBitmap();
    closeButton->OnShow();
  }
  #pragma endregion

  #pragma region - Scroll windows -

  CScrollWindow::CScrollWindow(): CWindow()
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

  CScrollWindow::~CScrollWindow()
  {
  }

  void CScrollWindow::OnScrolling(short _s)
  {
    int move = -(int)_s;
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
  }

  string CScrollWindow::GetCollisionButtonName(CPoint mPoint)
  {
    for (unsigned r = 0; r < endingVector.size(); r++)
    {
      for (unsigned c = 0; c < endingVector[r].size(); c++)
      {
        if (IsPointInRect(mPoint, endingVector[r][c].GetAnimate()->GetRect())) // 有碰撞
        {
          if (!endingVector[r][c].GetState())		// 未解鎖
          {
            CAudio::Instance()->Play("not_clear");
            return "NoButtonClick";
          }

          return endingVector[r][c].GetName();	// 有碰撞 + 已解鎖
        }
      }
    }

    return "NoButtonClick";
  }

  void CScrollWindow::LoadResource()
  {
    CWindow::LoadResource();
    cover.LoadBitmap("RES\\Windows", "cover_top", RGB(214, 214, 214));
    cover_bottom.LoadBitmap("RES\\Windows", "cover_bottom", RGB(214, 214, 214));

	int endID = 0;

    for (int r = 0; r < rowNum; r++)
    {
      vector<CButton> columnBitmaps(colNum);

      for (int c = 0; c < colNum; c++)
      {
        #pragma region - Create loadString -
        vector<string> loadString;
        loadString.push_back("RES\\End\\EndImg\\end_lock.bmp");
        loadString.push_back("RES\\End\\EndImg\\end_" + to_string(endID) + "_0.bmp");
        #pragma endregion
        columnBitmaps[c] = CButton(BitmapPath(loadString, RGB(214, 214, 214)), CPoint(x, y), false, true);
        columnBitmaps[c].SetName(endID < (int)endName.size() ? endName[endID] : "lock");
        columnBitmaps[c].SetValid(false);
		endID++;
      }

      endingVector.push_back(columnBitmaps);
      columnBitmaps.clear();
    }

    img_height = (endingVector[0][0]).Height();
    img_width =  (endingVector[0][0]).Width();
  }

  void CScrollWindow::Initialize(CPoint _p)
  {
    CWindow::Initialize(_p);
    cover.SetTopLeft(_p.x, _p.y);
    cover_bottom.SetTopLeft(_p.x, SIZE_Y - cover_bottom.Height());
    // set image initial position
    img_x = x + 150;
    img_y = cover.GetRect().bottom + 10;
    // set padding
    const int PADDING_X = 10;
    const int PADDING_Y = 10;
    //set limit of scrolling
    limit_top = img_y;
    limit_buttom = img_y + colNum * (img_height + PADDING_Y);

	int endID = 0;
    for (int r = 0; r < rowNum; r++)
    {
      for (int c = 0; c < colNum; c++)
      {
        endingVector[r][c].SetXY(img_x + (endID % 2) * (img_width + PADDING_X), img_y + (endID / 2) * (img_height + PADDING_Y));
        #pragma region - set get end -
        bool flag = CEndManager::Instance()->IsPassEnd(endingVector[r][c].GetName());
        endingVector[r][c].SetState(CEndManager::Instance()->IsPassEnd(endingVector[r][c].GetName()));
        #pragma endregion
		endID++;
      }
    }
  }

  void CScrollWindow::Close()
  {
    CWindow::Close();
    isOpen = false;
    Initialize(CPoint(x, y));
  }

  void CScrollWindow::OnCycle()
  {
    CWindow::OnCycle();

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
        //endingVector[r][c].CollisonMouse(mousePos);
      }
    }
  }

  void CScrollWindow::OnShow()
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
    cover_bottom.ShowBitmap();
    closeButton->OnShow();
  }
  #pragma endregion

  #pragma region - CSwitchWindow -
  CSwitchWindow::CSwitchWindow() : CWindow()
  {
    bmp.clear();
    index = 0;
    step = 0;
    arrow_left.SetValid(false);
    arrow_right.SetValid(false);
  }

  CSwitchWindow::~CSwitchWindow()
  {
  }

  void CSwitchWindow::LoadResource(string folder)
  {
    closeButton->LoadBitmap();
    vector<string> bmpPath;
    int folderNumber = getFolerFileNumber(folder);

    for (int k = 0; k < folderNumber; k++)
    {
      vector<CMovingBitmap> tempVectorBmp;
      string child_folder = folder + std::to_string(k) + "\\";
      bmpPath.clear();
      getFolderFile(child_folder, &bmpPath);

      for (unsigned int i = 0; i < bmpPath.size(); i++)
      {
        #pragma region - add bmp -
        CMovingBitmap tempBmp;
        string path = child_folder + bmpPath[i];
        char* address = ConvertCharPointToString(child_folder + bmpPath[i]);
        tempBmp.LoadBitmap(address);
        delete address;
        tempBmp.SetValid(false);
        tempBmp.SetTopLeft(0, 0);
        #pragma endregion
        tempVectorBmp.push_back(tempBmp);
      }

      bmp.push_back(tempVectorBmp);
    }

    background = bmp[0][0];
    #pragma region - set arrow -
    arrow_left.LoadBitmap("RES\\Handbook\\arrow_left.bmp", RGB(214, 214, 214));
    arrow_right.LoadBitmap("RES\\Handbook\\arrow_right.bmp", RGB(214, 214, 214));
    #pragma endregion
  }

  void CSwitchWindow::Initialize(CPoint _point)
  {
    CWindow::Initialize(_point);
    arrow_left.SetTopLeft(0, 180);
    arrow_right.SetTopLeft(560, 180);
    arrow_left.SetValid(false);
    arrow_right.SetValid(false);
    CLayerManager::Instance()->AddObject(&arrow_left, INTERFACE_LAYER + 1);
    CLayerManager::Instance()->AddObject(&arrow_right, INTERFACE_LAYER + 1);
  }

  void CSwitchWindow::Open()
  {
    CWindow::Open();

    if (index < (int)bmp.size())
    {
      background = bmp[index][step];
      arrow_left.SetValid(false);
      arrow_right.SetValid(true);
    }
  }

  void CSwitchWindow::Close()
  {
    CWindow::Close();
    bmp[index][step].SetValid(false);
    index = 0;
    step = 0;
    arrow_left.SetValid(false);
    arrow_right.SetValid(false);
  }

  void CSwitchWindow::Switchwindow(string dir)
  {
    bmp[index][step].SetValid(false);

    if (dir == "left")
    {
      SetIndex(index - 1);
      step = 0;
    }
    else if (dir == "right")
    {
      SetIndex(index + 1);
      step = 0;
    }

    bmp[index][step].SetValid(true);
    background = bmp[index][step];
  }

  void CSwitchWindow::SetIndex(int _index)
  {
    if (_index <= 0)
    {
      index = 0;
    }
    else if (_index >= (int)bmp.size())
    {
      index = (int)bmp.size() - 1;
    }
    else
    {
      index = _index;
    }
  }

  bool CSwitchWindow::CollisionArrow(CPoint mPoint)
  {
    if (arrow_left.GetValid() && IsPointInRect(mPoint, arrow_left.GetRect()))
    {
      Switchwindow("left");
      return true;
    }
    else if (arrow_right.GetValid() && IsPointInRect(mPoint, arrow_right.GetRect()))
    {
      Switchwindow("right");
      return true;
    }

    return false;
  }

  void CSwitchWindow::ClickWindows(CPoint point, string mouseKey)
  {
    if (!CollisionArrow(point))
    {
      if (mouseKey == "left" && step < (int)bmp[index].size() - 1)
      {
        step++;
      }
      else if (mouseKey == "right" && step > 0)
      {
        step--;
      }

      Switchwindow("this");
    }
  }

  void CSwitchWindow::OnCycle()
  {
    CWindow::OnCycle();

    if (!IsOpen())
    {
      return;
    }

    arrow_left.SetValid(IsOpen() && index != 0);
    arrow_right.SetValid(IsOpen() && index < (int)bmp.size() - 1);
  }
  #pragma endregion

  #pragma region - CPanel -
  CPanel::CPanel()
  {
    btnManager = new CButtonManager();
  }

  void CPanel::CreatButton()
  {
    const int BTN_X = 240;
    const int F_BTN_Y = 120;
    btnManager->CreateButton(BitmapPath("RES\\Button", "resume", 2, RGB(214, 214, 214)), CPoint(BTN_X, F_BTN_Y), false, true);
    btnManager->CreateButton(BitmapPath("RES\\Button", "restart", 2, RGB(214, 214, 214)), CPoint(BTN_X, F_BTN_Y + 60), false, true);
    btnManager->CreateButton(BitmapPath("RES\\Button", "menu", 2, RGB(214, 214, 214)), CPoint(BTN_X, F_BTN_Y + 120), false, true);
    btnManager->CreateButton(BitmapPath("RES\\Button", "exit", 2, RGB(214, 214, 214)), CPoint(BTN_X, F_BTN_Y + 180), false, true);
  }

  void CPanel::LoadResource()
  {
    CWindow::LoadResource();
    btnManager->Load();
    CMovingBitmap panelBK;
    panelBK.LoadBitmap("RES\\UI\\panel", "panel", RGB(214, 214, 214));
    background = panelBK;
  }

  void CPanel::Initialize(CPoint _p)
  {
    CWindow::Initialize(_p);
    btnManager->Initialize();
    btnManager->SetValid(false);
    const int BTN_X = _p.x + background.Width() - 50;
    const int BTN_Y = _p.y - 30;
    closeButton->SetXY(BTN_X, BTN_Y);
  }

  void CPanel::Clear()
  {
    CWindow::Clear();

    if (btnManager != NULL)
    {
      btnManager->Clear();
      delete btnManager;
      btnManager = NULL;
    }
  }

  void CPanel::OnCycle()
  {
    CWindow::OnCycle();
    btnManager->SetValid(IsOpen());
    btnManager->UpdateState(mousePos);
    CollisionClose(mousePos);
    btnManager->OnCycle();
  }

  string CPanel::GetCollisionButtonName()
  {
    return btnManager->GetCollisionButtonName();
  }
  #pragma endregion

  #pragma endregion

  #pragma region - Status Board -

  #pragma region - CStatusBoard -
  CStatusBoard::CStatusBoard()
  {
  }

  void CStatusBoard::Load()
  {
    HP_bar.LoadBitmap("RES\\UI\\status\\blood.bmp", RGB(214, 214, 214));
    HP_frame.LoadBitmap("RES\\UI\\status\\bar_frame.bmp", RGB(214, 214, 214));
    EQ_bar.LoadBitmap("RES\\UI\\status\\EQ.bmp", RGB(214, 214, 214));
    EQ_frame.LoadBitmap("RES\\UI\\status\\bar_frame.bmp", RGB(214, 214, 214));
    avatar.LoadBitmap("RES\\UI\\status\\avatar.bmp");
    avatar_frame.LoadBitmap("RES\\UI\\status\\avatar_frame.bmp", RGB(214, 214, 214));
  }

  void CStatusBoard::Initialize(CPoint _p, int _HP, int _EQ)
  {
    SetXY(_p);
    SetDeltaBar(_HP, _EQ);
    hp = _HP;
    eq = _EQ;
    CLayerManager::Instance()->AddObject(&HP_bar, INTERFACE_LAYER - 1);
    CLayerManager::Instance()->AddObject(&EQ_bar, INTERFACE_LAYER - 1);
    CLayerManager::Instance()->AddObject(&avatar, INTERFACE_LAYER - 1);
    CLayerManager::Instance()->AddObject(&HP_frame, INTERFACE_LAYER);
    CLayerManager::Instance()->AddObject(&EQ_frame, INTERFACE_LAYER);
    CLayerManager::Instance()->AddObject(&avatar_frame, INTERFACE_LAYER);
  }

  void CStatusBoard::UpdateBar(int _HP, int _EQ)
  {
    if (_HP <= 0)
    {
      HP_bar.SetTopLeft(-HP_bar.Width(), HP_bar.Top());
    }
    else
    {
      int subHP = hp - _HP;
      hp = _HP;
      HP_bar.SetTopLeft(HP_bar.Left() - dHP * subHP, HP_bar.Top());
    }

    if (_EQ <= 0)
      EQ_bar.SetTopLeft(-EQ_bar.Width(), EQ_bar.Top());

    {
      int subEQ = eq - _EQ;
      eq = _EQ;
      EQ_bar.SetTopLeft(EQ_bar.Left() - dEQ * subEQ, EQ_bar.Top());
    }
  }

  void CStatusBoard::SetXY(CPoint _p)
  {
    avatar.SetTopLeft(_p.x, _p.y);
    avatar_frame.SetTopLeft(_p.x, _p.y);
    const int HP_X = _p.x + avatar_frame.Width();
    const int HP_Y = _p.y;
    const int PADDING_Y = 15;
    HP_frame.SetTopLeft(HP_X, HP_Y + PADDING_Y);
    HP_bar.SetTopLeft(HP_X, HP_Y + PADDING_Y);
    const int EQ_X = _p.x + avatar_frame.Width();
    const int EQ_Y = HP_bar.GetRect().bottom;
    EQ_frame.SetTopLeft(HP_X, EQ_Y);
    EQ_bar.SetTopLeft(HP_X, EQ_Y);
  }

  void CStatusBoard::SetDeltaBar(int _blood, int _EQ)
  {
    dHP = HP_bar.Width() / _blood;
    dEQ = EQ_bar.Width() / _EQ;
  }

  void CStatusBoard::OnCycle(int _HP, int _EQ)
  {
    UpdateBar(_HP, _EQ);
  }
  #pragma endregion

  #pragma region - BossBoard -
  CBossBoard::CBossBoard()
  {
  }

  void CBossBoard::Load()
  {
    HP_bar.LoadBitmap("RES\\UI\\status\\blood.bmp", RGB(214, 214, 214));
    HP_frame.LoadBitmap("RES\\UI\\status\\bar_frame.bmp", RGB(214, 214, 214));
    avatar.LoadBitmap("RES\\Boss\\Avatar\\Xingting.bmp");	// 要擋住血條
    NULLAvatar.LoadBitmap("RES\\Boss\\Avatar\\NULL.bmp");
    avatar_frame.LoadBitmap("RES\\UI\\status\\avatar_frame.bmp", RGB(214, 214, 214));
    #pragma region - Load boss avatar -
    vector<string> avatarName;
    string avatarFolderPath = "RES\\Boss\\Avatar\\";
    getFolderFile(avatarFolderPath, &avatarName);

    for (unsigned i = 0; i < avatarName.size(); i++)
    {
      int strLength = avatarName[i].length();
      string ext = avatarName[i].substr(strLength - 4, strLength);

      if (ext == ".bmp")
      {
        string sAvatar = getFileName(avatarName[i]);
        bossAvatar[sAvatar] = CMovingBitmap();
        char* address = ConvertCharPointToString((avatarFolderPath + avatarName[i]));
        bossAvatar[sAvatar].LoadBitmap(address);
        delete address;
      }
    }

    #pragma endregion
  }

  void CBossBoard::Initialize(CPoint _p)
  {
    SetXY(_p);
    initPos = _p;
    CLayerManager::Instance()->AddObject(&HP_bar, INTERFACE_LAYER - 1);
    CLayerManager::Instance()->AddObject(&HP_frame, INTERFACE_LAYER);
    CLayerManager::Instance()->AddObject(&avatar_frame, INTERFACE_LAYER);
    CLayerManager::Instance()->AddObject(&avatar, INTERFACE_LAYER - 1);
    SetShow(false);
  }

  void CBossBoard::UpdateBar(int _HP)
  {
    if (_HP <= 0)	//強制補正
    {
      HP_bar.SetTopLeft(avatar_frame.Left(), HP_bar.Top());
    }

    int subHP = hp - _HP;
    hp = _HP;
    HP_bar.SetTopLeft(HP_bar.Left() + (int)(dHP * subHP), HP_bar.Top());
  }

  void CBossBoard::SetXY(CPoint _p)
  {
    const int AVATAR_X = SIZE_X - avatar_frame.Width();
    const int AVATAR_Y = _p.y;
    avatar.SetTopLeft(AVATAR_X, AVATAR_Y);
    avatar_frame.SetTopLeft(AVATAR_X, AVATAR_Y);
    const int HP_X = AVATAR_X - HP_frame.Width();
    const int HP_Y = avatar_frame.GetRect().bottom - HP_frame.Height();
    const int PADDING_Y = 15;
    HP_frame.SetTopLeft(HP_X, HP_Y);
    HP_bar.SetTopLeft(HP_X, HP_Y);
  }

  void CBossBoard::SetDeltaBar(int _HP)
  {
    dHP = (double)(HP_bar.Width() / _HP);
  }

  void CBossBoard::SetShow(bool _isValid)
  {
    isShow = _isValid;
    HP_bar.SetValid(_isValid);
    HP_frame.SetValid(_isValid);
    avatar.SetValid(_isValid);
    avatar_frame.SetValid(_isValid);
  }

  void CBossBoard::SetHP(int _hp)
  {
    UpdateBar(_hp);
  }

  void CBossBoard::OnCycle(CBossManager* bManager)
  {
    if (bManager->targetBoss != NULL && bManager->IsBattle())
    {
      if (!IsShow())	// 只需換一次
      {
        string bossID = (bManager->targetBoss)->GetID();

        if (bossAvatar.count(bossID) && !bossAvatar[bossID].IsNull())
        {
          avatar = bossAvatar[bossID];
          hp = (bManager->targetBoss)->GetHp();
          SetDeltaBar(hp);
        }
        else
        {
          avatar = NULLAvatar;
        }

        SetXY(initPos);
        SetShow(true);
      }

      SetHP(bManager->targetBoss->GetHp());
    }
    else
    {
      SetShow(false);
    }
  }
  #pragma endregion

  #pragma endregion
}