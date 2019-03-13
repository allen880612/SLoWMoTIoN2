#pragma once
namespace game_framework
{
	class CBlockMap
	{
	public:
		CBlockMap(); //default constructor (���Ψ쪺�a��O�]��CManager�Ыؤ@��}�C�ɻݭn�L�A���@�ؤ�k�i�H�׶}���b�ŧi�}�C�ɪ������w�����ȶi���l�ơA�Ӥ��O�bInitlize�̭���l�ơC�u�O��ı�o�bInitlize�̭���l�Ƥ���n��)
		CBlockMap(int, int, int, int, int, int);
		~CBlockMap();
		//private:
		int nowMap; //�ثe�a�Ϫ��s��
		int leftMap, rightMap, upMap, downMap; //�ثe�a�ϡA��W�U���k�a�Ϫ��s�� ( < 0�N���s�b)
		int loadMap; //���J���a�ϸ�T (IDB_???�n���O��#define���Ʀr�s��??���T�w)
		CMovingBitmap backgroundBitmap; //�I���Ϥ� (��LoadBitmapŪ��loadMap)
	};
}
