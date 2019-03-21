#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <vector>
#include "audio.h"
#include "gamelib.h"
#include "CBlockMap.h"
#include "CEraser.h"
using namespace std;
namespace game_framework
{
	CBlockMap::CBlockMap()
	{
		nowMap = 0;
		upMap = 0;
		downMap = 0;
		leftMap = 0;
		rightMap = 0;
		loadMap = IDB_MAP0;
		npc.clear();
	}

	CBlockMap::CBlockMap(int _nowMap, int _upMap, int _downMap, int _leftMap, int _rightMap, int _loadMap)
	{
		nowMap = _nowMap;
		upMap = _upMap;
		downMap = _downMap;
		leftMap = _leftMap;
		rightMap = _rightMap;
		loadMap = _loadMap;
		passerbyMaxSize = 5;
		npc.clear();
	}

	CBlockMap::CBlockMap(int _nowMap, int _upMap, int _downMap, int _leftMap, int _rightMap, string _ziliaojia, string _name, int _number)
	{
		nowMap = _nowMap;
		upMap = _upMap;
		downMap = _downMap;
		leftMap = _leftMap;
		rightMap = _rightMap;
		ziliaojia = _ziliaojia;
		name = _name;
		number = _number;
		passerbyMaxSize = 5;
		npc.clear();
	}

	CBlockMap::~CBlockMap()
	{
	}
}
