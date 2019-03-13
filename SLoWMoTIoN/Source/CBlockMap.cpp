#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CBlockMap.h"
namespace game_framework
{
	CBlockMap::CBlockMap()
	{
		nowMap = 0;
		upMap = 0;
		downMap = 0;
		leftMap = 0;
		rightMap = 0;
		loadMap = 132;
	}

	CBlockMap::CBlockMap(int _nowMap, int _upMap, int _downMap, int _leftMap, int _rightMap, int _loadMap)
	{
		nowMap = _nowMap;
		upMap = _upMap;
		downMap = _downMap;
		leftMap = _leftMap;
		rightMap = _rightMap;
		loadMap = _loadMap;
	}


	CBlockMap::~CBlockMap()
	{
	}
}
