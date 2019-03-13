#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CMapManager.h"
namespace game_framework
{
	CMapManager::CMapManager()
	{
		InitializeCBlockMap();
		nowMap = 0;
		loadMap = blockMap[0].loadMap;
	}


	CMapManager::~CMapManager()
	{
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
#pragma endregion

	void CMapManager::ChangeMap(int changeMap)
	{
		nowMap = changeMap;
		loadMap = blockMap[nowMap].loadMap;
	}

	void CMapManager::onShow() //顯示對應到的blockMap圖片 (nowMap = 1, 顯示blockMap[1]的background, 類推)
	{
		blockMap[nowMap].backgroundBitmap.SetTopLeft(0, 0);
		blockMap[nowMap].backgroundBitmap.ShowBitmap();
	}

	void CMapManager::InitializeCBlockMap()
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++) //初始化blockMap的上下左右地圖資訊，增加可讀性使用switch敘述
		{
			switch (mapIndex)
			{
				//順序：目前 上 下 左 右 ， -1表示不存在
			case 0:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, IDB_MAP0);
				break;

			case 1:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, -1, 0, IDB_MAP1);
				break;

			case 2:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 0, -1, IDB_MAP2);
				break;

			default:
				blockMap[mapIndex] = CBlockMap(-1, -1, -1, -1, -1, IDB_MAP0);
				break;
			}
		}
	}

	void CMapManager::LoadMapBitmap() //如字面意思，LoadMapBitmap，在GameStateRun:OnInit運行，一次性load blockMap的所有圖片
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++)
		{
			blockMap[mapIndex].backgroundBitmap.LoadBitmapA(blockMap[mapIndex].loadMap);
		}
	}
}
