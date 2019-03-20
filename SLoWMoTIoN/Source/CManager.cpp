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
		return blockMap[nowMap].npc.size();
	}

	int CMapManager::GetNpcLayer(int npcIndex)
	{
		return blockMap[nowMap].npc[npcIndex].layer.GetLayer();
	}

	bool CMapManager::GetNpcValid(int npcIndex)
	{
		return blockMap[nowMap].npc[npcIndex].IsValid();
	}

	CMovingBitmap* CMapManager::GetBitmap()
	{
		blockMap[nowMap].backgroundBitmap.SetTopLeft(x, 0);
		return &(blockMap[nowMap].backgroundBitmap);
	}

	CAnimate* CMapManager::GetNpc(int npcIndex)
	{
		return (blockMap[nowMap].npc[npcIndex].GetAnimate());
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
		if (nextMap == "left") //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ����� (���ʨ쥪�䪺�a��)
		{
			x = -(GetBitmapWidth() - SIZE_X);
		}
		else if (nextMap == "right")  //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ��k�� (���ʨ�k�䪺�a��)
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
			for(unsigned int npcIndex = 0; npcIndex < blockMap[nowMap].npc.size(); npcIndex++)
				blockMap[nowMap].npc[npcIndex].SetXY(blockMap[nowMap].npc[npcIndex].GetX1() + directionX, blockMap[nowMap].npc[npcIndex].GetY1());
		}
		if (isMovingRight)
		{
			x -= directionX;
			for (unsigned int npcIndex = 0; npcIndex < blockMap[nowMap].npc.size(); npcIndex++)
				blockMap[nowMap].npc[npcIndex].SetXY(blockMap[nowMap].npc[npcIndex].GetX1() - directionX, blockMap[nowMap].npc[npcIndex].GetY1());
		}
	}

	void CMapManager::OnShow() //��ܹ����쪺blockMap�Ϥ� (nowMap = 1, ���blockMap[1]��background, ����)
	{
		blockMap[nowMap].backgroundBitmap.SetTopLeft(0, 0);
		blockMap[nowMap].backgroundBitmap.ShowBitmap();
	}

	void CMapManager::InitializeCBlockMap()
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++) //��l��blockMap���W�U���k�a�ϸ�T�A�W�[�iŪ�ʨϥ�switch�ԭz
		{
			switch (mapIndex)
			{				//���ǡG�ثe �W �U �� �k �A -1��ܤ��s�b
			case 0:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, "RES", "IDB_MAP", mapIndex);
				blockMap[mapIndex].npc.push_back(CNPC(200, 300, "Role\\NPC", "LUKA", 2));
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

	void CMapManager::LoadMapBitmap() //�p�r���N��ALoadMapBitmap�A�bGameStateRun:OnInit�B��A�@����load blockMap���Ҧ��Ϥ�
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++)
		{
			blockMap[mapIndex].backgroundBitmap.LoadBitmapA(ConvertCharPointToString(blockMap[mapIndex].ziliaojia, blockMap[mapIndex].name, blockMap[mapIndex].number));
			for (unsigned int npcIndex = 0; npcIndex < blockMap[mapIndex].npc.size(); npcIndex++)
			{
				blockMap[mapIndex].npc[npcIndex].LoadBitmapA("Role", "LUKA", 2);
			}
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

			for (vector<CAnimate*>::iterator k = layerAnimate[i].begin(); k != layerAnimate[i].end(); k++)
			{
				(*k)->OnShow();
			}
		}
	}
	#pragma endregion
}
