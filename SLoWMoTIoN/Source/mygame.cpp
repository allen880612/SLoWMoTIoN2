/*
* mygame.cpp: ���ɮ��x�C��������class��implementation
* Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
*
* This file is part of game, a free game development framework for windows.
*
* game is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* game is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* History:
*   2002-03-04 V3.1
*          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
*	 2004-03-02 V4.0
*      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
*         demonstrate the use of states.
*      2. Demo the use of CInteger in CGameStateRun.
*   2005-09-13
*      Rewrite the codes for CBall and CEraser.
*   2005-09-20 V4.2Beta1.
*   2005-09-29 V4.2Beta2.
*      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
*   2006-02-08 V4.2
*      1. Revise sample screens to display in English only.
*      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
*      3. Rename OnInitialUpdate() -> OnInit().
*      4. Fix the bug that OnBeginState() of GameStateInit is not called.
*      5. Replace AUDIO_CANYON as AUDIO_NTUT.
*      6. Add help bitmap to CGameStateRun.
*   2006-09-09 V4.3
*      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
*         event driven.
*   2006-12-30
*      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
*         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
*   2008-02-15 V4.4
*      1. Add namespace game_framework.
*      2. Replace the demonstration of animation as a new bouncing ball.
*      3. Use ShowInitProgress(percent) to display loading progress.
*   2010-03-23 V4.6
*      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	/////////////////////////////////////////////////////////////////////////////

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
								//
								// �}�l���J���
								//

		logo.LoadBitmap(IDB_MIKU_B);
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
								//
								// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
								//
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo �����C������k
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
	}

	void CGameStateInit::OnShow()
	{
		//
		// �K�Wlogo
		//
		logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
		logo.ShowBitmap();
		//
		// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
		//
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		fp = pDC->SelectObject(&f);					// ��� font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
			GotoGameState(GAME_STATE_INIT);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
	}

	void CGameStateOver::OnInit()
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
								//
								// �}�l���J���
								//
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
								//
								// �̲׶i�׬�100%
								//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		fp = pDC->SelectObject(&f);					// ��� font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g), NUMBALLS(28)
	{
		ball = new CBall[NUMBALLS];
	}

	CGameStateRun::~CGameStateRun()
	{
		delete[] ball;
	}

	void CGameStateRun::OnBeginState()
	{
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 10;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;
		for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
			int x_pos = i % BALL_PER_ROW;
			int y_pos = i / BALL_PER_ROW;
			ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
			ball[i].SetDelay(x_pos);
			ball[i].SetIsAlive(true);
		}
		eraser.Initialize();
		background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
		help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
		hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
		hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
		//CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{
		//
		// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
		//
		SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// ���ʭI���Ϫ��y��
		//
		if (background.Top() > SIZE_Y)
			background.SetTopLeft(60, -background.Height());
		background.SetTopLeft(background.Left(), background.Top() + 1);

		// ����MIKU
		miku.OnMove();

		// ���ʲy
		//
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].OnMove();
		//
		// �������l
		//
		#pragma region - ���k�ɦa���ˬd - �d���or���a�� -
		#pragma region -- �k�䦳�a�ϥB�H�����k��樫 --
		if (eraser.GetX2() >= SIZE_X && mapManager.GetRightMap() >= 0 && eraser.GetMovingRight())
		{
			#pragma region --- �W�L��� - ���a��---
			if (eraser.GetX1() >= SIZE_X)
			{
				eraser.SetXY(0 - (eraser.GetX2() - eraser.GetX1()), eraser.GetY1());
				mapManager.ChangeMap(mapManager.GetRightMap());
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region -- �k��S���a�ϥB�H�����k��樫 --
		if (eraser.GetX2() >= SIZE_X && mapManager.GetRightMap() < 0 && eraser.GetMovingRight())
		{
			eraser.SetMovingRight(false); //�S���a�ϡA�d���
		}
		#pragma endregion

		#pragma region -- ���䦳�a�ϥB�H��������樫 --
		if (eraser.GetX1() <= 0 && mapManager.GetLeftMap() >= 0 && eraser.GetMovingLeft())
		{
			#pragma region --- �W�L��� - ���a��---
			if (eraser.GetX2() <= 0) //�W�L��ɡA���a��
			{
				eraser.SetXY(SIZE_X + (eraser.GetX2() - eraser.GetX1()), eraser.GetY1());
				mapManager.ChangeMap(mapManager.GetLeftMap());
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region -- ����S���a�ϥB�H��������樫 --
		if (eraser.GetX1() <= 0 && mapManager.GetLeftMap() < 0 && eraser.GetMovingLeft())
		{
			eraser.SetMovingLeft(false); //�S���a�ϡA�d���
		}
		#pragma endregion
		#pragma endregion

		eraser.OnMove();
		//
		// �P�_���l�O�_�I��y
		//
		for (i = 0; i < NUMBALLS; i++)
			if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
				ball[i].SetIsAlive(false);
				CAudio::Instance()->Play(AUDIO_DING);
				hits_left.Add(-1);
				//
				// �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
				//
				if (hits_left.GetInteger() <= 0) {
					CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
					CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
					GotoGameState(GAME_STATE_OVER);
				}
			}
		//
		// ���ʼu�����y
		//
		gameMap.OnMove();
	}

	void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
								//
								// �}�l���J���
								//
		gameMap.LoadBitmap();
		miku.LoadBitmap();
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
		eraser.LoadBitmap();
		background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
																//
																// ��������Loading�ʧ@�A�����i��
																//
		ShowInitProgress(50);
		Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
					//
					// �~����J��L���
					//
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
		corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
		corner.ShowBitmap(background);							// �Ncorner�K��background
		bball.LoadBitmap();										// ���J�ϧ�
		hits_left.LoadBitmap();
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
																	//
																	// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
																	//
		mapManager.LoadMapBitmap();
#pragma region MyRegion

#pragma endregion


	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard���b�Y
		const char KEY_UP = 0x26; // keyboard�W�b�Y
		const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
		const char KEY_DOWN = 0x28; // keyboard�U�b�Y
									//��H�����k�樫���a��n�d�b�D�{���P�_or�b�H���樫���ɭԧP�_?
									//�Ϊ̬O�b�H���̭����P�_�i�_�樫(?)���
		if (nChar == KEY_LEFT)
		{
			eraser.SetMovingLeft(true);
			//mapManager.ChangeMap(mapManager.GetLeftMap());
		}
		if (nChar == KEY_RIGHT)
		{
			eraser.SetMovingRight(true);
			//mapManager.ChangeMap(mapManager.GetRightMap());
		}
		if (nChar == KEY_UP)
			eraser.SetMovingUp(true);
		if (nChar == KEY_DOWN)
			eraser.SetMovingDown(true);

		gameMap.OnKeyDown(nChar);
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard���b�Y
		const char KEY_UP = 0x26; // keyboard�W�b�Y
		const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
		const char KEY_DOWN = 0x28; // keyboard�U�b�Y
		if (nChar == KEY_LEFT)
			eraser.SetMovingLeft(false);
		if (nChar == KEY_RIGHT)
			eraser.SetMovingRight(false);
		if (nChar == KEY_UP)
			eraser.SetMovingUp(false);
		if (nChar == KEY_DOWN)
			eraser.SetMovingDown(false);
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		eraser.SetMovingLeft(true);
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		eraser.SetMovingLeft(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		eraser.SetMovingRight(true);
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		eraser.SetMovingRight(false);
	}

	void CGameStateRun::OnShow()
	{
		//
		//  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
		//        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
		//        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
		//
		//
		//  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
		//
		background.ShowBitmap();			// �K�W�I����
		gameMap.OnShow();					//�K�W��h���I����
		help.ShowBitmap();					// �K�W������
		hits_left.ShowBitmap();
		for (int i = 0; i < NUMBALLS; i++)
			ball[i].OnShow();				// �K�W��i���y
		bball.OnShow();						// �K�W�u�����y

		mapManager.onShow();
		eraser.OnShow();					// �K�W���l
											//
											//  �K�W���W�Υk�U��������
											//
		corner.SetTopLeft(0, 0);
		corner.ShowBitmap();
		corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
		corner.ShowBitmap();
		//�K�WMIKU
		miku.onShow();

		printf("x = %d\n", eraser.GetX1());
	}
	CMiku::CMiku()
	{
		x = y = 0;
	}

	void CMiku::LoadBitmap()
	{
		pic.LoadBitmap(IDB_MIKU_A);
	}



	void CMiku::OnMove()
	{
		if (y <= SIZE_Y)
		{
			x += 3;
			y += 3;
		}
		else
		{
			x = y = 0;
		}
	}

	void CMiku::onShow()
	{
		pic.SetTopLeft(x, y);
		pic.ShowBitmap();
	}

	CGameMap::CGameMap()
		:X(20), Y(40), MW(120), MH(100)
	{
		int map_init[4][5] = { { 1,2,1,2,1 },
		{ 0,1,2,1,0 },
		{ 1,0,1,0,1 },
		{ 2,1,2,1,2 } };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				map[i][j] = map_init[i][j];
			}
		}
		random_num = 0;
		bballs = NULL;
	}

	void CGameMap::LoadBitmap()
	{
		blue.LoadBitmap(ID_BLUE);
		green.LoadBitmap(ID_GREEN);
		red.LoadBitmap(ID_RED);
	}

	void CGameMap::OnShow()
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (map[j][i] == 0)
				{
					red.SetTopLeft(X + (MW * i), Y + (MH * j));
					red.ShowBitmap();
				}
				else if (map[j][i] == 1)
				{
					blue.SetTopLeft(X + (MW * i), Y + (MH * j));
					blue.ShowBitmap();
				}
				else if (map[j][i] == 2)
				{
					green.SetTopLeft(X + (MW * i), Y + (MH * j));
					green.ShowBitmap();
				}
				else
					ASSERT(0);
			}
		}

		for (int i = 0; i < random_num; i++)
		{
			bballs[i].OnShow();
		}
	}

	void CGameMap::OnMove()
	{
		for (int i = 0; i < random_num; i++)
		{
			bballs[i].OnMove();
		}
	}

	void CGameMap::OnKeyDown(UINT nCHAR)
	{
		const int KEY_SPACE = 0x20;
		if (nCHAR == KEY_SPACE)
			RandomBonucingBall();

	}

	void CGameMap::RandomBonucingBall()
	{
		const int MAX_RANDOM_NUM = 10;				//�H���̤j��
		random_num = (rand() % MAX_RANDOM_NUM) + 1;	// [1 ~ MAX_RANDOM_NUM -1]

		delete[] bballs;							//���R�����e�t�m���|��
		bballs = new CBouncingBall[random_num];		//���H���ȰʺA�t�m
		int init_index = 0;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 5; col++)
			{
				if (map[row][col] != 0 && init_index < random_num)
				{
					InitializeBonucingBall(init_index, row, col);
					init_index++;
				}
			}
		}
	}

	void CGameMap::InitializeBonucingBall(int init_index, int row, int col)
	{
		const int VELOCITY = 10;							//�y����t
		const int BALL_PIC_HEIGHT = 15;						//�y���Ϥ�����
		int floor = Y + (row + 1) * MH - BALL_PIC_HEIGHT;	//�]�w�y�����U�I

		bballs[init_index].LoadBitmap();
		bballs[init_index].SetFloor(floor);
		bballs[init_index].SetVelocity(VELOCITY + col);
		bballs[init_index].SetXY(X + col * MW + MW / 2, floor);
	}

	CGameMap::~CGameMap()
	{
		delete[] bballs;
	}

}//End