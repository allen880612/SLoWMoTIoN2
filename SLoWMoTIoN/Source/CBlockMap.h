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
		int x, y; //��ڦ�m�W��x y
	};
	#pragma endregion

	class CBlockMap
	{
	public:
		CBlockMap(); //default constructor (���Ψ쪺�a��O�]��CManager�Ыؤ@��}�C�ɻݭn�L�A���@�ؤ�k�i�H�׶}���b�ŧi�}�C�ɪ������w�����ȶi���l�ơA�Ӥ��O�bInitlize�̭���l�ơC�u�O��ı�o�bInitlize�̭���l�Ƥ���n��)
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
		int nowMap; //�ثe�a�Ϫ��s��
		int leftMap, rightMap, upMap, downMap; //�ثe�a�ϡA��W�U���k�a�Ϫ��s�� ( < 0�N���s�b)
		int loadMap; //���J���a�ϸ�T (IDB_???�n���O��#define���Ʀr�s��??���T�w)
		CMovingBitmap backgroundBitmap; //�I���Ϥ� (��LoadBitmapŪ��loadMap)
		int passerbyMaxSize;
		vector<int> passerbyID = {0, 1};
		string ziliaojia, name;
		int number;

		string loadPath;
		vector<CBlock> block;
	private:
		void LoadMap(string); //Ū��string��txt
		void WriteMap(string); //�g�Ustring��txt
	};
	
	
}
