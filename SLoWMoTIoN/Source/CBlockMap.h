#pragma once
#include <vector>
#include "CEraser.h"
using namespace std;
namespace game_framework
{
	#pragma region - CSimpleMapObj -
	class CSimpleMapObj
	{
	public:
		CSimpleMapObj() { path = ""; x = y = 0; };
		CSimpleMapObj(string _path, int _x, int _y) {
			path = _path;
			x = _x;
			y = _y;
		};
		~CSimpleMapObj() {};
		void SetXY(int _x, int _y, int camera = 0) {
			x = _x; y = _y;
			bmp.SetTopLeft(x - camera, y);
		};
		void LoadImg() {
			char *address = ConvertCharPointToString(path);
			bmp.LoadBitmap(address, RGB(214, 214, 214));
			delete address;
		};
		CMovingBitmap bmp;
		string path;
		int x, y; //��ڦ�m�W��x y
	};
	#pragma endregion

	#pragma region - CBlock -
	class CBlock : public CSimpleMapObj
	{
	public:
		CBlock() : CSimpleMapObj() {};
		CBlock(string _path, int _x, int _y) : CSimpleMapObj(_path, _x, _y) {};
		~CBlock() {};
	};
	#pragma endregion

	#pragma region - CDoor -
	class CBlockMap;
	class CDoor : public CSimpleMapObj
	{
	public:
		CDoor() : CSimpleMapObj() {};
		CDoor(string _path, int _x, int _y, string _type, int _deliveMapIndex) : CSimpleMapObj(_path, _x, _y) {
			type = _type;
			deliverMapIndex = _deliveMapIndex;
		};
		~CDoor() {};
		int GetSwitchMapIndex() {
			return deliverMapIndex;
		}
		string GetType() { return type; };
	private:
		string type; //�W or �U
		int deliverMapIndex = -1; //�n�ǰe���a��
	};
	#pragma endregion

	#pragma region - CBlockMap -
	class CBlockMap
	{
	public:
		CBlockMap(); //default constructor
		CBlockMap(int);
		~CBlockMap();

		void LoadImg();
		void LoadInformation(int);
		void LoadInformation(string);
		void CreateInformation();
		void CreateInformation(string);
		void Initialize(); //���Ψ쪺�F�賣�k0
		bool isLoad = false;
		
		int nowMap; //�ثe�a�Ϫ��s��
		int leftMap, rightMap, upMap, downMap; //�ثe�a�ϡA��W�U���k�a�Ϫ��s�� ( < 0�N���s�b)
		int loadMap; //���J���a�ϸ�T
		CMovingBitmap backgroundBitmap; //�I���Ϥ� (��LoadBitmapŪ��loadMap)
		int passerbyMaxSize;
		vector<int> passerbyID = {0, 1};
		string ziliaojia, name;
		int number;

		string loadPath;
		vector<CBlock> block;
		vector<CDoor> door;
		
	private:
		void LoadMap(string); //Ū��string��txt
		void WriteMap(string); //�g�Ustring��txt
	};
	#pragma endregion
}