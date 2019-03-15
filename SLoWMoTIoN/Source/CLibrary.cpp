#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CLibrary.h"

namespace game_framework
{
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
		if (_layer >= 10)
			_layer = 9;
		else if (_layer < 0)
			_layer = 0;
		layer = _layer;
	}
	#pragma endregion

	#pragma region MyRegion

	#pragma endregion

}