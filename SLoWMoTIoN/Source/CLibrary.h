#pragma once
#include "Refactor.h"
namespace myLibrary
{
	char* ConvertCharPointToString(string, string, int);
	void DeleteCharPoint(vector<char*>&);
	int GetPostive(int);
	int GetRandom(int, int); //�o��Random�Ʀr�Amin <= k <= max
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

		void  ReleaseAnimate();

	private:

		vector<CMovingBitmap>			bmp;
		int								bmp_amount;
		int								x, y;			// �ʵe���y��
		int								bmp_index;
	};
#pragma endregion

	#pragma region - timer -
	class CTimer
	{
	public:
		CTimer();
		CTimer(int);
		~CTimer();
		void CountDown();
		int GetTime();
		bool IsTimeOut();
		void ResetTime(int);

		void operator=(CTimer); //�B��l�h���A��K�bGameStateRun::OnBeginState�����cTimer
	private:
		const int reflash = (1000 / GAME_CYCLE_TIME); //�@���s�X��
		int time;
	};
	#pragma endregion

}