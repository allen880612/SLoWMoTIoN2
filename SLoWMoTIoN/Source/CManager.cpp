#include "stdafx.h"
#include "Resource.h"
#include "Refactor.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <map>
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

	vector<CNPC*>* CMapManager::GetPasserby()
	{
		return &passerbyManager.passerby;
	}

	void CMapManager::DeletePasserby(vector<CNPC*>::iterator passerbyj)
	{
		passerbyManager.DeletePasserby(passerbyj);
	}

	void CMapManager::AddPasserby()
	{
		passerbyManager.passerby.push_back(passerbyManager.AddPasserby(blockMap[nowMap].passerbyID, background.Width()));
	}

	void CMapManager::Initialize()
	{
		nowMap = 2;
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
			x = -(GetBitmapWidth() - SIZE_X);
		}
		else if (nextMap == "right")  //下一張地圖(要換的地圖)，在原本地圖的右邊 (移動到右邊的地圖)
		{
			x = 0; 
		}
		background.SetTopLeft(x, 0);

		passerbyManager.Clear();
		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width());
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
		if (isMovingLeft)
		{
			x += directionX;
			//for(unsigned int npcIndex = 0; npcIndex < blockMap[nowMap].npc.size(); npcIndex++)
				//blockMap[nowMap].npc[npcIndex].SetXY(blockMap[nowMap].npc[npcIndex].GetX1() + directionX, blockMap[nowMap].npc[npcIndex].GetY1());
			for (unsigned int npcIndex = 0; npcIndex < passerbyManager.passerby.size(); npcIndex++)
				passerbyManager.passerby[npcIndex]->SetXY(passerbyManager.passerby[npcIndex]->GetX1() + directionX, passerbyManager.passerby[npcIndex]->GetY1());
		}
		if (isMovingRight)
		{
			x -= directionX;
			for (unsigned int npcIndex = 0; npcIndex < passerbyManager.passerby.size(); npcIndex++)
				passerbyManager.passerby[npcIndex]->SetXY(passerbyManager.passerby[npcIndex]->GetX1() - directionX, passerbyManager.passerby[npcIndex]->GetY1());
		}
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
		return x >= 0 || x <= -((background.Width()) - 640);
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
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, 5, "RES", "IDB_MAP", mapIndex);
				break;

			case 1:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, -1, 0, 2, "RES", "IDB_MAP", mapIndex);
				break;

			case 2:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 0, 3, 3, "RES", "IDB_MAP", mapIndex);
				break;

			case 3:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 2, -1, 0, "RES", "IDB_MAP", mapIndex);
				break;

			default:
				blockMap[mapIndex] = CBlockMap(-1, -1, -1, -1, -1, 0, "RES", "IDB_MAP", 0);
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
		for (vector<CNPC*>::iterator it = passerby.begin(); it != passerby.end(); it++)
		{
			delete *it;
			(*it) = NULL;
		}
		vector<CNPC*> del;
		passerby.swap(del);
		passerby.clear();
	}
	CNPC* CPasserbyManager::AddPasserby(vector<int> id, int mapWidth)
	{
		#pragma region Create a Passerby
		int randomID = GetRandom(0, id.size() - 1); //random 決定 passerby種類 (1號or2號)
		CNPC *newPasserby = new CNPC(0, 0, ziliaojia, name[id[randomID]], 2, (id[randomID] + 1) * 10); //先創建一個default passerby
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
			CNPC* newPasserby = AddPasserby(id, mapWidth);
			newPasserby->SetValid(true);
			passerby.push_back(newPasserby);
		}
	}

	void CPasserbyManager::DeletePasserby(vector<CNPC*>::iterator passerbyj)
	{
		delete *passerbyj;
		*passerbyj = NULL;
		passerbyj = passerby.erase(passerbyj);
	}

	vector<CNPC*> CPasserbyManager::GetPasserby()
	{
		return passerby;
	}
	#pragma endregion

	#pragma region - DialogManager -
	CDialogManager CDialogManager::dialogManager;

	CDialogManager::CDialogManager()
	{
		IsBitmapLoaded = false;
		IsTxtLoaded = false;
		IsDialoging = false;
		mode = "";
		txt.clear();
	}

	CDialogManager::~CDialogManager()
	{
		
	}

	void CDialogManager::Load_Image()
	{
		char *address;
		#pragma region - load dialog background -
		address = ConvertCharPointToString("Dialog", "ground", 0);
		dialog_background.LoadBitmap(address, RGB(255, 255, 255));
		delete address;
		#pragma endregion

		#pragma region - load xingting - avatar -
		address = ConvertCharPointToString("Dialog", "xingting", 0);
		avatar_xingting.LoadBitmap(address);
		delete address;
		#pragma endregion

		#pragma region - load role avatar -
		address = ConvertCharPointToString("Dialog", "mikuAvatar", 0);
		avatar_role.LoadBitmap(address);
		delete address;
		#pragma endregion

		#pragma region - load null -
		address = ConvertCharPointToString("Dialog", "bmp", 0);
		avatar_null.LoadBitmap(address);
		delete address;
		#pragma endregion

		IsBitmapLoaded = true;
	}

	void CDialogManager::LoadText()
	{
		txt[RoleVSBoss].push_back("Ahhh~~");
		txt[RoleVSBoss].push_back("Bhhh~~");
		txt[RoleVSBoss].push_back("Chhh~~");
	}

	void CDialogManager::Initialize()
	{
		if (IsBitmapLoaded == false)
		{
			Load_Image();
		}
		if (IsTxtLoaded == false)
		{
			LoadText();
		}
		avatar_null.SetValid(false);
		dialog_background.SetValid(false);
		avatar = avatar_null;

		#pragma region - Init - Image Point -
		dialog_background.SetTopLeft(0, SIZE_Y - dialog_background.Height()); //reset dbg's xy
		avatar_role.SetTopLeft(dialog_background.Left() + MARGIN_DIALOG_AVATAR, dialog_background.Top() + MARGIN_DIALOG_AVATAR);
		avatar_xingting.SetTopLeft(dialog_background.Left() + MARGIN_DIALOG_AVATAR, dialog_background.Top() + MARGIN_DIALOG_AVATAR);
		#pragma endregion

		backgroundLayer.SetLayer(8);
		avatarLayer.SetLayer(backgroundLayer.GetLayer() + 1);

		CLayerManager::Instance()->AddObject(&dialog_background, backgroundLayer.GetLayer());
		CLayerManager::Instance()->AddObject(&avatar, avatarLayer.GetLayer());

		step = 0;
	}

	CDialogManager * CDialogManager::Instance()
	{
		return &dialogManager;
	}

	void CDialogManager::Start(string _mode)
	{
		IsDialoging = true;
		dialog_background.SetValid(true);
		mode = _mode;
		Dialog();
	}

	void CDialogManager::Next()
	{
		if(IsDialoging)
		{
			step++;
			Dialog();
		}	
	}

	void CDialogManager::Stop()
	{
		avatar = avatar_null;
		dialog_background.SetValid(false);
		IsDialoging = false;
	}

	bool CDialogManager::GetDialogState()
	{
		return IsDialoging;
	}

	void CDialogManager::Dialog()
	{
		if (mode == RoleVSBoss)
		{
			if (step == 0)
			{
				avatar = avatar_xingting;
			}
			else if (step == 1)
			{
				avatar = avatar_role;
			}
			else if (step == 2)
			{
				avatar = avatar_xingting;
			}
			else
			{
				step = 0;
				Stop();
			}
		}

	}
	#pragma endregion

	#pragma region - BossManager -
	CBossManager::CBossManager()
	{
		#pragma region -- Create Boss --
		bossInformation[BOSS_XINGTING] = CBoss(450, 250, 8787, BitmapPath("Role", "IDB_ROLE", 13), RGB(255, 255, 255));
		#pragma endregion
		targetBoss = NULL;
	}

	CBossManager::~CBossManager()
	{
	}

	void CBossManager::Initialize() //CGameRun::OnBeginState時 initialize
	{
		for (map<string, CBoss>::iterator bossiter = bossInformation.begin(); bossiter != bossInformation.end(); bossiter++)
		{
			bossiter->second.Initialize(); //初始化boss
			bossiter->second.GetAnimate()->SetValid(false); //將所有圖片先設為false
			CLayerManager::Instance()->AddObject(bossiter->second.GetAnimate(), bossiter->second.layer.GetLayer());
		}
		targetBoss = NULL;
	}

	void CBossManager::Clear()
	{
		bossInformation.clear();
		targetBoss = NULL;
	}

	void CBossManager::TargetBoss(int nowMap)
	{
		if (nowMap == 3)
		{
			targetBoss = &bossInformation[BOSS_XINGTING];
			targetBoss->GetAnimate()->SetValid(true);
		}
		else
		{
			targetBoss->GetAnimate()->SetValid(false);
			targetBoss = NULL;
		}
	}
	#pragma endregion
	
}
