#pragma once
#include "CBlockMap.h"
#include "CLibrary.h"
#include "Refactor.h"
//CManager: �a�Ϻ޲z�� and �ϼh�޲z�� and Timer�A��mygame�ϥ�
namespace game_framework
{
	#pragma region - MapManager -
	class CMapManager
	{
	public:
		CMapManager();
		~CMapManager();
		int GetNowMap();
		int GetUpMap();
		int GetDownMap();
		int GetLeftMap();
		int GetRightMap();
		int GetLoadMap();
		int GetSplitLeft();
		int GetSplitRight();
		int GetX1();
		int GetX2();
		int GetBitmapWidth();
		int GetNpcNumber();
		int GetNpcLayer(int);
		void SetMovingLeft(bool);
		void SetMovingRight(bool);
		void OnMove();


		void ChangeMap(int, string);
		void LoadMapBitmap();
		void OnShow();
		CMovingBitmap* GetBitmap();
		CAnimate* GetNpc(int);
		CLayer layer;
	private:
		const int directionX = 20;
		int nowMap;
		int loadMap;
		string loadMapPath;
		CBlockMap blockMap[MAX_MAP_NUMBER];
		int x;
		bool isMovingLeft;
		bool isMovingRight;
		int npcNumber;

		void InitializeCBlockMap();
	};
	#pragma endregion

	#pragma region - layerManager -
	class CLayerManager
	{
	public:
		CLayerManager();
		~CLayerManager();
		void Clear();
		void AddObject(CMovingBitmap*, int);
		void AddObject(CAnimate*, int);
		void ShowLayer();
	private:
		vector <CMovingBitmap*> layerBitmap[MAX_LAYER_NUMBER];
		vector <CAnimate*> layerAnimate[MAX_LAYER_NUMBER];
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