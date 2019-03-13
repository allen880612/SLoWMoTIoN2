#pragma once
namespace game_framework
{
	class CBlockMap
	{
	public:
		CBlockMap(); //default constructor (其實用到的地方是因為CManager創建一般陣列時需要他，有一種方法可以避開→在宣告陣列時直接指定給予值進行初始化，而不是在Initlize裡面初始化。只是我覺得在Initlize裡面初始化比較好看)
		CBlockMap(int, int, int, int, int, int);
		~CBlockMap();
		//private:
		int nowMap; //目前地圖的編號
		int leftMap, rightMap, upMap, downMap; //目前地圖，其上下左右地圖的編號 ( < 0代表不存在)
		int loadMap; //載入的地圖資訊 (IDB_???好像是用#define成數字存的??不確定)
		CMovingBitmap backgroundBitmap; //背景圖片 (用LoadBitmap讀取loadMap)
	};
}
