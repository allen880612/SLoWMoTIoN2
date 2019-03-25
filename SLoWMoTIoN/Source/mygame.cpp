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
*      Rewrite the codes for CBall and Crole.
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
#pragma once
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "CManager.h"
#include "CLibrary.h"
#include <vector>
using namespace std;

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

		logo.LoadBitmap(".\\RES\\Menu.bmp");
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
		/*CEraser *fuck;
		CNPC ddd;
		fuck = &ddd;*/
		//
		// �K�Wlogo
		//
		//logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
		logo.SetTopLeft(0, 0);
		logo.ShowBitmap();
		//
		// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
		//
		CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		fp = pDC->SelectObject(&f);					// ��� font f
		
		/*
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (enable_game_pause)
			pdc->textout(5, 425, "press ctrl-q to pause the game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		*/
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
		const int TIME_LEFT = 10;			//�C�����
		const int TIME_LEFT_X = 590;
		const int TIME_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;

		for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
			int x_pos = i % BALL_PER_ROW;
			int y_pos = i / BALL_PER_ROW;
			ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
			ball[i].SetDelay(x_pos);
			ball[i].SetIsAlive(true);
		}
		mapManager.Initialize();
		role.Initialize();
		background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
		help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
		time_left.SetInteger(TIME_LEFT);					// ���w�ѤU��������
		time_left.SetTopLeft(TIME_LEFT_X, TIME_LEFT_Y);		// ���w�ѤU�����ƪ��y��
		//CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
		timer = CTimer(TIME_LEFT); //�}����==
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
		//miku.LoadBitmap();
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
																//role.LoadBitmap();
		role.LoadBitmap("Role", "MIKU", 13);
		background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
																//
																// ��������Loading�ʧ@�A�����i��
																//
		ShowInitProgress(50);
		//Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//			//
		//			// �~����J��L���
		//			//
		help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
		corner.LoadBitmap(IDB_CORNER);								// ���J�����ϧ�
		corner.ShowBitmap(background);								// �Ncorner�K��background
																	//bball.LoadBitmap();										// ���J�ϧ�
		time_left.LoadBitmap();
		CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
		CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
		CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
																	//
																	// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
																	//
		#pragma region - Initialize - MapManager -
		mapManager.LoadMapBitmap();
		#pragma endregion

		#pragma region - layerManager - AddObject -
		layerManager.AddObject(role.GetAnimate(), role.layer.GetLayer());
		layerManager.AddObject(mapManager.GetBitmap(), mapManager.layer.GetLayer());
		for (int i = 0; i < mapManager.GetNpcNumber(); i++)
		{
			layerManager.AddObject(mapManager.GetNpc(i), mapManager.GetNpcLayer(i));
		}
		#pragma endregion

	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{
		#pragma region - timer count down -
		timer.CountDown();
		if (timer.IsTimeOut())
			GotoGameState(GAME_STATE_OVER);
		/*if (counter % 30 == 0)
			time_left.Add(-1);*/
		#pragma endregion
		
		//
		// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
		//
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR));
		//
		// ���ʭI���Ϫ��y��
		//
		if (background.Top() > SIZE_Y)
			background.SetTopLeft(60, -background.Height());
		background.SetTopLeft(background.Left(), background.Top() + 1);
		
		// ����MIKU
		//miku.OnMove();

		// ���ʲy
		//
		//int i;
		//for (i = 0; i < NUMBALLS; i++)
		//	ball[i].OnMove();
		//
		// �������l
		//

		#pragma region - Moving -
		int screenPosX = ScreenX(mapManager.GetX1(), role.GetX3());
		#pragma region -- Moving Right --

		if (role.GetMovingRight())
		{
			if (screenPosX >= mapManager.GetSplitLeft() && screenPosX < mapManager.GetSplitRight())
			{
				mapManager.SetMovingLeft(false);
				mapManager.SetMovingRight(true);
				mapManager.OnMove();
				role.SetCanMoving(false);
			}
			else
			{
				mapManager.SetMovingLeft(false);
				mapManager.SetMovingRight(false);
				role.SetCanMoving(true);
			}
		}

		#pragma region -- Moving Left --
		if (role.GetMovingLeft())
		{
			if (screenPosX > mapManager.GetSplitLeft() && screenPosX <= mapManager.GetSplitRight())
			{
				mapManager.SetMovingLeft(true);
				mapManager.SetMovingRight(false);
				mapManager.OnMove();
				role.SetCanMoving(false);
			}
			else
			{
				mapManager.SetMovingLeft(false);
				mapManager.SetMovingRight(false);
				role.SetCanMoving(true);
			}
		}
		#pragma endregion
		#pragma endregion

		#pragma region -- ���k�ɦa���ˬd - �d���or���a�� --
		

		#pragma region --- �k�䦳�a�ϥB�H�����k��樫 ---
		if (role.GetX2() >= SIZE_X && mapManager.GetRightMap() >= 0 && role.GetMovingRight())
		{
			#pragma region ---- �W�L��� - ���a�� ----
			if (role.GetX1() >= SIZE_X)
			{
				role.SetXY(0 - (role.GetX2() - role.GetX1()), role.GetY1());
				mapManager.ChangeMap(mapManager.GetRightMap(), "right");
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region --- �k��S���a�ϥB�H�����k��樫 ---
		if (role.GetX2() >= SIZE_X && mapManager.GetRightMap() < 0 && role.GetMovingRight())
		{
			role.SetCanMoving(false); //�S���a�ϡA�d���
		}
		#pragma endregion

		#pragma region --- ���䦳�a�ϥB�H��������樫 ---
		if (role.GetX1() <= 0 && mapManager.GetLeftMap() >= 0 && role.GetMovingLeft())
		{
			#pragma region ---- �W�L��� - ���a�� ----
			if (role.GetX2() <= 0) //�W�L��ɡA���a��
			{
				role.SetXY(SIZE_X + (role.GetX2() - role.GetX1()), role.GetY1());
				mapManager.ChangeMap(mapManager.GetLeftMap(), "left");
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region --- ����S���a�ϥB�H��������樫 ---
		if (role.GetX1() <= 0 && mapManager.GetLeftMap() < 0 && role.GetMovingLeft())
		{
			role.SetCanMoving(false); //�S���a�ϡA�d���
		}
		#pragma endregion
		#pragma endregion

		#pragma region -- role Moving --
		role.OnMove();
		#pragma endregion
		#pragma endregion

		#pragma region - Jump -
		if (role.GetMovingJump())
		{
			if (role.GetY2() > SIZE_Y)
			{
				role.SetMovingJump(false);
				role.SetCanJumping(true);
				role.SetXY(role.GetX1(), SIZE_Y - role.Height() - 1);
			}
			else
			{
				role.SetMovingJump(true);
				role.SetCanJumping(false);
			}

		}
		#pragma endregion
		
		#pragma region - Collision -
		scallions = role.GetScallion();			//���X�������а��I��
		passerbys = mapManager.GetPasserby();	//���Xpasserby���иI��
		for (unsigned int i = 0; i < scallions.size(); i++)
		{
			for (unsigned int j = 0; j < passerbys.size(); j++)
			{
				if (scallions[i]->IsCollision(passerbys[j]))
				{
					scallions[i]->SetIsAlive(false);
					//�R����

					//��NPC���F�R��
					passerbys[j]->SetValid(false);
				}
			}
		}
		vector<CScallion*> delS;
		vector<CNPC*> delN;
		scallions.swap(delS);
		passerbys.swap(delN);
		scallions.clear();
		passerbys.clear();
		#pragma endregion
		
		//
		// �P�_���l�O�_�I��y
		//
		//for (i = 0; i < NUMBALLS; i++)
		//	if (ball[i].IsAlive() && ball[i].HitEraser(&role)) {
		//		ball[i].SetIsAlive(false);
		//		CAudio::Instance()->Play(AUDIO_DING);
		//		time_left.Add(0);
		//		//
		//		// �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
		//		//
		//		if (time_left.GetInteger() <= 0) {
		//			CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
		//			CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
		//			GotoGameState(GAME_STATE_OVER);
		//		}
		//	}

	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		
		//if (nChar == KEY_LEFT)
		//{
		//	role.SetMovingLeft(true);
		//}
		//if (nChar == KEY_RIGHT)
		//{
		//	role.SetMovingRight(true);
		//}
		//if (nChar == KEY_UP)
		//	role.SetMovingUp(true);
		//if (nChar == KEY_DOWN)
		//{
		//	role.SetMovingDown(true);
		//}

		if (nChar == KEY_A)
			role.SetMovingLeft(true);
		if (nChar == KEY_D)
			role.SetMovingRight(true);
		if (nChar == KEY_W)
			role.SetMovingUp(true);
		if (nChar == KEY_S)
		{
			role.SetMovingDown(true);
		}
			
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

		//if (nChar == KEY_LEFT)
		//	role.SetMovingLeft(false);
		//if (nChar == KEY_RIGHT)
		//	role.SetMovingRight(false);
		//if (nChar == KEY_UP)
		//	role.SetMovingUp(false);
		//if (nChar == KEY_DOWN)
		//{
		//	role.SetMovingDown(false);
		//}

		if (nChar == KEY_A)
			role.SetMovingLeft(false);
		if (nChar == KEY_D)
			role.SetMovingRight(false);
		if (nChar == KEY_W)
			role.SetMovingUp(false);
		if (nChar == KEY_S)
		{
			role.SetMovingDown(false);
		}

	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		role.SetIsFire(true);
		role.SetMouseXY(point.x, point.y);

	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		role.SetIsFire(false);
		role.SetMouseXY(point.x, point.y);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{

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
		
		//background.ShowBitmap();			// �K�W�I����
		//help.ShowBitmap();					// �K�W������
		//

		//corner.SetTopLeft(0, 0);
		//corner.ShowBitmap();
		//corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
		//corner.ShowBitmap();
		//�K�WMIKU
		//miku.onShow();

		#pragma region - paint object -
		//layerManager.Clear();
		#pragma region -- add object to layer --
		/*layerManager.AddObject(miku.GetBitmap(), miku.GetLayer());
		layerManager.AddObject(role.GetAnimate(), role.layer.GetLayer());
		layerManager.AddObject(mapManager.GetBitmap(), mapManager.layer.GetLayer());
		for (int i = 0; i < mapManager.GetNpcNumber(); i++)
		{
			if(mapManager.GetNpcValid(i))
				layerManager.AddObject(mapManager.GetNpc(i), mapManager.GetNpcLayer(i));
		}*/
		#pragma endregion
		layerManager.ShowLayer();
		#pragma endregion

		role.OnShow();

		#pragma region - paint time remain -
		char str[100];
		sprintf(str, "%d", timer.GetTime(2));
		PaintText(str, 300, 0, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��
		#pragma endregion

		//time_left.ShowBitmap();			// �쥻���I���Ѿl����

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
		overBitmap.LoadBitmap(".\\RES\\Gameover.bmp");
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
		/*
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
		*/
		
		overBitmap.SetTopLeft(0, 0);
		overBitmap.ShowBitmap();
		//char str[100];
		//sprintf(str, "��~��~�I~�A�C���N��%d��᭫��", counter / 30);
		//PaintText(str, 100, 180, "�L�n������", 20, RGB(255, 255, 255), RGB(0, 0, 0));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��
	}


	CMiku::CMiku()
	{
		x = y = 0;
		layer = 9;
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

	CMovingBitmap* CMiku::GetBitmap()
	{
		pic.SetTopLeft(x, y);
		return &pic;
	}

	int CMiku::GetLayer()
	{
		return layer;
	}

	void CMiku::SetLayer(int _layer)
	{
		if (_layer >= 10)
			_layer = 9;
		else if (_layer < 0)
			_layer = 0;

		layer = _layer;
	}

	void CMiku::onShow()
	{
		pic.SetTopLeft(x, y);
		pic.ShowBitmap();
	}



	


}//End