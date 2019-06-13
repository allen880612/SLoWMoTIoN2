#pragma once
#include "stdafx.h"
#include "Resource.h"
#include "Refactor.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <map>
#include <sstream>
#include <fstream>
#include "mygame.h"
#include "CManager.h"
using namespace myLibrary;

namespace game_framework
{
	#pragma region - mapManager -
		CMapManager::CMapManager()
		{
			max_map_number = getFolerFileNumber("RES\\Map\\Information\\");
			InitializeCBlockMap();
		}

		CMapManager::~CMapManager()
		{
		}

		vector<CMonster*>* CMapManager::GetPasserby()
		{
			return &passerbyManager.monsterManager;
		}

		void CMapManager::DeletePasserby(vector<CMonster*>::iterator passerbyj)
		{
			passerbyManager.DeleteMonster(passerbyj);
		}

		void CMapManager::AddPasserby()
		{
			CMonster* newPasserby = passerbyManager.AddMonster(blockMap[nowMap].passerbyID, background.Width());
			newPasserby->SetBlock(GetBlockVector());
			passerbyManager.monsterManager.push_back(newPasserby);
		}

		void CMapManager::Initialize()
		{
			ReloadBlockMap(); //每次init都檢查是否要重新載入blockmap
			nowMap = NOW_MAP;
			loadMap = blockMap[nowMap].loadMap;
			x = 0;
			layer.SetLayer(0);
			background = blockMap[nowMap].backgroundBitmap;
			background.SetTopLeft(0, 0);
			CLayerManager::Instance()->AddObject(&background, layer.GetLayer());
	#pragma region - 設置block -

			for (int i = 0; i < max_map_number; i++)
			{
				for (vector<CBlock>::iterator bkiter = blockMap[i].block.begin(); bkiter != blockMap[i].block.end(); bkiter++)
				{
					bkiter->bmp.SetValid(false | (i == nowMap));
					bkiter->SetXY(bkiter->x, bkiter->y);
					CLayerManager::Instance()->AddObject(&(bkiter->bmp), BLOCK_LAYER);
				}
			}

	#pragma endregion
	#pragma region - 設置Door -

			for (int i = 0; i < max_map_number; i++)
			{
				for (vector<CDoor>::iterator bkiter = blockMap[i].door.begin(); bkiter != blockMap[i].door.end(); bkiter++)
				{
					bkiter->bmp.SetValid(false | (i == nowMap));
					bkiter->SetXY(bkiter->x, bkiter->y);
					CLayerManager::Instance()->AddObject(&(bkiter->bmp), BLOCK_LAYER);
				}
			}

	#pragma endregion
			passerbyManager.CreateMonster(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width(), GetBlockVector());
		}

		void CMapManager::ReloadBlockMap()
		{
			bool isreloadDataExist = false; //先假設檔案不存在
			fstream reloadData;
			reloadData.open("RES\\Map\\ReloadMapInformation.txt");
	#pragma region - 檔案存在 - reload -

			if (reloadData.is_open()) //檔案存在
			{
				isreloadDataExist = true;
				string line; //temp string
	#pragma region -- 檢查是否有新增的地圖 --
				reloadData >> line; // load first string in reloadData
				int nowMapNumber = ConvertStringToInt(line); //現在Map - Information有的地圖總數
				int nowBlockSize = blockMap.size();

				if (nowBlockSize < nowMapNumber) //blockMap的size 比 目前的地圖總數小 > 新增地圖
				{
					max_map_number = nowMapNumber;

					for (int mapIndex = nowBlockSize; mapIndex < nowMapNumber; mapIndex++)
					{
						blockMap.push_back(CBlockMap(mapIndex));
					}
				}

	#pragma endregion
	#pragma region -- 重新載入編輯過的地圖 --

				while (reloadData >> line)
				{
					if (line != "")
					{
						int reloadMapIndex = ConvertStringToInt(line); //取得要重新載入的地圖編號

						if (reloadMapIndex < nowBlockSize) //small small 防呆
						{
							blockMap[reloadMapIndex] = CBlockMap(reloadMapIndex);
						}
					}
				}

	#pragma endregion
	#pragma region -- 新增&重新載入完成 - 載入新的圖片 --
				LoadMapBitmap();
	#pragma endregion
			}

	#pragma endregion
			reloadData.close();
	#pragma region - 檔案存在 - 刪檔 -

			if (isreloadDataExist)
			{
				DeleteFile("RES\\Map\\ReloadMapInformation.txt");
			}

	#pragma endregion
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
			return passerbyManager.monsterManager.size();
		}

		int CMapManager::GetNpcLayer(int npcIndex)
		{
			return passerbyManager.monsterManager[npcIndex]->layer.GetLayer();
		}

		bool CMapManager::GetNpcValid(int npcIndex)
		{
			return passerbyManager.monsterManager[npcIndex]->GetValid();
		}

		vector<CBlock>* CMapManager::GetBlockVector()
		{
			return &blockMap[nowMap].block;
		}

		vector<CDoor>* CMapManager::GetDoorVector()
		{
			return &blockMap[nowMap].door;
		}

		CMovingBitmap* CMapManager::GetBitmap()
		{
			return &background;
		}

