#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CLibrary.h"
//以下include安心的
#include "game.h"
#include "MainFrm.h"
#include <string.h>
#include "mygame.h"
#include <sstream>

namespace myLibrary
{
	char* ConvertCharPointToString(string ziliaojia, string name, int number)
	{
		char* address;
		std::stringstream ss;
		ss << number;
		string file = ".\\" + ziliaojia + "\\" + name + "_" + ss.str() + ".bmp";

		address = new char[file.length() + 1];
		strcpy(address, file.c_str());
		return address;
	}
	int abs(int k)
	{
		return k > 0 ? k : -k;
	}
}
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

	#pragma region - CAnimate -
	CAnimate::CAnimate()
	{
		x = y = bmp_index = bmp_amount = 0;
		bitmaps.clear();
		bmp.clear();
	}

	CAnimate::~CAnimate()
	{
		bitmaps.clear();
		bmp.clear();
	}

	void CAnimate::OnMove(int dir)
	{
		GAME_ASSERT(bitmaps.size() != 0, "CAnimation: Bitmaps must be loaded first.");

		if (dir == 0)	//Top
		{
			if (bmp_index < 3)
			{
				bmp_index++;
			}
			else
				bmp_index = 0;
		}
		else if (dir == 1) //Right
		{
			if (bmp_index >= 4 && bmp_index < 6)
				bmp_index++;
			else
				bmp_index = 4;
		}
		else if (dir == 2) // Down
		{
			if (bmp_index >= 7 && bmp_index < 9)
				bmp_index++;
			else
				bmp_index = 7;
		}
		else if (dir == 3) // Left
		{
			if (bmp_index >= 10 && bmp_index < 12)
				bmp_index++;
			else
				bmp_index = 10;
		}
		else //static
		{
			bmp_index = 0;
		}
		
		//bitmaps[bmp_index]->SetTopLeft(x, y);
		bmp[bmp_index].SetTopLeft(x, y);
	}

	void CAnimate::LoadBitmap(vector<char*> bmps, COLORREF colorkey)
	{
		for (vector<char*>::iterator bitmap = bmps.begin(); bitmap != bmps.end(); bitmap++)
		{
			bitmaps.push_back(AddBitmap(*(bitmap), colorkey));
		}
	}

	#pragma endregion

	CMovingBitmap* CAnimate::AddBitmap(int IDB_BITMAP, COLORREF colorkey)
	{
		CMovingBitmap add_bmp;
		add_bmp.LoadBitmap(IDB_BITMAP, colorkey);
		bmp.push_back(add_bmp);
		Reset();
		return &(bmp[bmp.size() - 1]);

	}

	CMovingBitmap* CAnimate::AddBitmap(char *filename, COLORREF colorkey)
	{
		CMovingBitmap add_bmp;
		add_bmp.LoadBitmap(filename, colorkey);
		bmp.push_back(add_bmp);
		Reset();
		return &(bmp[bmp.size() - 1]);
	}

	void CAnimate::Reset()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
	}

	void CAnimate::SetTopLeft(int nx, int ny)
	{
		x = nx, y = ny;
		//bitmaps[bmp_index]->SetTopLeft(x, y);
		bmp[bmp_index].SetTopLeft(x, y);

	}

	void CAnimate::OnShow()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded before they are shown.");
		//bitmaps[bmp_index]->ShowBitmap();
		bmp[bmp_index].ShowBitmap();
		
		
	}

	int CAnimate::Top()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return y;
	}

	int CAnimate::Left()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return x;
	}

	int CAnimate::Height()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		//return bitmaps[bmp_index]->Height();
		return bmp[bmp_index].Height();
		
	}

	int CAnimate::Width()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		//return bitmaps[bmp_index]->Width();
		return bmp[bmp_index].Width();
		
	}

}