#pragma once
#include "Refactor.h"
namespace myLibrary
{
	char* ConvertCharPointToString(string, string, int);
	int abs(int);
}
namespace game_framework
{
	int ScreenX(int, int);
	#pragma region - CLayer -
	class CLayer
	{
		public:
			CLayer();
			~CLayer();
			void SetLayer(int);
			int GetLayer();
		private:
			int layer;
	};
	#pragma endregion

	#pragma region - CAnimate -
	class CAnimate
	{
	public:

		CAnimate();
		~CAnimate();
		CMovingBitmap*  AddBitmap(int, COLORREF = CLR_INVALID);
		// �W�[�@�i�ϧΦܰʵe(�Ϫ��s���γz����)
		CMovingBitmap*  AddBitmap(char *, COLORREF = CLR_INVALID);
		// �W�[�@�i�ϧΦܰʵe(�Ϫ��s���γz����)
		void  LoadBitmap(vector<char*>, COLORREF = CLR_INVALID);
		//�w�r��}�C���J�Ϥ�
		int   Height();					// ���o�ʵe������
		int   Left();					// ���o�ʵe�����W���� x �y��
		void  OnMove(int);				// �̷Ӥ�V��bitmap
		void  OnShow();					// �N�ʵe�K��ù�
		void  Reset();					// ���]���񶶧Ǧ^��Ĥ@�i�ϧ�
		void  SetTopLeft(int, int);		// �N�ʵe�����W���y�в��� (x,y)
		int   Top();					// ���o�ʵe�����W���� y �y��
		int   Width();					// ���o�ʵe���e��


	private:

		vector<CMovingBitmap>			bmp;
		//vector<CMovingBitmap>::iterator bmp_iter;
		int								bmp_amount;
		int								x, y;			// �ʵe���y��
		vector<CMovingBitmap*>			bitmaps;
		int								bmp_index;
	};
#pragma endregion
}