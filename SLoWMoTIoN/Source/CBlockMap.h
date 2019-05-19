#pragma once
#include <vector>
#include "CEraser.h"
using namespace std;
namespace game_framework
{
	#pragma region - CBlock -
	class CBlock
	{
	public:
		CBlock() { path = ""; x = y = 0; };
		CBlock(string _path, int _x, int _y) {
			path = _path;
			x = _x;
			y = _y;
		};
		~CBlock() {};
		void SetXY(int _x, int _y, int camera = 0) {
			x = _x; y = _y;
			blockBmp.SetTopLeft(x - camera, y);
		};
		CMovingBitmap blockBmp;
		string path;
		string name;
		int x, y; //實際位置上的x y
	};
	#pragma endregion

	class CBlockMap
	{
	public:
		CBlockMap(); //default constructor (其實用到的地方是因為CManager創建一般陣列時需要他，有一種方法可以避開→在宣告陣列時直接指定給予值進行初始化，而不是在Initlize裡面初始化。只是我覺得在Initlize裡面初始化比較好看)
		CBlockMap(int);
		CBlockMap(int, int, int, int, int, int);
		CBlockMap(int, int, int, int, int, int, string, string, int);

		void LoadImg();
		void LoadInformation(int);
		void LoadInformation(string);
		void CreateInformation();
		void CreateInformation(string);

		bool isLoad = false;

		~CBlockMap();
		//private:
		int nowMap; //目前地圖的編號
		int leftMap, rightMap, upMap, downMap; //目前地圖，其上下左右地圖的編號 ( < 0代表不存在)
		int loadMap; //載入的地圖資訊 (IDB_???好像是用#define成數字存的??不確定)
		CMovingBitmap backgroundBitmap; //背景圖片 (用LoadBitmap讀取loadMap)
		int passerbyMaxSize;
		vector<int> passerbyID = {0, 1};
		string ziliaojia, name;
		int number;

		string loadPath;
		vector<CBlock> block;
	private:
		void LoadMap(string); //讀取string的txt
		void WriteMap(string); //寫下string的txt
	};
	
	
}
