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
		loadMap = blockMap[nowMap].loadMap;
		x = 0;
		layer.SetLayer(1);
		background = blockMap[nowMap].backgroundBitmap;
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

	CMovingBitmap* CMapManager::GetBitmap()
	{
		return &background;
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
	int CMapManager::GetMaxPasserbySize()
	{
		return blockMap[nowMap].passerbyMaxSize;
	}
	vector<int> CMapManager::GetCanShowPasserbyIDOnThisMap()
	{
		return blockMap[nowMap].passerbyID;
	}
	#pragma endregion

	void CMapManager::ChangeMap(int changeMap, string nextMap)
	{
		nowMap = changeMap;
		loadMap = blockMap[nowMap].loadMap;
		background = blockMap[nowMap].backgroundBitmap;
		if (nextMap == "left") //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ����� (���ʨ쥪�䪺�a��)
		{
			x = -(GetBitmapWidth() - SIZE_X);
		}
		else if (nextMap == "right")  //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ��k�� (���ʨ�k�䪺�a��)
		{
			x = 0; 
		}
		background.SetTopLeft(x, 0);
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
		}
		if (isMovingRight)
		{
			x -= directionX;
		}
		background.SetTopLeft(x, 0);
	}

	void CMapManager::OnShow() //��ܹ����쪺blockMap�Ϥ� (nowMap = 1, ���blockMap[1]��background, ����)
	{
		background.SetTopLeft(0, 0);
		background.ShowBitmap();
	}

	void CMapManager::InitializeCBlockMap()
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++) //��l��blockMap���W�U���k�a�ϸ�T�A�W�[�iŪ�ʨϥ�switch�ԭz
		{
			switch (mapIndex)
			{				//���ǡG�ثe �W �U �� �k �A -1��ܤ��s�b
			case 0:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, "RES", "IDB_MAP", mapIndex);
				break;

			case 1:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, -1, 0, "RES", "IDB_MAP", mapIndex);
				break;

			case 2:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 0, -1, "RES", "IDB_MAP", mapIndex);
				break;

			default:
				blockMap[mapIndex] = CBlockMap(-1, -1, -1, -1, -1, "RES", "IDB_MAP", 0);
				break;
			}
		}
	}

	void CMapManager::LoadMapBitmap() //�p�r���N��ALoadMapBitmap�A�bGameStateRun:OnInit�B��A�@����load blockMap���Ҧ��Ϥ�
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

			for (vector<CAnimate*>::iterator k = layerAnimate[i].begin(); k != layerAnimate[i].end();)
			{
				if ((*k)->IsNull())
				{
					k = layerAnimate[i].erase(k);
				}
				else
				{
					(*k)->OnShow();
					k++;
				}
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
			delete *it;
			(*it) = NULL;
		}
		vector<CNPC*> del;
		passerby.swap(del);
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

	int CPasserbyManager::GetNpcNumber()
	{
		return passerby.size();
	}

	int CPasserbyManager::GetNpcLayer(int npcIndex)
	{
		return passerby[npcIndex]->layer.GetLayer();
	}

	bool CPasserbyManager::GetNpcValid(int npcIndex)
	{
		return passerby[npcIndex]->IsValid();
	}

	CAnimate* CPasserbyManager::GetNpc(int npcIndex)
	{
		return (passerby[npcIndex]->GetAnimate());
	}

	void CPasserbyManager::SetMapMovingLeft(bool _flag)
	{
		isMapMovingLeft = _flag;
	}
	void CPasserbyManager::SetMapMovingRight(bool _flag)
	{
		isMapMovingRight = _flag;
	}

	void CPasserbyManager::OnMapMove()
	{
		const int directionX = 20;
		if (isMapMovingLeft)
		{
			for (unsigned int npcIndex = 0; npcIndex < passerby.size(); npcIndex++)
				passerby[npcIndex]->SetXY(passerby[npcIndex]->GetX1() + directionX, passerby[npcIndex]->GetY1());
		}
		if (isMapMovingRight)
		{
			for (unsigned int npcIndex = 0; npcIndex < passerby.size(); npcIndex++)
				passerby[npcIndex]->SetXY(passerby[npcIndex]->GetX1() - directionX, passerby[npcIndex]->GetY1());
		}
	}

	#pragma endregion
}