		CAnimate* CMapManager::GetNpc(int npcIndex)
		{
			return (passerbyManager.monsterManager[npcIndex]->GetAnimate());
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
	#pragma region - 設置block -

			//關掉原本的
			for (vector<CBlock>::iterator bkiter = blockMap[nowMap].block.begin(); bkiter != blockMap[nowMap].block.end(); bkiter++)
			{
				bkiter->bmp.SetValid(false);
			}

			//打開後來的
			for (vector<CBlock>::iterator bkiter = blockMap[changeMap].block.begin(); bkiter != blockMap[changeMap].block.end(); bkiter++)
			{
				bkiter->bmp.SetValid(true);
			}

	#pragma endregion
	#pragma region - 設置Door -

			//關掉原本的
			for (vector<CDoor>::iterator bkiter = blockMap[nowMap].door.begin(); bkiter != blockMap[nowMap].door.end(); bkiter++)
			{
				bkiter->bmp.SetValid(false);
			}

			//打開後來的
			for (vector<CDoor>::iterator bkiter = blockMap[changeMap].door.begin(); bkiter != blockMap[changeMap].door.end(); bkiter++)
			{
				bkiter->bmp.SetValid(true);
			}

	#pragma endregion
			nowMap = changeMap;
			loadMap = blockMap[nowMap].loadMap;
			background = blockMap[nowMap].backgroundBitmap;
			CCamera::Instance()->Reset();

			if (nextMap == "left") //下一張地圖(要換的地圖)，在原本地圖的左邊 (移動到左邊的地圖)
			{
				CCamera::Instance()->SetXY((GetBitmapWidth() - SIZE_X), 0);
			}
			else if (nextMap == "right")  //下一張地圖(要換的地圖)，在原本地圖的右邊 (移動到右邊的地圖)
			{
				CCamera::Instance()->SetXY(0, 0);
			}

			background.SetTopLeft(x, 0);
			passerbyManager.Clear();
			passerbyManager.CreateMonster(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width(), GetBlockVector());
			x = CCamera::Instance()->GetX();
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
	#pragma region - 螢幕上的 background 要移動 -
			int dx = CCamera::Instance()->GetX();
			x = -dx;
			SetXY(x, 0);
	#pragma endregion
	#pragma region - 螢幕上的 block 也要移動 -

			for (vector<CBlock>::iterator bkiter = blockMap[nowMap].block.begin(); bkiter != blockMap[nowMap].block.end(); bkiter++)
			{
				bkiter->SetXY(bkiter->x, bkiter->y, dx);
			}

	#pragma endregion
	#pragma region - 螢幕上的 door 也要移動 -

			for (vector<CDoor>::iterator bkiter = blockMap[nowMap].door.begin(); bkiter != blockMap[nowMap].door.end(); bkiter++)
			{
				bkiter->SetXY(bkiter->x, bkiter->y, dx);
			}

	#pragma endregion
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
			return x < 0 || x >((background.Width()) - 640);
		}

		void CMapManager::OnShow() //顯示對應到的blockMap圖片 (nowMap = 1, 顯示blockMap[1]的background, 類推)
		{
			background.SetTopLeft(0, 0);
			background.ShowBitmap();
		}

		void CMapManager::InitializeCBlockMap()
		{
			for (int mapIndex = 0; mapIndex < max_map_number; mapIndex++) //初始化blockMap的上下左右地圖資訊，增加可讀性使用switch敘述
			{
				blockMap.push_back(CBlockMap(mapIndex));
			}
		}

		void CMapManager::LoadMapBitmap() //如字面意思，LoadMapBitmap，在GameStateRun:OnInit運行，一次性load blockMap的所有圖片
		{
			for (int mapIndex = 0; mapIndex < max_map_number; mapIndex++)
			{
				if (!blockMap[mapIndex].isLoad)
				{
					blockMap[mapIndex].LoadImg();
					blockMap[mapIndex].isLoad = true;
				}
			}
		}
	#pragma endregion

	class CDialogManager;
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
				layerInteger[i].clear();
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

