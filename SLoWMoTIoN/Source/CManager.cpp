#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CManager.h"
#include "CLibrary.h"
using namespace myLibrary;

namespace game_framework
{
	#pragma region - mapManager -
	CMapManager::CMapManager()
	{
		InitializeCBlockMap();
		Initialize();
	}


	CMapManager::~CMapManager()
	{
	}

	void CMapManager::Initialize()
	{
		nowMap = 0;
		loadMap = blockMap[0].loadMap;
		x = 0;
		layer.SetLayer(1);
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
		//return blockMap[nowMap].npc.size();
	}

	int CMapManager::GetNpcLayer(int npcIndex)
	{
		return 6;
		//return blockMap[nowMap].npc[npcIndex].layer.GetLayer();
	}

	bool CMapManager::GetNpcValid(int npcIndex)
	{
		return true;
		//return blockMap[nowMap].npc[npcIndex].IsValid();
	}

	CMovingBitmap* CMapManager::GetBitmap()
	{
		blockMap[nowMap].backgroundBitmap.SetTopLeft(x, 0);
		return &(blockMap[nowMap].backgroundBitmap);
	}

	CAnimate* CMapManager::GetNpc(int npcIndex)
	{
		return (passerbyManager.passerby[npcIndex]->GetAnimate());
		//return (blockMap[nowMap].npc[npcIndex].GetAnimate());
	}

	int CMapManager::GetSplitLeft()
	{
		return 0 + (SIZE_X / 2);
	}

	int CMapManager::GetSplitRight()
	{
		return blockMap[nowMap].backgroundBitmap.Width() - (SIZE_X / 2);
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
		return blockMap[nowMap].backgroundBitmap.Width();
	}
	#pragma endregion

	void CMapManager::ChangeMap(int changeMap, string nextMap)
	{
		nowMap = changeMap;
		loadMap = blockMap[nowMap].loadMap;
		if (nextMap == "left") //下一張地圖(要換的地圖)，在原本地圖的左邊 (移動到左邊的地圖)
		{
			x = -(GetBitmapWidth() - SIZE_X);
		}
		else if (nextMap == "right")  //下一張地圖(要換的地圖)，在原本地圖的右邊 (移動到右邊的地圖)
		{
			x = 0; 
		}
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
	}

	void CMapManager::OnShow() //顯示對應到的blockMap圖片 (nowMap = 1, 顯示blockMap[1]的background, 類推)
	{
		blockMap[nowMap].backgroundBitmap.SetTopLeft(0, 0);
		blockMap[nowMap].backgroundBitmap.ShowBitmap();
	}

	void CMapManager::InitializeCBlockMap()
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++) //初始化blockMap的上下左右地圖資訊，增加可讀性使用switch敘述
		{
			switch (mapIndex)
			{				//順序：目前 上 下 左 右 ， -1表示不存在
			case 0:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, "RES", "IDB_MAP", mapIndex);
				blockMap[mapIndex].npc.push_back(CNPC(400, 300, "Role", "LUKA", 2));
				break;

			case 1:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, -1, 0, "RES", "IDB_MAP", mapIndex);
				blockMap[mapIndex].npc.push_back(CNPC(400, 300, "Role", "LUKA", 2));
				break;

			case 2:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 0, -1, "RES", "IDB_MAP", mapIndex);
				blockMap[mapIndex].npc.push_back(CNPC(800, 300, "Role", "LUKA", 2));
				break;

			default:
				blockMap[mapIndex] = CBlockMap(-1, -1, -1, -1, -1, "RES", "IDB_MAP", 0);

				blockMap[mapIndex].npc.push_back(CNPC(0, 300, "Role", "LUKA", 2));
				break;
			}
		}
	}

	void CMapManager::LoadMapBitmap() //如字面意思，LoadMapBitmap，在GameStateRun:OnInit運行，一次性load blockMap的所有圖片
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++)
		{
			blockMap[mapIndex].backgroundBitmap.LoadBitmapA(ConvertCharPointToString(blockMap[mapIndex].ziliaojia, blockMap[mapIndex].name, blockMap[mapIndex].number));
			for (unsigned int npcIndex = 0; npcIndex < blockMap[mapIndex].npc.size(); npcIndex++)
			{
				blockMap[mapIndex].npc[npcIndex].LoadBitmapA("Role", "LUKA", 2);
			}
		}

		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width());

	}
	#pragma endregion

	#pragma region - layerManager -
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
				(*k)->ShowBitmap();
			}

			for (vector<CAnimate*>::iterator k = layerAnimate[i].begin(); k != layerAnimate[i].end(); k++)
			{
				(*k)->OnShow();
			}
		}
	}
	#pragma endregion

	#pragma region - passerbyManager -
	CPasserbyManager::CPasserbyManager()
	{
		passerby.clear();
	}
	CPasserbyManager::~CPasserbyManager()
	{

	}
	void CPasserbyManager::Clear()
	{
		for (vector<CNPC*>::iterator it = passerby.begin(); it != passerby.end(); it++)
		{
			if (NULL != *it)
			{
				delete *it;
				*it = NULL;
			}
		}
		passerby.clear();
	}
	void CPasserbyManager::AddPasserbyManager(CNPC *newPasserby)
	{
		passerby.push_back(newPasserby);
	}
	void CPasserbyManager::CreatePasserby(int createNumber, vector<int> id, int mapWidth)
	{
		for (int i = 0; i < createNumber; i++)
		{
			CNPC *newPasserby = new CNPC();
			int randomID = GetRandom(0, id.size() - 1);
			newPasserby->LoadBitmap(ziliaojia, name[id[randomID]], 1);
			int randomX = GetRandom(0, mapWidth - newPasserby->GetX1());
			newPasserby->SetXY(randomX, 300);
			passerby.push_back(newPasserby);
		}
	}
	#pragma endregion
}
