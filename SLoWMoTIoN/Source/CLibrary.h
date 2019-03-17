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
		// 增加一張圖形至動畫(圖的編號及透明色)
		CMovingBitmap*  AddBitmap(char *, COLORREF = CLR_INVALID);
		// 增加一張圖形至動畫(圖的編號及透明色)
		void  LoadBitmap(vector<char*>, COLORREF = CLR_INVALID);
		//已字串陣列載入圖片
		int   Height();					// 取得動畫的高度
		int   Left();					// 取得動畫的左上角的 x 座標
		void  OnMove(int);				// 依照方向更換bitmap
		void  OnShow();					// 將動畫貼到螢幕
		void  Reset();					// 重設播放順序回到第一張圖形
		void  SetTopLeft(int, int);		// 將動畫的左上角座標移至 (x,y)
		int   Top();					// 取得動畫的左上角的 y 座標
		int   Width();					// 取得動畫的寬度


	private:

		vector<CMovingBitmap>			bmp;
		//vector<CMovingBitmap>::iterator bmp_iter;
		int								bmp_amount;
		int								x, y;			// 動畫的座標
		vector<CMovingBitmap*>			bitmaps;
		int								bmp_index;
	};
#pragma endregion
}