		void CLayerManager::AddObject(CInteger* object, int targetLayer)
		{
			layerInteger[targetLayer].push_back(object);
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

				for (vector<CInteger*>::iterator k = layerInteger[i].begin(); k != layerInteger[i].end();)
				{
					if ((*k)->IsNull())
					{
						k = layerInteger[i].erase(k);
					}
					else
					{
						if ((*k)->GetValid())	//if valid then show
						{
							(*k)->ShowBitmap();
						}

						k++;
					}
				}

				if (CDialogManager::Instance()->GetDialogState() && i == 8)
				{
					CDialogManager::Instance()->ShowText();
				}
			}
		}

		CLayerManager* CLayerManager::Instance()
		{
			return &layerManager;
		}

		void CLayerManager::Initialize()
		{
			Clear();
		}
	#pragma endregion

	#pragma region - passerbyManager -
	CMonsterManager::CMonsterManager()
	{
		layer.SetLayer(4);
		monsterManager.clear();
	}

	CMonsterManager::~CMonsterManager()
	{
		Clear();
	}

	void CMonsterManager::Clear()
	{
		for (vector<CMonster*>::iterator it = monsterManager.begin(); it != monsterManager.end(); it++)
		{
			delete *it;
			(*it) = NULL;
		}

		vector<CMonster*> del;
		monsterManager.swap(del);
		monsterManager.clear();
	}

	CMonster* CMonsterManager::GetMonsterType(int randomID)
	{
		if (randomID == 0)
		{
			return new CMonsterType1(0, 0, BitmapPath("RES\\Role\\NPC\\LUKA", "LUKA", 2, RGB(214, 214, 214)), 10); //先創建一個default passerby
		}
		else if (randomID == 1)
		{
			return new CMonsterType1(0, 0, BitmapPath("RES\\Role\\NPC\\RIN", "RIN", 2, RGB(214, 214, 214)), 1); //先創建一個default passerby
		}
		else if (randomID == 2)
		{
			return new CMonsterType2(0, 0, BitmapPath("RES\\Role\\NPC\\mushroom", "mushroom", 5, RGB(214, 214, 214)), 20); //先創建一個default passerby
		}
		else if (randomID == 3)
		{
			return new CMonsterType2(0, 0, BitmapPath("RES\\Role\\NPC\\faqai", "faqai", 5, RGB(255, 255, 255)), 87); //先創建一個default passerby
		}

		return new CMonsterType1(0, 0, BitmapPath("RES\\Role\\NPC\\LUKA", "LUKA", 2, RGB(214, 214, 214)), 10); //先創建一個default passerby
	}

	CMonster* CMonsterManager::AddMonster(vector<int> id, int mapWidth)
	{
#pragma region Create a Passerby
		int randomID = GetRandom(0, id.size() - 1); //random 決定 passerby種類 (1號or2號)
		CMonster* newPasserby = GetMonsterType(id[randomID]);
		int randomX = GetRandom(0, mapWidth - newPasserby->Width()); //random 決定passerby的出現位置
		newPasserby->SetXY(randomX, -newPasserby->Height()); //set passerby x, y
		return newPasserby;
#pragma endregion
	}

	void CMonsterManager::CreateMonster(int createNumber, vector<int> id, int mapWidth, vector<CBlock>* bkvector)
	{
		for (int i = 0; i < createNumber; i++)
		{
			CMonster* newPasserby = AddMonster(id, mapWidth);
			newPasserby->SetBlock(bkvector);
			newPasserby->SetValid(false);
			monsterManager.push_back(newPasserby);
		}
	}

	void CMonsterManager::DeleteMonster(vector<CMonster*>::iterator passerbyj)
	{
		delete *passerbyj;
		*passerbyj = NULL;
		passerbyj = monsterManager.erase(passerbyj);
	}

	vector<CMonster*> CMonsterManager::GetMonster()
	{
		return monsterManager;
	}
	#pragma endregion

	#pragma region - CDialogManager -
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
	#pragma region - Load avatar -
			vector<string> avatarName;
			string avatarFolderPath = "RES\\Dialog\\Avatar\\";
			getFolderFile(avatarFolderPath, &avatarName);

			for (unsigned i = 0; i < avatarName.size(); i++)
			{
				int strLength = avatarName[i].length();
				string ext = avatarName[i].substr(strLength - 4, strLength);

				if (ext == ".bmp")
				{
					string avatar = getFileName(avatarName[i]);
					dialogAvatar[avatar] = CMovingBitmap();
					char* address = ConvertCharPointToString((avatarFolderPath + avatarName[i]));
					dialogAvatar[avatar].LoadBitmap(address, RGB(214, 214, 214));
					delete address;
				}
			}

	#pragma endregion
	#pragma region - load image -
	#pragma region - load dialog background -
			dialog_background.LoadBitmap("RES\\Dialog", "ground", RGB(255, 255, 255));
	#pragma endregion
	#pragma region - load null -
			avatar_null.LoadBitmap("RES\\Dialog\\Avatar", "null");
	#pragma endregion
	#pragma region - load next animation -
			textNext.LoadBitmap("RES\\Dialog\\next", "next", 4, DIALOG_BACKGROUND_COLOR);
			textNext.SetTopLeft(500, 450);
	#pragma endregion
	#pragma endregion
			IsBitmapLoaded = true;
		}

		void CDialogManager::LoadDialog()
		{
			vector<string> dialogTxt;
			string dialogFolderPath = "RES\\Dialog\\Txt\\";
			getFolderFile(dialogFolderPath, &dialogTxt);

			for (unsigned int i = 0; i < dialogTxt.size(); i++)
			{
				string dialogFileName = getFileName(dialogTxt[i]);
				dialogmap[dialogFileName] = CDialog(dialogFolderPath + dialogTxt[i], dialogFileName);
			}

			IsDialogLoad = true;
		}


		void CDialogManager::LoadDialog(string dialogName, string loadPath)
		{
			dialogmap[dialogName] = CDialog(loadPath, dialogName);
		}

		void CDialogManager::ShowText_Next()
		{
			if (IsPrintTips)
			{
				textNext.SetValid(true);
				textNext.OnMove();
			}
			else
			{
				textNext.SetValid(false);
			}
		}

		void CDialogManager::DialogWithSound()
		{
			if (nowTxtName == DIALOG_DATA_FAQAI)
			{
				if (step == 3 || step == 5)
				{
					CAudio::Instance()->Play("faqai");
				}
			}
			else if (nowTxtName == "roleWinFacaiSeed")
			{
				if (step == 1)
				{
					CAudio::Instance()->Play("faqai");
				}
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
			nowDialog = NULL;
			step = 0;
			nowShowTextSize = 0;
			IsPrintTips = false;
			musicFromNPC = "";
			AddShowTextTimer.ResetTime(DIALOG_ADDTEXT_TIME);
		}

		CDialogManager* CDialogManager::Instance()
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

			nowTxtName = mode;
			IsDialoging = true;
			IsPrintTips = false;
			dialog_background.SetValid(true);
			Dialog();
		}

		void CDialogManager::Next()
		{
			if (IsDialoging)
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
			if (nowDialog != NULL)
			{
				avatar = avatar_null;
				dialog_background.SetValid(false);
				textNext.SetValid(false);
				IsDialoging = false;
				IsPrintTips = false;
				nowDialog->SetTriggered();
				nowDialog = NULL;
				nowShowTextSize = 0;
				step = 0;
				nowTxtName = "";

				if (musicFromNPC != "")
				{
					CAudio::Instance()->Stop(musicFromNPC);
					CAudio::Instance()->Play("SLoWMoTIoN_Game");
					musicFromNPC = "";
				}
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
						char* tteemmppkkk = new char[DIALOG_MAX_TEXT + 5];
						strcpy(tteemmppkkk, tempk);
						split_showtext.push_back(tteemmppkkk);
						charindex = 0;
						memset(tempk, '\0', sizeof(tempk));
					}
				}

				char* tteemmppkkk = new char[DIALOG_MAX_TEXT + 5];
				strcpy(tteemmppkkk, tempk);
				split_showtext.push_back(tteemmppkkk);
	#pragma endregion
	#pragma region - Set Print Text Color -
				COLORREF txtColor = DIALOG_TEXT_COLOR;

				if (nowDialog != NULL)
				{
					txtColor = nowDialog->GetDialogColor(step);
				}

	#pragma endregion
	#pragma region - draw text -

				for (unsigned int i = 0; i < split_showtext.size(); i++)
				{
					PaintText(split_showtext[i], avatar.Left() + avatar.Width() + MARGIN_DIALOG_TEXT_X, avatar.Top() + MARGIN_DIALOG_TEXT_Y + i * 35, "微軟正黑體", DIALOG_TEXT_SIZE, txtColor, DIALOG_BACKGROUND_COLOR);
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
				DialogWithSound();
			}
			else
			{
				step = 0;
				showtext = "";
				Stop();
			}
		}
	#pragma endregion

	#pragma region - BossManager -
		CBossManager::CBossManager()
		{
	#pragma region -- Create Boss --
			bossInformation[BOSS_XINGTING] = new CXingting(450, 250, 8787, "Xingting", BitmapPath("RES\\Boss\\xingting", "xingting", 2, RGB(214, 214, 214)));
			bossInformation[BOSS_FACAISEED] = new CFacaiSeed(100, 270, 35, BOSS_FACAISEED, BitmapPath("RES\\Boss\\FacaiSeed", "faqai", 19, RGB(255, 255, 255)));
	#pragma endregion
			targetBoss = NULL;
			isBattle = false;
		}

		CBossManager::~CBossManager()
		{
			if (targetBoss != NULL)
			{
				targetBoss->Clear();
			}

			Clear();
		}

		void CBossManager::Initialize() //CGameRun::OnBeginState時 initialize
		{
			//(second(value)為指標，所以second後面用-> )
			for (map<string, CBoss*>::iterator bossiter = bossInformation.begin(); bossiter != bossInformation.end(); bossiter++)
			{
				bossiter->second->Initialize(); //初始化boss
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
			if (nowMap == BOSS_MAP_XINGTING && bossInformation[BOSS_XINGTING]->GetAlive())
			{
				targetBoss = bossInformation[BOSS_XINGTING];
				targetBoss->Initialize();
				targetBoss->GetAnimate()->SetValid(true);
			}
			else if (nowMap == BOSS_MAP_FACAISEED && (bossInformation[BOSS_FACAISEED]->GetAlive() && !bossInformation[BOSS_FACAISEED]->IsEnd()))
			{
				targetBoss = bossInformation[BOSS_FACAISEED];
				targetBoss->Initialize();
				targetBoss->GetAnimate()->SetValid(true);
			}
			else
			{
				if (targetBoss != NULL)
				{
					targetBoss->GetAnimate()->SetValid(false);
					targetBoss->Clear();
					isBattle = false;
					targetBoss = NULL;
				}
			}
		}

		void CBossManager::BossDead()
		{
			targetBoss->Clear();
			targetBoss = NULL;
			SetBattle(false);
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
			}
		}

		void CNPCManager::Initialize(int nowMap)
		{
			map_max_Number = getFolerFileNumber("RES\\Map\\Information\\");

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

		void CNPCManager::OnCycle(int thisMap, CPoint rolePoint)
		{
			for (vector<CNPC*>::iterator npciter = npc[thisMap].begin(); npciter != npc[thisMap].end(); npciter++)
			{
				(*npciter)->OnCycle(rolePoint);
			}
		}

		bool CNPCManager::IsTotalTalk()
		{
			
			for (int i = 0; i < MAX_MAP_NUMBER; i++)
			{
				for (vector<CNPC*>::iterator npciter = npc[i].begin(); npciter != npc[i].end(); ++npciter)
				{
					if (!(*npciter)->IsTalked())
					{
						return false;
					}
				}
			}
			return true;
		}

		void CNPCManager::LoadNPC()
		{
			npc[4].push_back(new CNPC1(CPoint(50, 388), BitmapPath("RES\\NPC\\test", "test", 1, RGB(255, 255, 255)), "frog", FROG));
			npc[4].push_back(new CNPC3(CPoint(450, 388), BitmapPath("RES\\NPC\\test2", "test2", 1, RGB(255, 255, 255)), "deadlock", "MyVoiceIsDead", MyVoiceIsDead));
			npc[3].push_back(new CNPC1(CPoint(50, 350), BitmapPath("RES\\NPC\\studentB", "studentB", 1, RGB(255, 255, 255)), DIALOG_AVATAR_NAME_STUDENTB, DIALOG_DATA_MEETSTB));
			npc[3].push_back(new CNPC1(CPoint(450, 350), BitmapPath("RES\\NPC\\studentG", "studentG", 1, RGB(255, 255, 255)), DIALOG_AVATAR_NAME_STUDENTG, DIALOG_DATA_STGHAVEBREAKFAST));
			npc[3].push_back(new CNPC1(CPoint(560, 90), BitmapPath("RES\\NPC\\faqai", "faqai", 20, RGB(255, 255, 255)), "faqai", DIALOG_DATA_FAQAI));
			npc[7].push_back(new CNPC1(CPoint(400, 360), BitmapPath("RES\\NPC\\an", "an", 6, RGB(214, 214, 214)), "an", "EasyAndroidHomework"));
			npc[8].push_back(new CNPC3(CPoint(680, 360), BitmapPath("RES\\NPC\\remilia_music", "remilia", 4, RGB(254, 254, 254)), "remilia", "Septette", "Music_Septette"));
			npc[8].push_back(new CNPC3(CPoint(900, 360), BitmapPath("RES\\NPC\\flandre_music", "flandre", 4, RGB(214, 214, 214)), "flandre", "UN_OwenWasHer", "Music_UN"));
			npc[8].push_back(new CNPC3(CPoint(340, 340), BitmapPath("RES\\NPC\\sakuya_music", "sakuya", 6, RGB(214, 214, 214)), "sakuya", "FloweringNight", "Music_FloweringNight"));
			npc[8].push_back(new CNPC3(CPoint(120, 360), BitmapPath("RES\\NPC\\suwako_music", "suwako", 2, RGB(214, 214, 214)), "suwako", "NativeFaith", "Music_NativeFaith"));
			npc[10].push_back(new CNPC1(CPoint(100, 360), BitmapPath("RES\\NPC\\mushroom", "mushroom", 4, RGB(214, 214, 214)), "mushroom", "mushroom"));
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

		void CEventManager::Initialize()
		{
			if (gameState != NULL)
			{
				dialogWithXingting = false;
				dialogWithFacaiSeed = false;
			}
		}

		void CEventManager::SetGameStateRun(CGameStateRun* state)
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

				if (!dialogWithXingting && nowMap == BOSS_MAP_XINGTING && gameState->role.IsDead() == false && gameState->bossManager.IsBattle() == false)
				{
					if (rolePosition >= 100)
					{
						CDialogManager::Instance()->Start(DIALOG_DATA_VSXingting1);
						dialogWithXingting = true;
						gameState->bossManager.SetBattle(true);
						gameState->SwitchTimer(gameState->bossManager.targetBoss->GetAliveTimer());
					}
				}

	#pragma endregion
	#pragma region - dialog - with FacaiSeed -

				if (!dialogWithFacaiSeed && nowMap == BOSS_MAP_FACAISEED && gameState->role.IsDead() == false && gameState->bossManager.IsBattle() == false)
				{
					if (rolePosition <= 540)
					{
						CDialogManager::Instance()->Start("roleVsFacaiSeed1");
						dialogWithFacaiSeed = true;
						gameState->bossManager.SetBattle(true);
						gameState->SwitchTimer(gameState->bossManager.targetBoss->GetAliveTimer());
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
	#pragma region - open file to load alpha -
			fstream alphaData;
			alphaData.open("RES\\End\\vsAlphaBlend.txt");
			string line1, line2;
			alphaData >> line1 >> line2;
			int fadein = ConvertStringToInt(line1);
			int fadeout = ConvertStringToInt(line2);
			alphaData.close();
	#pragma endregion
			endBmp.SetFadeInOut(fadein, fadeout);
			Initialize();
		}

		CEndManager::~CEndManager()
		{
			//防止map更改HaveEnd的順序 > 先讀取 在寫入
	#pragma region - load -
			fstream haveEnd_open;
			string tempName, nonstr;
			vector<string> endName;
			haveEnd_open.open("RES\\End\\HaveEnd.txt", ios::in);

			while (haveEnd_open >> tempName >> nonstr)
			{
				endName.push_back(tempName);
			}

			haveEnd_open.close();
	#pragma endregion
	#pragma region - write -
			fstream haveEnd_write;
			haveEnd_write.open("RES\\End\\HaveEnd.txt", ios::out);

			for (unsigned int i = 0; i < endName.size(); i++)
			{
				string writeLine = endName[i] + " " + (endmap[endName[i]].IsGetEnd() ? "true" : "false");
				haveEnd_write << writeLine << "\n";
			}

			haveEnd_write.close();
	#pragma endregion
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
			endmap[END_NAME_SALTEDFISH] = CEnd(END_NAME_SALTEDFISH);
			endmap[END_NAME_WIN_FACAISEED] = CEnd(END_NAME_WIN_FACAISEED);
			endmap[END_NAME_LOSE_FACAISEED] = CEnd(END_NAME_LOSE_FACAISEED);
			endmap[END_NAME_PERFECT_LINK] = CEnd(END_NAME_PERFECT_LINK);
			endmap[END_NAME_POINT_999] = CEnd(END_NAME_POINT_999);
			endmap[END_NAME_POINT_OVERFLOW] = CEnd(END_NAME_POINT_OVERFLOW);
	#pragma region - open HaveEnd.txt to Load The End is Get -
			fstream haveEnd;
			haveEnd.open("RES\\End\\HaveEnd.txt", ios::in);
			string lineData;
			map<string, CEnd>::iterator iter;

			while (getline(haveEnd, lineData))
			{
				//lineInfo[0] = name, [1] = have end (true or false)
				vector<string> lineInfo = SplitString(lineData);
	#pragma region - find lineInfo[0] is the key of map -
				iter = endmap.find(lineInfo[0]);

				if (iter != endmap.end()) //map have this key
				{
					iter->second.SetGetEnd(ConvertStringToBoolen(lineInfo[1]));
				}

	#pragma endregion
			}

			haveEnd.close();
	#pragma endregion
		}

		void CEndManager::OnCycle()
		{
			Play();
		}

		bool CEndManager::IsPassEnd(string endName)
		{
			if (!endmap.count(endName))
				return false;

			return endmap[endName].IsGetEnd();
		}

		vector<string> CEndManager::GetAllEndName()
		{
			vector<string> endName;

			for (map<string, CEnd>::iterator iter = endmap.begin(); iter != endmap.end(); iter++)
			{
				endName.push_back(iter->first);
			}

			return endName;
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
				nowEnd->SetGetEnd();
				Stop();
			}
		}

		void CEndManager::Stop()
		{
			isEnding = false;
		}

	#pragma endregion

	#pragma region - Button Manager -
		CButtonManager::CButtonManager()
		{
			Initialize();
		}

		CButtonManager::~CButtonManager()
		{
			Clear();
		}

		void CButtonManager::Load()
		{
			for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
			{
				(btniter->second)->LoadBitmap();
			}
		}

		void CButtonManager::Initialize()
		{
			isValid = false;

			for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
			{
				(btniter->second)->Initialize();
			}
		}

		void CButtonManager::Clear()
		{
			for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
			{
				delete btniter->second;
				btniter->second = NULL;
			}

			buttons.clear();
		}

		void CButtonManager::AddButton(CButton* _button)
		{
			if (_button->name != "")
				buttons[_button->name] = _button;
		}

		void CButtonManager::CreateButton(BitmapPath _loadpath, CPoint point, bool _state, bool _needCollision)
		{
			AddButton(new CButton(_loadpath, point, _state, _needCollision));
		}

		void CButtonManager::CreateButton(CButton* button)
		{
			AddButton(button);
		}

		void CButtonManager::ClickButton(string _btnName)
		{
			(buttons[_btnName])->ClickButton();
		}

		bool CButtonManager::IsCollisionMouse(string _btnName)
		{
			return (buttons[_btnName])->IsCollisionMouse(mouse);
		}

		bool CButtonManager::GetState(string _btnName)
		{
			return (buttons[_btnName])->state;
		}

		string CButtonManager::GetCollisionButtonName()
		{
			for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
			{
				if (IsPointInRect(mouse, btniter->second->GetAnimate()->GetRect())) // 有碰撞
				{
					return btniter->first;
				}
			}

			return "NoButtonClick";
		}

		void CButtonManager::UpdateState(CPoint _m)
		{
			mouse = _m;

			for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
			{
				(btniter->second)->CollisonMouse(_m);
			}
		}

		void CButtonManager::ShowButton()
		{
			for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
			{
				(btniter->second)->OnShow();
			}
		}

		void CButtonManager::OnCycle()
		{
			for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
			{
				(btniter->second)->OnMove();
				(btniter->second)->SetValid(isValid);
			}
		}
	#pragma endregion

	#pragma region - CMapEditer -
		CMapEditer::CMapEditer()
		{
			arrow[0] = Arrow("up");
			arrow[1] = Arrow("down");
			arrow[2] = Arrow("left");
			arrow[3] = Arrow("right");
		}

		CMapEditer::~CMapEditer()
		{
			mapObj.clear();

			if (!blockMap.empty())
			{
				StoreMapInformaion();
			}
		}

		void CMapEditer::Initialize()
		{
			nowMap = getFolerFileNumber("RES\\Map\\Information\\");
	#pragma region - blockMap - ReLoad -
			blockMap.clear();

			for (int i = 0; i < nowMap; i++)
			{
				blockMap.push_back(CBlockMap(i));
				blockMap[i].LoadImg();
			}

			selectNowMap = 0;
			selectMapMode = "none";
	#pragma endregion
	#pragma region - Load arrow -

			for (int i = 0; i < 4; i++)
			{
				arrow[i].Initialize();
			}

			SetArrowCanShow();
	#pragma endregion
			LoadReloadMapInformation();
			NewMapInit();
		}

		void CMapEditer::NewMapInit()
		{
			mapObj.clear(); //ImageInfo - block
			bkmap.Initialize();
			printNowMap = &nowMap;
			isPrintNowMap = false;
			saveTxtName = EDITER_PRESET_SAVETXTNAME;
			isMouseDown = false;
			isSaved = false;
			haveBG = false;
			isMapRight = isMapLeft = false;
			selectObj = NULL;
			cameraX = 0;
		}

		void CMapEditer::AddImage(vector<string> path)
		{
			//類型 load (load map)
			//類型(背景 / block), 圖片路徑
			if (path[0] == "background")
			{
				haveBG = true;
				ImageInfo tempcv;
				tempcv = ImageInfo(path[1], "background");
				background = tempcv;
				background.SetXY(0, 0, 0);
			}
			else if (path[0] == "block")
			{
				ImageInfo tempObj = ImageInfo(path[1], "block");
				tempObj.SetXY(cameraX, tempObj.y);
				mapObj.push_back(tempObj);
			}
			else if (path[0] == "load")
			{
				NewMapInit();
				LoadBlockMap(path[1]);
				saveTxtName = path[1];
				background.SetXY(0, 0, 0);
			}
			else if (path[0] == "upDoor" || path[0] == "downDoor")
			{
				ImageInfo tempObj = ImageInfo(path[1], path[0]);
				tempObj.SetXY(cameraX, tempObj.y);
				mapObj.push_back(tempObj);
			}
		}

		void CMapEditer::SetMapMoveDir(string dir)
		{
			if (dir == "right")
			{
				isMapRight = true;
			}
			else if (dir == "left")
			{
				isMapLeft = true;
			}
			else if (dir == "cleft")
			{
				isMapLeft = false;
			}
			else if (dir == "cright")
			{
				isMapRight = false;
			}
		}

		void CMapEditer::OnSave()
		{
			if (IsInSelectMapMode())
			{
				if (selectMapMode == "up")
				{
					bkmap.upMap = selectNowMap;
					blockMap[selectNowMap].downMap = nowMap;
				}
				else if (selectMapMode == "down")
				{
					bkmap.downMap = selectNowMap;
					blockMap[selectNowMap].upMap = nowMap;
				}
				else if (selectMapMode == "left")
				{
					bkmap.leftMap = selectNowMap;
					blockMap[selectNowMap].rightMap = nowMap;
				}
				else if (selectMapMode == "right")
				{
					bkmap.rightMap = selectNowMap;
					blockMap[selectNowMap].leftMap = nowMap;
				}

				reloadMap[selectNowMap] = true;
				SetSelectMapMode("none");
			}
			else
			{
				bkmap.nowMap = nowMap;
				CreateObjToBkmap();

				if (!isSaved) //沒有儲存過地圖
				{
					CString saveDir = ".txt";
					string tempSaveName = "map" + std::to_string(nowMap) + ".txt"; //緩衝
					CString saveName = tempSaveName.c_str();
					CString saveExt = "txt (*.txt)|*.txt||";
					CFileDialog saveDlg(false, saveDir, saveName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, saveExt);
					int result = saveDlg.DoModal();

					if (result == IDOK)
					{
						saveTxtName = saveDlg.GetFileName();
						isSaved = true;
						nowMapNumber++;
						bkmap.CreateInformation(saveTxtName);
					}

					if (result == IDCANCEL)
					{
						return;
					}
				}

				if (nowMap < (int)reloadMap.size())
				{
					reloadMap[nowMap] = true;
				}

	#pragma region - bkmap write to blockmap -

				if (nowMap < (int)blockMap.size()) //修改一張地圖
				{
					blockMap[nowMap] = bkmap;
				}
				else //要新增一張地圖
				{
					blockMap.push_back(bkmap);
				}

	#pragma endregion
			}
		}

		void CMapEditer::CreateObjToBkmap()
		{
			bkmap.block.clear();
			bkmap.door.clear();
			bkmap.loadPath = background.path;
	#pragma region - Load Map Simple Object -

			for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
			{
				if (mbiter->id == "block")
				{
					bkmap.block.push_back(CBlock(mbiter->path, mbiter->x, mbiter->y));
				}
				else if (mbiter->id == "upDoor")
				{
					bkmap.door.push_back(CDoor(mbiter->path, mbiter->x, mbiter->y, mbiter->id, bkmap.upMap));
				}
				else if (mbiter->id == "downDoor")
				{
					bkmap.door.push_back(CDoor(mbiter->path, mbiter->x, mbiter->y, mbiter->id, bkmap.downMap));
				}
			}

	#pragma endregion
		}

		void CMapEditer::LoadBlockMap(string mapName)
		{
			sscanf(mapName.c_str(), "map%d.txt", &nowMap);
			bkmap = blockMap[nowMap];
			isSaved = true;
			background = ImageInfo(bkmap.loadPath, "background"); //load background
			haveBG = true;
	#pragma region - Load block -

			for (vector<CBlock>::iterator mbiter = bkmap.block.begin(); mbiter != bkmap.block.end(); mbiter++)
			{
				ImageInfo tempk = ImageInfo(mbiter->path, "block");
				tempk.SetXY(mbiter->x, mbiter->y, cameraX);
				mapObj.push_back(tempk);
			}

	#pragma endregion
	#pragma region - Load Door -

			for (vector<CDoor>::iterator mbiter = bkmap.door.begin(); mbiter != bkmap.door.end(); mbiter++)
			{
				ImageInfo tempk = ImageInfo(mbiter->path, mbiter->GetType());
				tempk.SetXY(mbiter->x, mbiter->y, cameraX);
				mapObj.push_back(tempk);
			}

	#pragma endregion
		}

		void CMapEditer::StoreMapInformaion()
		{
			for (unsigned int i = 0; i < blockMap.size(); i++)
			{
				blockMap[i].CreateInformation();
			}

			blockMap.clear();
		}

		void CMapEditer::OnMove()
		{
			if (isMapRight)
			{
				if (cameraX >= 0 && (cameraX + SIZE_X) < background.bmp.Width())
				{
					cameraX += MOVE_DISTANCE;
				}
				else
				{
					cameraX = background.bmp.Width() - SIZE_X;
				}
			}

			if (isMapLeft)
			{
				if (cameraX > 0 && cameraX + SIZE_X <= background.bmp.Width())
				{
					cameraX -= MOVE_DISTANCE;
				}
				else
				{
					cameraX = 0;
				}
			}

			if (haveBG)
			{
				background.bmp.SetTopLeft(background.x - cameraX, background.y);
			}

			for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
			{
				mbiter->bmp.SetTopLeft(mbiter->x - cameraX, mbiter->y);
			}
		}

		void CMapEditer::OnShow()
		{
			if (!IsInSelectMapMode()) //不在選擇地圖的模式中
			{
				if (haveBG)
				{
					background.bmp.ShowBitmap();
				}

				for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
				{
					mbiter->bmp.ShowBitmap();
				}
			}
			else //選擇地圖中
			{
				if (selectNowMap < (int)blockMap.size())
				{
					blockMap[selectNowMap].backgroundBitmap.ShowBitmap();
				}

				for (int i = 0; i < 4; i++)
				{
					if (arrow[i].CanShow())
					{
						arrow[i].GetBmp()->ShowBitmap();
					}
				}
			}
		}

		void CMapEditer::SelectBlock(CPoint mouse)
		{
			for (vector<ImageInfo>::reverse_iterator mbiter = mapObj.rbegin(); mbiter != mapObj.rend(); mbiter++)
			{
				if (IsPointInRect(mouse, mbiter->bmp.GetRect()))
				{
					selectObj = &(*mbiter);
					SetDPoint_MouseToTopLeft(mouse);
					return;
				}
			}

			selectObj = NULL;
			SetDPoint_MouseToTopLeft();
		}

		void CMapEditer::DragBlock(CPoint mouse)
		{
			if (selectObj == NULL)
				return;

			selectObj->SetXY(mouse.x - dpoint_mouseToTopleft.x, mouse.y - dpoint_mouseToTopleft.y, cameraX);
		}

		void CMapEditer::DeleteBlock()
		{
			if (selectObj != NULL)
			{
				for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
				{
					if (selectObj == &(*mbiter))
					{
						mbiter = mapObj.erase(mbiter);
						selectObj = NULL;
						return;
					}
				}
			}
		}

		string CMapEditer::GetNowMap()
		{
			return "now map: " + std::to_string(*printNowMap);
		}

		void CMapEditer::CreateReloadMapInformation()
		{
			fstream reloadData;
			reloadData.open("RES\\Map\\ReloadMapInformation.txt", ios::out);
			reloadData << std::to_string(nowMapNumber) + "\n"; //write目前有多少張地圖ㄌ

			for (unsigned int i = 0; i < reloadMap.size(); i++)
			{
				if (reloadMap[i])
				{
					reloadData << std::to_string(i) + "\n"; //write 那些地圖要reload
				}
			}

			reloadData.close();
		}

		void CMapEditer::LoadReloadMapInformation()
		{
			fstream reloadData;
			reloadData.open("RES\\Map\\ReloadMapInformation.txt");

			if (!reloadData.is_open()) //開啟失敗
			{
				nowMapNumber = nowMap;
				reloadMap.clear();
				reloadMap.resize(nowMapNumber, false);
			}

			reloadData.close();
		}

		void CMapEditer::SetSelectMapZero()
		{
			selectNowMap = -1;
			OnSave();
		}

		void CMapEditer::SetSelectMapMode(string _mode)
		{
			if (_mode == "up" || _mode == "down" || _mode == "left" || _mode == "right")
				printNowMap = &selectNowMap;
			else
			{
				selectNowMap = 0;
				printNowMap = &nowMap;
			}

			selectMapMode = _mode;
		}

		void CMapEditer::ClickArrow(CPoint _mouse)
		{
			for (int i = 0; i < 4; i++)
			{
				if (arrow[i].CanShow() && IsPointInRect(_mouse, arrow[i].GetBmp()->GetRect()))
				{
					SwitchMap(arrow[i].GetDir());
				}
			}
		}

		void CMapEditer::SwitchMap(string _dir)
		{
			if (_dir == "up")
			{
				GotoMap(blockMap[selectNowMap].upMap);
			}
			else if (_dir == "down")
			{
				GotoMap(blockMap[selectNowMap].downMap);
			}
			else if (_dir == "left")
			{
				GotoMap(blockMap[selectNowMap].leftMap);
			}
			else if (_dir == "right")
			{
				GotoMap(blockMap[selectNowMap].rightMap);
			}
		}

		void CMapEditer::GotoMap(int mapIndex)
		{
			if (mapIndex != -1)
			{
				selectNowMap = mapIndex;
				SetArrowCanShow();
			}
		}

		void CMapEditer::SetArrowCanShow()
		{
			arrow[0].SetCanShow(blockMap[selectNowMap].upMap >= 0 && blockMap[selectNowMap].upMap < (int)blockMap.size());
			arrow[1].SetCanShow(blockMap[selectNowMap].downMap >= 0 && blockMap[selectNowMap].downMap < (int)blockMap.size());
			arrow[2].SetCanShow(blockMap[selectNowMap].leftMap >= 0 && blockMap[selectNowMap].leftMap < (int)blockMap.size());
			arrow[3].SetCanShow(blockMap[selectNowMap].rightMap >= 0 && blockMap[selectNowMap].rightMap < (int)blockMap.size());
		}
	#pragma endregion

	#pragma region - UIManager -
		UIManager::UIManager()
		{
		}

		void UIManager::Load()
		{
			roleStatus.Load();
			bossStatus.Load();
			uiTime.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
			uiScore.LoadBitmap(".\\RES\\Number\\cookiezi2", "default");
		}

		void UIManager::Initialize(CRole* _role, CBossManager* _bManager)
		{
			role = _role;
			bManager = _bManager;
			roleStatus.Initialize(CPoint(0, 0), role->GetHp(), role->GetEq());
			bossStatus.Initialize(CPoint(0, 0));
			uiTime.Initialize(CPoint(250, 0), GAME_TIME, 2);
			uiScore.Initialize(CPoint(150, 60), 0, 3);
			CLayerManager::Instance()->AddObject(&uiTime, INTERFACE_LAYER);
			CLayerManager::Instance()->AddObject(&uiScore, INTERFACE_LAYER);
		}

		void UIManager::OnCycle(int _time)
		{
			uiTime.SetInteger(_time);
			roleStatus.OnCycle(role->GetHp(), role->GetEq());
			bossStatus.OnCycle(bManager);
			uiTime.SetInteger(_time);
			uiScore.SetInteger(role->GetScore());
		}
	#pragma endregion
}