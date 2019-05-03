#include "stdafx.h"
#include "Resource.h"
#include "Refactor.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <map>
#include "mygame.h"
#include "CManager.h"
using namespace myLibrary;

namespace game_framework
{
	#pragma region - mapManager -
	CMapManager::CMapManager()
	{
		InitializeCBlockMap();
		//Initialize();
	}


	CMapManager::~CMapManager()
	{
	}

	vector<CPasserby*>* CMapManager::GetPasserby()
	{
		return &passerbyManager.passerby;
	}

	void CMapManager::DeletePasserby(vector<CPasserby*>::iterator passerbyj)
	{
		passerbyManager.DeletePasserby(passerbyj);
	}

	void CMapManager::AddPasserby()
	{
		passerbyManager.passerby.push_back(passerbyManager.AddPasserby(blockMap[nowMap].passerbyID, background.Width()));
	}

	void CMapManager::Initialize()
	{
		nowMap = NOW_MAP;
		loadMap = blockMap[nowMap].loadMap;
		x = 0;
		layer.SetLayer(0);
		background = blockMap[nowMap].backgroundBitmap;
		background.SetTopLeft(0, 0);
		CLayerManager::Instance()->AddObject(&background, layer.GetLayer());
		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width());
	}

	#pragma region - GetMap -

	int CMapManager::GetNowMap()
	{
		return nowMap;
	}

	int CMapManager::GetUpMap()
	{
		return blockMap[nowMap].upMap;
	}

	int CMapManager::GetDownMap()
	{
		return blockMap[nowMap].downMap;
	}

	int CMapManager::GetLeftMap()
	{
		return blockMap[nowMap].leftMap;
	}

	int CMapManager::GetRightMap()
	{
		return blockMap[nowMap].rightMap;
	}

	int CMapManager::GetLoadMap()
	{
		return loadMap;
	}

	int CMapManager::GetNpcNumber()
	{
		return passerbyManager.passerby.size();
	}

	int CMapManager::GetNpcLayer(int npcIndex)
	{
		return passerbyManager.passerby[npcIndex]->layer.GetLayer();
	}

	bool CMapManager::GetNpcValid(int npcIndex)
	{
		return passerbyManager.passerby[npcIndex]->GetValid();
	}

	CMovingBitmap* CMapManager::GetBitmap()
	{
		return &background;
	}

	CAnimate* CMapManager::GetNpc(int npcIndex)
	{
		return (passerbyManager.passerby[npcIndex]->GetAnimate());
	}

	int CMapManager::GetSplitLeft()
	{
		return 0 + (SIZE_X / 2);
	}

	int CMapManager::GetSplitRight()
	{
		return background.Width() - (SIZE_X / 2);
	}

	int CMapManager::GetX1()
	{
		return x;
	}

	int CMapManager::GetX2()
	{
		return x + (SIZE_X);
	}

	int CMapManager::GetBitmapWidth()
	{
		return background.Width();
	}
	#pragma endregion

	void CMapManager::ChangeMap(int changeMap, string nextMap)
	{
		nowMap = changeMap;
		loadMap = blockMap[nowMap].loadMap;
		background = blockMap[nowMap].backgroundBitmap;
		if (nextMap == "left") //下一張地圖(要換的地圖)，在原本地圖的左邊 (移動到左邊的地圖)
		{
			x = init_x = -(GetBitmapWidth() - SIZE_X);
		}
		else if (nextMap == "right")  //下一張地圖(要換的地圖)，在原本地圖的右邊 (移動到右邊的地圖)
		{
			x = init_x =  0; 
		}
		background.SetTopLeft(x, 0);

		passerbyManager.Clear();
		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width());

		CCamera::Instance()->Reset();
	}

	void CMapManager::SetMovingLeft(bool _flag)
	{
		isMovingLeft = _flag;
	}

	void CMapManager::SetMovingRight(bool _flag)
	{
		isMovingRight = _flag;
	}

	void CMapManager::OnMove()
	{
		
		//if (isMovingLeft)
		//{
		//	x += directionX;
		//	//for(unsigned int npcIndex = 0; npcIndex < blockMap[nowMap].npc.size(); npcIndex++)
		//		//blockMap[nowMap].npc[npcIndex].SetXY(blockMap[nowMap].npc[npcIndex].GetX1() + directionX, blockMap[nowMap].npc[npcIndex].GetY1());
		//	for (unsigned int npcIndex = 0; npcIndex < passerbyManager.passerby.size(); npcIndex++)
		//		passerbyManager.passerby[npcIndex]->SetXY(passerbyManager.passerby[npcIndex]->GetX1() + directionX, passerbyManager.passerby[npcIndex]->GetY1());
		//}
		//if (isMovingRight)
		//{
		//	x -= directionX;
		//	for (unsigned int npcIndex = 0; npcIndex < passerbyManager.passerby.size(); npcIndex++)
		//		passerbyManager.passerby[npcIndex]->SetXY(passerbyManager.passerby[npcIndex]->GetX1() - directionX, passerbyManager.passerby[npcIndex]->GetY1());
		//}
		//SetXY(x - CCamera::Instance()->GetX(), 0);

		int dx = CCamera::Instance()->GetX();
		x = init_x - dx;
		SetXY(x, 0);
	}

	void CMapManager::SetXY(int _x, int _y)
	{
		x = _x;
		if (x <= -((background.Width()) - 640))
			x = -((background.Width()) - 640);
		if (x >= 0)
			x = 0;
		background.SetTopLeft(x, _y);
	}

	bool CMapManager::IsMapEnd()
	{
		return x < 0 || x > ((background.Width()) - 640);
	}

	void CMapManager::OnShow() //顯示對應到的blockMap圖片 (nowMap = 1, 顯示blockMap[1]的background, 類推)
	{
		background.SetTopLeft(0, 0);
		background.ShowBitmap();
	}

	void CMapManager::InitializeCBlockMap()
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++) //初始化blockMap的上下左右地圖資訊，增加可讀性使用switch敘述
		{
			switch (mapIndex)
			{				//順序：目前 上 下 左 右 地圖上有幾個passerby存在， -1表示不存在
			case 0:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, 5, "RES\\Map", "IDB_MAP", mapIndex);
				break;

			case 1:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 4, 0, 2, "RES\\Map", "IDB_MAP", mapIndex);
				break;

			case 2:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 0, 3, 3, "RES\\Map", "IDB_MAP", mapIndex);
				break;

			case 3:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 2, 5, 0, "RES\\Map", "IDB_MAP", mapIndex);
				break;

			case 4:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, -1, 1, 0, "RES\\Map", "IDB_MAP", mapIndex);
				break;

			case 5:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 3, -1, 0, "RES\\Map", "IDB_MAP", mapIndex);
				break;

			default:
				blockMap[mapIndex] = CBlockMap(-1, -1, -1, -1, -1, 0, "RES\\Map", "IDB_MAP", 0);
				break;
			}
		}
	}

	void CMapManager::LoadMapBitmap() //如字面意思，LoadMapBitmap，在GameStateRun:OnInit運行，一次性load blockMap的所有圖片
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++)
		{
			char *address = ConvertCharPointToString(blockMap[mapIndex].ziliaojia, blockMap[mapIndex].name, blockMap[mapIndex].number);
			blockMap[mapIndex].backgroundBitmap.LoadBitmap(address);
			delete address;
		}
	}
	#pragma endregion

	#pragma region - layerManager -
	CLayerManager CLayerManager::layerManager;
	CLayerManager::CLayerManager()
	{
		Clear();
	}
	CLayerManager::~CLayerManager()
	{

	}
	void CLayerManager::Clear()
	{
		for (int i = 0; i < MAX_LAYER_NUMBER; i++)
		{
			layerBitmap[i].clear();
			layerAnimate[i].clear();
			layerAction[i].clear();
		}
	}

	void CLayerManager::AddObject(CMovingBitmap* object, int targetLayer)
	{
		layerBitmap[targetLayer].push_back(object);
	}

	void CLayerManager::AddObject(CAnimate* object, int targetLayer)
	{
		layerAnimate[targetLayer].push_back(object);
	}

	void CLayerManager::AddObject(CAction* object, int targetLayer)
	{
		layerAction[targetLayer].push_back(object);
	}

	void CLayerManager::ShowLayer()
	{
		for (int i = 0; i < MAX_LAYER_NUMBER; i++)
		{
			for (vector<CMovingBitmap*>::iterator k = layerBitmap[i].begin(); k != layerBitmap[i].end(); k++)
			{
				if ((*k)->GetValid())
				{
					(*k)->ShowBitmap();
				}
			}

			for (vector<CAnimate*>::iterator k = layerAnimate[i].begin(); k != layerAnimate[i].end();)
			{
				if ((*k)->IsNull())
				{
					k = layerAnimate[i].erase(k);
				}
				else
				{
					if ((*k)->GetValid())	//if valid then show
					{
						(*k)->OnShow();
					}
					k++;
				}
			}

			for (vector<CAction*>::iterator k = layerAction[i].begin(); k != layerAction[i].end();)
			{
				if ((*k)->IsNull())
				{
					k = layerAction[i].erase(k);
				}
				else
				{
					if ((*k)->GetValid())	//if valid then show
					{
						(*k)->OnShow();
					}
					k++;
				}
			}
		}
	}
	CLayerManager * CLayerManager::Instance()
	{
		return &layerManager;
	}
	void CLayerManager::Initialize()
	{
		Clear();
	}
	#pragma endregion

	#pragma region - passerbyManager -
	CPasserbyManager::CPasserbyManager()
	{
		layer.SetLayer(4);
		passerby.clear();
	}
	CPasserbyManager::~CPasserbyManager()
	{
		Clear();
	}
	void CPasserbyManager::Clear()
	{
		for (vector<CPasserby*>::iterator it = passerby.begin(); it != passerby.end(); it++)
		{
			delete *it;
			(*it) = NULL;
		}
		vector<CPasserby*> del;
		passerby.swap(del);
		passerby.clear();
	}
	CPasserby* CPasserbyManager::AddPasserby(vector<int> id, int mapWidth)
	{
		#pragma region Create a Passerby
		int randomID = GetRandom(0, id.size() - 1); //random 決定 passerby種類 (1號or2號)
		CPasserby *newPasserby = new CPasserby(0, 0, ziliaojia, name[id[randomID]], 2, (id[randomID] + 1) * 10); //先創建一個default passerby
		//newPasserby->LoadBitmap(ziliaojia, name[id[randomID]], 2, RGB(255, 255, 255)); //load passerby的圖片
		int randomX = GetRandom(0, mapWidth - newPasserby->Width()); //random 決定passerby的出現位置
		newPasserby->SetXY(randomX, 480 - newPasserby->Height()); //set passerby x, y
		return newPasserby;
		#pragma endregion
	}
	void CPasserbyManager::CreatePasserby(int createNumber, vector<int> id, int mapWidth)
	{
		for (int i = 0; i < createNumber; i++)
		{
			CPasserby* newPasserby = AddPasserby(id, mapWidth);
			newPasserby->SetValid(true);
			passerby.push_back(newPasserby);
		}
	}
	void CPasserbyManager::DeletePasserby(vector<CPasserby*>::iterator passerbyj)
	{
		delete *passerbyj;
		*passerbyj = NULL;
		passerbyj = passerby.erase(passerbyj);
	}
	vector<CPasserby*> CPasserbyManager::GetPasserby()
	{
		return passerby;
	}
	#pragma endregion

	#pragma region - DialogManager -
	CDialogManager CDialogManager::dialogManager;

	CDialogManager::CDialogManager()
	{
		IsBitmapLoaded = false;
		IsDialoging = false;
		IsPrintTips = false;
		nowDialog = NULL;
		showtext = "";
		txt.clear();
		nowShowTextSize = 0;
		AddShowTextTimer = CTimer(DIALOG_ADDTEXT_TIME);
	}

	CDialogManager::~CDialogManager()
	{
		dialogmap.clear();
		nowDialog = NULL;
	}

	void CDialogManager::OnCycle()
	{
		#pragma region - add show text -
		if (showtext != "" && nowShowTextSize < (int)showtext.size())
		{
			AddShowTextTimer.CountDown();
			if (AddShowTextTimer.IsTimeOut()) //add show text
			{
				//doit
				if (showtext[nowShowTextSize] < 0) //非ASCII體系中的字
				{
					nowShowTextSize += 2;
				}
				else
				{
					nowShowTextSize++;
				}
				AddShowTextTimer.ResetTime();
				ShowText();
			}
		}
		#pragma endregion

		#pragma region - print finish tips (next) -
		if (showtext != "" && nowShowTextSize >= (int)showtext.size())
		{
			IsPrintTips = true;
		}
		#pragma endregion

	}

	void CDialogManager::Load_Image()
	{
		#pragma region - define avatar map -
		dialogAvatar[DIALOG_AVATAR_NAME_ROLE] = CMovingBitmap();
		dialogAvatar[DIALOG_AVATAR_NAME_XINGTING] = CMovingBitmap();
		dialogAvatar[DIALOG_AVATAR_NAME_LOCK] = CMovingBitmap();
		dialogAvatar[DIALOG_AVATAR_NAME_QUESTION] = CMovingBitmap();
		dialogAvatar[DIALOG_AVATAR_NAME_STUDENTB] = CMovingBitmap();
		dialogAvatar[DIALOG_AVATAR_NAME_STUDENTG] = CMovingBitmap();
		#pragma endregion

		#pragma region - load image -

		char *address;
		#pragma region - load dialog background -
		address = ConvertCharPointToString("RES\\Dialog", "ground");
		dialog_background.LoadBitmap(address, RGB(255, 255, 255));
		delete address;
		#pragma endregion

		#pragma region - load xingting - avatar -
		address = ConvertCharPointToString("RES\\Dialog\\Avatar", "xingting");
		dialogAvatar[DIALOG_AVATAR_NAME_XINGTING].LoadBitmap(address);
		delete address;
		#pragma endregion

		#pragma region - load role avatar -
		address = ConvertCharPointToString("RES\\Dialog\\Avatar", "mikuAvatar");
		dialogAvatar[DIALOG_AVATAR_NAME_ROLE].LoadBitmap(address, RGB(255,255,255));
		delete address;
		#pragma endregion

		#pragma region - load null -
		address = ConvertCharPointToString("RES\\Dialog", "null");
		avatar_null.LoadBitmap(address);
		delete address;
		#pragma endregion

		#pragma region - load next animation -
		textNext.LoadBitmap("RES\\Dialog\\next", "next", 4, DIALOG_BACKGROUND_COLOR);
		textNext.SetTopLeft(500, 450);
		#pragma endregion

		#pragma region - load lock -
		dialogAvatar[DIALOG_AVATAR_NAME_LOCK].LoadBitmap("RES\\Dialog\\Avatar\\lock_0.bmp", RGB(255,255,255));
		#pragma endregion

		#pragma region - load question -
		dialogAvatar[DIALOG_AVATAR_NAME_QUESTION].LoadBitmap("RES\\Dialog\\Avatar\\question_0.bmp", RGB(214, 241, 214));
		#pragma endregion

		#pragma	region - load student b -
		dialogAvatar[DIALOG_AVATAR_NAME_STUDENTB].LoadBitmap("RES\\Dialog\\Avatar\\studentB.bmp", RGB(214, 241, 214));
		#pragma endregion

		#pragma region - load student g -
		dialogAvatar[DIALOG_AVATAR_NAME_STUDENTG].LoadBitmap("RES\\Dialog\\Avatar\\studentG.bmp", RGB(214, 241, 214));
		#pragma endregion

		#pragma endregion

		IsBitmapLoaded = true;
	}

	void CDialogManager::LoadDialog()
	{
		//dialogmap.clear();
		dialogmap[DIALOG_DATA_VSXingting1] = CDialog("RES\\Dialog\\Txt\\VSXingting1.txt", DIALOG_DATA_VSXingting1, false);
		dialogmap[DIALOG_DATA_VSXingting2] = CDialog("RES\\Dialog\\Txt\\VSXingting2.txt", DIALOG_DATA_VSXingting2, false);
		dialogmap[Tips] = CDialog("RES\\Dialog\\Txt\\InitTip.txt", Tips, false);
		dialogmap[FROG] = CDialog("RES\\Dialog\\Txt\\test.txt", FROG, true);
		dialogmap[Music_Deadlock] = CDialog("RES\\Dialog\\Txt\\MyVoiceIsDead.txt", Music_Deadlock, true);
		dialogmap[DIALOG_DATA_MEETSTB] = CDialog("RES\\Dialog\\Txt\\MeetStudentB.txt", DIALOG_DATA_MEETSTB, true);
		dialogmap[DIALOG_DATA_STGHAVEBREAKFAST] = CDialog("RES\\Dialog\\Txt\\StudentGHaveBreakfast.txt", DIALOG_DATA_STGHAVEBREAKFAST, true);
		dialogmap[DIALOG_DATA_STGSUBMIT] = CDialog("RES\\Dialog\\Txt\\StudentKSubmitHW.txt", DIALOG_DATA_STGSUBMIT, true);

		IsDialogLoad = true;
	}


	void CDialogManager::LoadDialog(string dialogName, string loadPath, bool canReTrigger)
	{

		dialogmap[dialogName] = CDialog(loadPath, dialogName, canReTrigger);
		bool flag = dialogmap[dialogName].GetTriggered();

		flag = !flag;
	}

	void CDialogManager::ShowText_Next()
	{
		if (IsPrintTips)
		{
			//PaintText("Next", 550, 450, "微軟正黑體", 14, RGB(0, 0, 0), DIALOG_BACKGROUND_COLOR);
			textNext.SetValid(true);
			textNext.OnMove();
		}
		else
		{
			textNext.SetValid(false);
		}
	}

	void CDialogManager::Initialize()
	{
		if (IsBitmapLoaded == false)
		{
			Load_Image();
			#pragma region - Init - Image Point -
			dialog_background.SetTopLeft(0, SIZE_Y - dialog_background.Height()); //reset dbg's xy
			for (map<string, CMovingBitmap>::iterator dbmp = dialogAvatar.begin(); dbmp != dialogAvatar.end(); dbmp++)
			{
				dbmp->second.SetTopLeft(dialog_background.Left() + MARGIN_DIALOG_AVATAR, dialog_background.Top() + MARGIN_DIALOG_AVATAR);
			}
			#pragma endregion
		}

		#pragma region - reload - dialog -
		if (IsDialogLoad == false)
		{
			LoadDialog();
			IsDialogLoad = true;
		}
		else
		{
			for (map<string, CDialog>::iterator dialogiter = dialogmap.begin(); dialogiter != dialogmap.end(); dialogiter++)
			{
				dialogiter->second.Initialize();
			}
		}
		#pragma endregion

		avatar_null.SetValid(false);
		dialog_background.SetValid(false);
		textNext.SetValid(false);
		avatar = avatar_null;

		backgroundLayer.SetLayer(8);
		avatarLayer.SetLayer(backgroundLayer.GetLayer() + 1);

		CLayerManager::Instance()->AddObject(&dialog_background, backgroundLayer.GetLayer());
		CLayerManager::Instance()->AddObject(&avatar, avatarLayer.GetLayer());
		CLayerManager::Instance()->AddObject(&textNext, avatarLayer.GetLayer());

		step = 0;
		nowShowTextSize = 0;
		IsPrintTips = false;
		musicFromNPC = "";
		AddShowTextTimer.ResetTime(DIALOG_ADDTEXT_TIME);
	}

	CDialogManager * CDialogManager::Instance()
	{
		return &dialogManager;
	}

	void CDialogManager::Start(string mode)
	{

		if (DebugMode)
			return;

		nowDialog = &dialogmap[mode];
		if (nowDialog->GetTriggered()) //被觸發過 回去
		{
			nowDialog = NULL;
			return;
		}
		IsDialoging = true;
		IsPrintTips = false;
		dialog_background.SetValid(true);
		Dialog();
	}

	void CDialogManager::Next()
	{
		if(IsDialoging)
		{
			if (nowShowTextSize < (int)showtext.size()) //當字還沒顯示完的時候，就已經點擊畫面 > 直接顯示完全部文字
			{
				nowShowTextSize = (int)showtext.size();
			}
			else //字已經顯示完了，可以切換
			{
				step++;
				IsPrintTips = false;
				nowShowTextSize = 0;
				Dialog();
			}
		}	
	}

	void CDialogManager::Stop()
	{
		avatar = avatar_null;
		dialog_background.SetValid(false);
		textNext.SetValid(false);
		IsDialoging = false;
		nowDialog->SetTriggered();
		nowDialog = NULL;
		nowShowTextSize = 0;
		if (musicFromNPC != "")
		{
			CAudio::Instance()->Stop(musicFromNPC);
			CAudio::Instance()->Play("MUSIC_GAMEING");
			musicFromNPC = "";
		}
	}

	void CDialogManager::ShowText()
	{
		if (showtext != "")
		{
			#pragma region - split showtext -
			vector<char*> split_showtext;
			int charindex = 0;
			int vectorindex = 0;
			char tempk[DIALOG_MAX_TEXT + 5];
			memset(tempk, '\0', sizeof(tempk));
			
			for (int i = 0; i < nowShowTextSize;)
			{
				if (charindex < DIALOG_MAX_TEXT)
				{
					//將字元加入tempk, 當字元超過一定量的時候給split-showtext(換行)
					if (showtext[i] == '_') //例外處理，將底線換成空白 (文本不能打空白)
					{
						tempk[charindex++] = ' ';
						i++;
					}
					else  if (showtext[i] == '@') //將at 換成換行
					{
						charindex += 999;
						i++;
					}
					else 
					{
						if (showtext[i] > 0)
						{
							tempk[charindex++] = showtext[i++];
						}
						else
						{
							if (charindex + 1 < DIALOG_MAX_TEXT) //add a chinese
							{
								tempk[charindex++] = showtext[i++];
								tempk[charindex++] = showtext[i++];
							}
							else
							{
								charindex += 999;
							}
						}
					}
						
					
					#pragma region -- 針對倒數第二個字元是英文，但最後一個字是中文的額外設定 --
					#pragma region --- 條件 & 說明 ---
					/*
					1. 目前文字是英文
					2. 下一個字是中文
					3. 下一個字在29
					說明：問題發生的原因是因為英文的size是1，但中文是2，而我設定當size到30就會換行
					因此如果size=28的時候裝一個英文字(裝一個英文字後size=29)
					但下一個字是中文的時候，照理講要裝兩個size才會正確，但因為只能裝一個，另一個會到下一行才裝
					此時中文字不會正常顯示，會變成亂碼
					解決方式：符合上述條件就直接先換行
					*/
					#pragma endregion
					if (showtext[i] > 0 && showtext[i + 1] < 0 && charindex + 1 >= DIALOG_MAX_TEXT)
					{
						charindex += 999;
					}
					#pragma endregion

				}
				if (charindex >= DIALOG_MAX_TEXT)
				{
					char *tteemmppkkk = new char[DIALOG_MAX_TEXT + 5];
					strcpy(tteemmppkkk, tempk);
					split_showtext.push_back(tteemmppkkk);

					charindex = 0;
					memset(tempk, '\0', sizeof(tempk));
				}
			}
			char *tteemmppkkk = new char[DIALOG_MAX_TEXT + 5];
			strcpy(tteemmppkkk, tempk);
			split_showtext.push_back(tteemmppkkk);
			#pragma endregion

			//下面那傢伙測試用的
			//PaintText(split_showtext[0], 0 + 20, 100, "微軟正黑體", DIALOG_TEXT_SIZE, RGB(0, 0, 0), RGB(0, 255, 0));

			#pragma region - draw text -
			//char* showpointer;
			//showpointer = new char[showtext.length() + 1];
			//strcpy(showpointer, showtext.c_str());
			////x = 160, endx = 480
			//PaintText(showpointer, avatar.Left() + avatar.Width() + MARGIN_DIALOG_TEXT_X, avatar.Top() + MARGIN_DIALOG_TEXT_Y, "微軟正黑體", DIALOG_TEXT_SIZE, RGB(0, 0, 0), RGB(232, 232, 232));
			//delete showpointer;
			for (unsigned int i = 0; i < split_showtext.size(); i++)
			{
				PaintText(split_showtext[i], avatar.Left() + avatar.Width() + MARGIN_DIALOG_TEXT_X, avatar.Top() + MARGIN_DIALOG_TEXT_Y + i * 35, "微軟正黑體", DIALOG_TEXT_SIZE, DIALOG_TEXT_COLOR, DIALOG_BACKGROUND_COLOR);
				delete split_showtext[i];
			}
			#pragma endregion

			#pragma region - draw finish tips text -
			ShowText_Next();
			#pragma endregion

		}
	}

	bool CDialogManager::GetDialogState()
	{
		return IsDialoging;
	}

	void CDialogManager::Dialog()
	{
		if (step < nowDialog->GetTxtSize())
		{
			avatar = dialogAvatar[nowDialog->GetAvatar(step)];
			showtext = nowDialog->GetDialogTxt(step);
		}
		else
		{
			step = 0;
			showtext = "";
			Stop();
		}
		//if (nowDialog->GetMode() == RoleVSBoss) //先保留==
		//{
			//if (step == 0)
			//{
			//	avatar = avatar_xingting;
			//	//showtext = "我是吳杏婷, I'm a Fucking Teacher，AHHH~~~~FSDFD##@!!!";
			//	//showtext = "我是吳杏婷我是吳杏婷我是吳杏婷我是吳杏婷";
			//	//showtext = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
			//}
			//else if (step == 1)
			//{
			//	avatar = avatar_role;
			//	showtext = "Bhhh~~~";
			//}
			//else if (step == 2)
			//{
			//	avatar = avatar_xingting;
			//	showtext = "Chhh~~~";
			//}
			//else
			//{
			//	step = 0;
			//	showtext = "";
			//	Stop();
			//}
		//}

	}
	#pragma endregion

	#pragma region - BossManager -
	CBossManager::CBossManager()
	{
		#pragma region -- Create Boss --
		bossInformation[BOSS_XINGTING] = new CXingting(450, 250, 8787, BitmapPath("RES\\Boss", "xingting", 2), RGB(214, 214, 214));
		#pragma endregion
		targetBoss = NULL;
		isBattle = false;
	}

	CBossManager::~CBossManager()
	{
		if (targetBoss != NULL)
		{
			targetBoss->ClearBullet();
		}
		Clear();
	}

	void CBossManager::Initialize() //CGameRun::OnBeginState時 initialize
	{
		//(second(value)為指標，所以second後面用-> )
		//真有你的C plus plus
		for (map<string, CBoss*>::iterator bossiter = bossInformation.begin(); bossiter != bossInformation.end(); bossiter++)
		{
			bossiter->second->Initialize(); //初始化boss 
			bossiter->second->GetAnimate()->SetValid(false); //將所有圖片先設為false
			CLayerManager::Instance()->AddObject(bossiter->second->GetAnimate(), bossiter->second->layer.GetLayer());
		}
		targetBoss = NULL;
		isBattle = false;
	}

	void CBossManager::Clear()
	{
		//解構
		for (map<string, CBoss*>::iterator bossiter = bossInformation.begin(); bossiter != bossInformation.end(); bossiter++)
		{
			delete bossiter->second;
			bossiter->second = NULL;
		}

		bossInformation.clear();
		targetBoss = NULL;
	}

	void CBossManager::TargetBoss(int nowMap)
	{
		if (nowMap == BOSS_MAP_XINGTING)
		{
			targetBoss = bossInformation[BOSS_XINGTING];
			targetBoss->GetAnimate()->SetValid(true);
		}
		else
		{
			if (targetBoss != NULL)
			{
				targetBoss->GetAnimate()->SetValid(false);
				targetBoss->ClearBullet();
				isBattle = false;
				targetBoss = NULL;
			}
		}
	}
	#pragma endregion

	#pragma region - CNPCManager -
	CNPCManager::CNPCManager()
	{
		Clear();
		LoadNPC();
	}	

	CNPCManager::~CNPCManager()
	{
		Clear();
	}
	void CNPCManager::Clear()
	{
		for (int i = 0; i < MAX_MAP_NUMBER; i++)
		{
			for (vector<CNPC*>::iterator npciter = npc[i].begin(); npciter != npc[i].end(); ++npciter)
			{
				delete (*npciter);
				(*npciter) = NULL;
			}
			npc[i].clear();
			vector<CNPC*> delSwap;
			npc[i].swap(delSwap);

		}
	}

	void CNPCManager::Initialize(int nowMap)
	{
		for (int i = 0; i < MAX_MAP_NUMBER; i++)
		{
			for (vector<CNPC*>::iterator npciter = npc[i].begin(); npciter != npc[i].end(); npciter++)
			{
				(*npciter)->Initialize();
			}
		}
		SetNPCValid(nowMap, true);
	}

	void CNPCManager::ChangeMap(int nowMap, int nextMap)
	{
		SetNPCValid(nowMap, false);
		SetNPCValid(nextMap, true);
	}

	void CNPCManager::SetNPCValid(int thisMap, bool flag)
	{
		for (vector<CNPC*>::iterator npciter = npc[thisMap].begin(); npciter != npc[thisMap].end(); npciter++)
		{
			(*npciter)->SetValid(flag);
		}
	}

	void CNPCManager::OnMove(int thisMap)
	{
		for (vector<CNPC*>::iterator npciter = npc[thisMap].begin(); npciter != npc[thisMap].end(); npciter++)
		{
			(*npciter)->OnMove();
		}
	}

	void CNPCManager::LoadNPC()
	{
		npc[4].push_back(new CNPC1(CPoint(50, 388), BitmapPath("RES\\NPC\\test", "test", 1, RGB(255, 255, 255)), "frog", FROG));
		npc[4].push_back(new CNPC3(CPoint(450, 388), BitmapPath("RES\\NPC\\test", "test2", 1, RGB(255, 255, 255)), "deadlock", "MUSIC_DeadLock", Music_Deadlock));

		npc[3].push_back(new CNPC1(CPoint(50, 388), BitmapPath("RES\\NPC\\test", "test", 1, RGB(255, 255, 255)), DIALOG_AVATAR_NAME_STUDENTB, DIALOG_DATA_MEETSTB));
		npc[3].push_back(new CNPC1(CPoint(450, 388), BitmapPath("RES\\NPC\\test", "test", 1, RGB(255, 255, 255)), DIALOG_AVATAR_NAME_STUDENTG, DIALOG_DATA_STGHAVEBREAKFAST));
		npc[3].push_back(new CNPC1(CPoint(700, 388), BitmapPath("RES\\NPC\\test", "test", 1, RGB(255, 255, 255)), "zy", DIALOG_DATA_STGSUBMIT));
	}
	#pragma endregion


	#pragma region - CEventManager -
	CEventManager::CEventManager()
	{
		gameState = NULL;
	}

	CEventManager::~CEventManager()
	{
	}

	void CEventManager::SetGameStateRun(CGameStateRun *state)
	{
		if (gameState == NULL)
		{
			gameState = state;
		}
	}
	void CEventManager::trigger()
	{
		if (gameState != NULL)
		{
			#pragma region - local variable -
			int nowMap = gameState->mapManager.GetNowMap();
			int rolePosition = ScreenX(gameState->mapManager.GetX1(), gameState->role.GetX3());
			#pragma endregion

			#pragma region - dialog - tips -
			if (tips)
			{
				CDialogManager::Instance()->Start(Tips);
				tips = false;
			}
			#pragma endregion

			#pragma region - dialog - vs xingting -
			if (!dialogWithXingting && nowMap == BOSS_MAP_XINGTING)
			{
				if (rolePosition >= 100)
				{
					CDialogManager::Instance()->Start(DIALOG_DATA_VSXingting1);
					dialogWithXingting = false;
					gameState->bossManager.SetBattle(true);
				}
			}
			#pragma endregion
		}
	}
	#pragma endregion

	#pragma region  - CEndManager -
	CEndManager CEndManager::endManager;
	CEndManager::CEndManager()
	{
		Initialize();
	}

	CEndManager::~CEndManager()
	{
	}

	void CEndManager::Initialize()
	{
		if (isLoadEnd == false)
		{
			LoadEnd();
			isLoadEnd = true;
		}

		nowEnd = nullptr;
		isEnding = false;
		step = 0;
		alpha = 0;
		time_remaining = CTimer(1.0);
		isFadeIn = true; 
		isFadeOut = false;
		isOpenDialog = false;
	}

	void CEndManager::LoadEnd()
	{
		endmap[END_NAME_WINXINGTING] = CEnd(END_NAME_WINXINGTING);
		endmap[END_NAME_LOSEXINGTING] = CEnd(END_NAME_LOSEXINGTING);
	}

	void CEndManager::OnCycle()
	{
		Play();
	}

	void CEndManager::Start(string endName)
	{
		nowEnd = &endmap[endName];
		isEnding = true;
		endBmp.SetBmp(nowEnd->GetBmpPath(0));
		step = 0;
		isFadeIn = true;
		isFadeOut = false;
	}
	
	void CEndManager::Play()
	{
		if (nowEnd->GetBmpPath(step) != END_EOF) //取得的路徑位置還還不是空路徑
		{

			if (isFadeIn)
			{
				endBmp.FadeIn();
				if (endBmp.GetAlpha() >= 255)
				{
					if (!CDialogManager::Instance()->GetDialogState())
					{
						if (nowEnd->GetTxt(step) != END_EOF && isOpenDialog == false) //正常結局+對話 (中的對話)
						{
							CDialogManager::Instance()->Start(nowEnd->GetTxt(step));
							isOpenDialog = true;
						}
						else if (nowEnd->GetTxt(step) == END_EOF && isOpenDialog == false) //僅有結局，沒有對話 (重設計時器)
						{
							time_remaining.ResetTime(1.5);
							isOpenDialog = true; //! 開啟true後 下一個thread會進入到下方的else並計時
						}
						else
						{
							time_remaining.CountDown(); //對話結束後 持續一段時間 才切換下一張
							if (time_remaining.IsTimeOut()) //1.5s 時間到 換狀態
							{
								isFadeIn = false;
								isFadeOut = true;
								isOpenDialog = false;
								time_remaining.ResetTime(1.0);
							}
						}
					}
				}
			}
			else if (isFadeOut)
			{
				endBmp.FadeOut();
				if (endBmp.GetAlpha() <= 0) //圖片完全透明 換狀態 換圖片
				{
					step++;
					if (nowEnd->GetBmpPath(step) != END_EOF) //更改step 在做一次防呆
					{
						endBmp.SetBmp(nowEnd->GetBmpPath(step));
						isFadeIn = true;
						isFadeOut = false;
					}
				}
			}
			endBmp.DrawImage();
		}
		else
		{
			Stop();
		}
	}
	
	void CEndManager::Stop()
	{
		isEnding = false;
	}

	#pragma endregion

}
