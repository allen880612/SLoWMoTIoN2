#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <vector>
#include "audio.h"
#include "gamelib.h"
#include <fstream>
#include <sstream>
#include "CBlockMap.h"
#include "CEraser.h"
using namespace std;
namespace game_framework
{
	CBlockMap::CBlockMap()
	{
		nowMap = 0;
		upMap = -1;
		downMap = -1;
		leftMap = -1;
		rightMap = -1;
		loadMap = IDB_MAP0;
		passerbyMaxSize = 5;
		loadPath = "RES\\Map\\IDB_Map_0.bmp";
	}

	CBlockMap::CBlockMap(int _nowMap)
	{
		nowMap = _nowMap;
		LoadInformation(nowMap);
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
	}

	CBlockMap::CBlockMap(int _nowMap, int _upMap, int _downMap, int _leftMap, int _rightMap, int _passerbyMaxSize, string _ziliaojia, string _name, int _number)
	{
		nowMap = _nowMap;
		upMap = _upMap;
		downMap = _downMap;
		leftMap = _leftMap;
		rightMap = _rightMap;
		passerbyMaxSize = _passerbyMaxSize;
		ziliaojia = _ziliaojia;
		name = _name;
		number = _number;

		CreateInformation();
	}

	CBlockMap::~CBlockMap()
	{
	}

	void CBlockMap::LoadImg()
	{
		char *address = ConvertCharPointToString(loadPath);
		backgroundBitmap.LoadBitmap(address);
		delete address;

		for (vector<CBlock>::iterator blockiter = block.begin(); blockiter != block.end(); blockiter++)
		{
			address = ConvertCharPointToString(blockiter->path);
			blockiter->blockBmp.LoadBitmap(address);
			delete address;
		}
	}

	void CBlockMap::LoadInformation(int mapIndex)
	{
		string mapFilePath = "RES\\Map\\Information\\map" + std::to_string(mapIndex) + ".txt";
		LoadMap(mapFilePath);
	}

	void CBlockMap::LoadInformation(string mapFileName)
	{
		string mapFilePath = "RES\\Map\\Information\\" + mapFileName; //§tªþÀÉ¦W
		LoadMap(mapFileName);
	}

	void CBlockMap::LoadMap(string mapFilePath)
	{
		fstream mapData;
		mapData.open(mapFilePath);
		string lineData;
		stringstream ss;
		while (getline(mapData, lineData))
		{
			ss.str(""); //init
			ss.clear(); //init
			ss << lineData;
			vector<string> tempString;
			#pragma region - split string -
			while (ss)
			{
				string line;
				ss >> line;
				if(line != "")
					tempString.push_back(line);
			}
			#pragma endregion
			#pragma region - load map information -
			if (tempString[0] == "background")
			{
				loadPath = "RES\\Map\\" + tempString[1];
			}
			else if(tempString[0] == "block")
			{
				block.push_back(CBlock(tempString[1], ConvertStringToInt(tempString[2]), ConvertStringToInt(tempString[3])));
			}
			else if (tempString[0] == "up")
			{
				upMap = ConvertStringToInt(tempString[1]);
			}
			else if (tempString[0] == "down")
			{
				downMap = ConvertStringToInt(tempString[1]);
			}
			else if (tempString[0] == "left")
			{
				leftMap = ConvertStringToInt(tempString[1]);
			}
			else if (tempString[0] == "right")
			{
				rightMap = ConvertStringToInt(tempString[1]);
			}
			else if (tempString[0] == "passerbyNumber")
			{
				passerbyMaxSize = ConvertStringToInt(tempString[1]);
			}
			else if (tempString[0] == "passerbyID")
			{
				passerbyID.clear();
				for (unsigned int i = 1; i < tempString.size(); i++)
				{
					passerbyID.push_back(ConvertStringToInt(tempString[i]));
				}
			}
			#pragma endregion
		}
		mapData.close();
	}

	void CBlockMap::CreateInformation()
	{
		fstream data;
		string fileName = "RES\\Map\\Information\\map" + std::to_string(nowMap) + ".txt";
		data.open(fileName, ios::out);
		vector<string> r;
		r.push_back("background " + name + "_" + std::to_string(nowMap) + ".bmp" + " 0 0\n");
		r.push_back("up " + std::to_string(upMap) + "\n");
		r.push_back("down " + std::to_string(downMap) + "\n");
		r.push_back("left " + std::to_string(leftMap) + "\n");
		r.push_back("right " + std::to_string(rightMap) + "\n");
		r.push_back("passerbyNumber " + std::to_string(passerbyMaxSize) + "\n");
		r.push_back("passerbyID 0 1\n");
		for (unsigned int i = 0; i < r.size(); i++)
		{
			data << r[i];
		}
		data.close();
	}
}
