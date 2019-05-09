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
#include "CBall.h"
#include "CLibrary.h"
#include <vector>
using namespace std;
using namespace myLibrary;
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	/////////////////////////////////////////////////////////////////////////////
	int finalScore = 0;

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{

	}

	void CGameStateInit::OnBeginState()
	{
		static bool isLoaded = false;

		CLayerManager::Instance()->Initialize();
		CAudio::Instance()->Initialize();
		CAudio::Instance()->Play("MUSIC_MENU", true);

		#pragma region Button Create
		buttonManager.CreateButton(BitmapPath("RES\\Button", "music", 2, RGB(214, 214, 214)), CPoint(250, 420), true, false);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "sound", 2, RGB(214, 214, 214)), CPoint(450, 420), true, false);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "play", 2, RGB(214, 214, 214)), CPoint(350, 190), false, true);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "ending", 2, RGB(214, 214, 214)), CPoint(350, 260), false, true);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "about", 2, RGB(214, 214, 214)), CPoint(350, 330), false, true);
		#pragma endregion

		if (!isLoaded)
		{
			buttonManager.Load();
			windowsEnding.LoadResource();
		}

		/*btn_music = CButton(BitmapPath("RES\\Button", "music", 2, RGB(214, 214, 214)), CPoint(250, 420), true, false);
		btn_sound = CButton(BitmapPath("RES\\Button", "sound", 2, RGB(214, 214, 214)), CPoint(450, 420), true, false);
		btn_play = CButton(BitmapPath("RES\\Button", "play", 2, RGB(214, 214, 214)), CPoint(350, 190), false, true);
		btn_ending = CButton(BitmapPath("RES\\Button", "ending", 2, RGB(214, 214, 214)), CPoint(350, 260), false, true);
		btn_about = CButton(BitmapPath("RES\\Button", "about", 2, RGB(214, 214, 214)), CPoint(350, 330), false, true);
		btn_music.Initialize();
		btn_sound.Initialize();
		btn_play.Initialize();
		btn_ending.Initialize();
		btn_about.Initialize();*/

		

		buttonManager.Initialize();		
		windowsEnding.Initialize(CPoint(60, 0));

		isLoaded = true;
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
		logo.LoadBitmap(".\\RES\\Map\\Menu.bmp");
		
		

				
		//Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
								//
								// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
								//
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;

		if (nChar == KEY_SPACE)
		{
			//CAudio::Instance()->Stop(AUDIO_MENU);
			GotoGameState(GAME_STATE_RUN);			// ������GAME_STATE_RUN
		}						
		else if (nChar == KEY_ESC)								// Demo �����C������k
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{

		
		if (windowsEnding.IsCollisionClose(point))
		{
			windowsEnding.Close();
		}
		else if (windowsEnding.IsOpen())	 
		{
			//stop the other button
		}
		else if (buttonManager.IsCollisionMouse("music"))
		{
			buttonManager.ClickButton("music");
			CAudio::Instance()->SetIsPlayMusic(buttonManager.GetState("music"));
		}
		else if (buttonManager.IsCollisionMouse("sound"))
		{
			buttonManager.ClickButton("sound");
			CAudio::Instance()->SetIsPlaySound(buttonManager.GetState("sound"));
		}
		else if (buttonManager.IsCollisionMouse("ending"))	//�L���̦n close button �i�H�� �o�ӦP��Ĳ�o
		{
			windowsEnding.Open();
		}
		else if (buttonManager.IsCollisionMouse("play"))
		{
			GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
		}
	
	}

	void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
	}

	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		

		if (windowsEnding.IsOpen())
		{
			windowsEnding.CollisionClose(point);
			return;
		}

		buttonManager.UpdateState(point);
	}

	void CGameStateInit::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
	{	
		if (zDelta > 0) //���ϥΪ̤�V�u -> ���U���� (�Ϥ� y -= dy)
		{
		}
		else			//���� �ϥΪ̤�V�u -> ���W���� (�Ϥ� y += dy)
		{
		}

		windowsEnding.OnScrolling(zDelta);
	}

	void CGameStateInit::OnMove()
	{
		#pragma region mute music with menu_bgm
		if (!buttonManager.GetState("music"))	//music mute
		{
			CAudio::Instance()->Pause();
		}
		else
		{
			CAudio::Instance()->Resume();
		}
		#pragma endregion

		logo.SetTopLeft(0, 0);
		buttonManager.OnCycle();
		windowsEnding.OnCycle();
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

		logo.ShowBitmap();
		CLayerManager::Instance()->ShowLayer();

		windowsEnding.OnShow();

		////
		//// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
		////
		//CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
		//CFont f, *fp;
		//f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
		//fp = pDC->SelectObject(&f);					// ��� font f
		//
		///*
		//pDC->SetBkColor(RGB(0, 0, 0));
		//pDC->SetTextColor(RGB(255, 255, 0));
		//pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		//pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		//if (enable_game_pause)
		//	pdc->textout(5, 425, "press ctrl-q to pause the game.");
		//pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		//*/
		//pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
		//CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g), NUMBALLS(28)
	{
		ball = new CBall[NUMBALLS];
		#pragma region - Set eventManager -
		eventManager.SetGameStateRun(this);
		#pragma endregion
	}

	CGameStateRun::~CGameStateRun()
	{
		delete[] ball;
		npcManager.Clear();
		bossManager.Clear();
	}

	void CGameStateRun::OnBeginState()
	{
		finalScore = 0;
		//const int BALL_GAP = 90;
		//const int BALL_XY_OFFSET = 45;
		//const int BALL_PER_ROW = 7;
		//const int BACKGROUND_X = 60;
		//const int ANIMATION_SPEED = 15;

		//for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
		//	int x_pos = i % BALL_PER_ROW;
		//	int y_pos = i / BALL_PER_ROW;
		//	ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		//	ball[i].SetDelay(x_pos);
		//	ball[i].SetIsAlive(true);
		//}

		CLayerManager::Instance()->Initialize();
		CDialogManager::Instance()->Initialize();
		CCamera::Instance()->Initialize();
		mapManager.Initialize();
		bossManager.Initialize();
		npcManager.Initialize(mapManager.GetNowMap()); //npcManager��l�ƩҦ�NPC���P�ɡA���nowMap�W��NPC

		role.Initialize();
		time_left.Initialize(CPoint(250, 0), GAME_TIME, 2); 
		//hp_left.Initialize(CPoint(20, 100), 20, 2);
		//role.Initialize(AUDIO_THROW, AUDIO_JUMP);
		//background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
		//help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��

		//time_left.SetInteger(GAME_TIME, 2);						// ���w�ѤU���ɶ�
		//time_left.SetTopLeft(TIME_LEFT_X, TIME_LEFT_Y);			// ���w�ѤU�ɶ��ƪ��y��
		//CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
		timer = CTimer(GAME_TIME); //�}����==
		nowUsedTimer = &timer;
		CAudio::Instance()->Stop("MUSIC_MENU");
		CAudio::Instance()->Play("MUSIC_GAMEING");
		isWinXingting = false;
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
		/*int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].LoadBitmap();	*/							// ���J��i�Ӳy���ϧ�

		ShowInitProgress(50);										// ���J�ϧ�
		time_left.LoadBitmap(".\\RES\\Number\\cookiezi", "default");
		
		#pragma region - Initialize - MapManager -
		mapManager.LoadMapBitmap();
		
		#pragma endregion

		#pragma region ���J����
		role.Load();
		/*role.LoadAction("idle", BitmapPath("RES\\Role\\miku\\idle", "idle", 19, RGB(150, 200, 250)));
		role.LoadAction("run", BitmapPath("RES\\Role\\miku\\run", "run", 7, RGB(150, 200, 250)));
		role.LoadAction("jump", BitmapPath("RES\\Role\\miku\\jump", "jump", 7, RGB(150, 200, 250)));*/
		#pragma endregion
	}

	void CGameStateRun::OnMove()							// ���ʹC������
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//SetCursor(AfxGetApp()->LoadCursor("..\\RES\\gamecurs.cur"));
		#pragma region - pause game state in dialoging -
		if (CDialogManager::Instance()->GetDialogState())
		{
			CDialogManager::Instance()->OnCycle();
			return;
		}
		#pragma endregion

		#pragma region - timer count down -
		nowUsedTimer->CountDown();
		if (timer.IsTimeOut())
		{
			finalScore = role.GetScore();
			CAudio::Instance()->Stop("MUSIC_GAMEING");
			if (isWinXingting)
			{
				CEndManager::Instance()->Start(END_NAME_WINXINGTING);
			}
			else
			{
				CEndManager::Instance()->Start(END_NAME_LOSEXINGTING);
			}
			SwitchState(GAME_STATE_OVER);
		}
		if (nowUsedTimer->GetTime() == (int)nowUsedTimer->GetTime())
		{
			time_left.Add(-1);
		}


		#pragma endregion
		
		#pragma region - role - dead -
		if (role.IsDead())
		{
			CEndManager::Instance()->Start(END_NAME_LOSEXINGTING);
			role.SetXY(0, 0);
			SwitchState(GAME_STATE_OVER);
		}
		#pragma endregion

		#pragma region - boos - dead -
		if (bossManager.targetBoss != NULL && bossManager.targetBoss->IsDead())
		{
			if (bossManager.targetBoss->GetBossId() == "Xingting")
			{
				isWinXingting = true;
			}
			bossManager.BossDead();
			SwitchTimer(&timer);
		}
		#pragma endregion


		//
		// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
		//
		
		//
		// ���ʭI���Ϫ��y��
		//
		
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


		

		#pragma region - Position Trigger -
		PositionTrigger();
		#pragma endregion

		#pragma region - Moving -
		int screenPosX = ScreenX(mapManager.GetX1(), role.GetX3());
		/*bool isCollisionBlock = false;
		if (bossManager.targetBoss != NULL)
		{
			isCollisionBlock = role.IsCollisionBoss(bossManager.targetBoss);
		}*/
		//temp

		#pragma region SetCamera
		//CCamera::Instance()->SetCameraBoundary(mapManager.GetSplitLeft(), mapManager.GetSplitRight());
		//int CameraBoundary_right = SIZE_X + ((mapManager.GetBitmapWidth() - SIZE_X) / 2) - SIZE_X;//���Y�k���
		//int CameraBoundary_Left = -(((mapManager.GetBitmapWidth() - SIZE_X) / 2) - SIZE_X) - SIZE_X;//���Y�����
		int mapR = mapManager.GetSplitRight();
		int mapL = mapManager.GetSplitLeft();
		int CameraBoundary_R = (mapManager.GetBitmapWidth() / 2 - mapL) + (mapR - mapManager.GetBitmapWidth() / 2);//���Y�k���
		int CameraBoundary_L = -CameraBoundary_R;//���Y�����
		CCamera::Instance()->SetCanMoving(true);
		#pragma endregion

		#pragma region --- role - collision with boss ---
		if (bossManager.targetBoss != NULL)
		{
			if (bossManager.targetBoss != NULL)
			{
				if (role.IsCollisionBoss(bossManager.targetBoss)) //collision with boss
				{
					CCamera::Instance()->SetCanMoving(false);
					role.SetCanMoving(false);
				}
			}
		}
		#pragma endregion


		#pragma region -- Moving Left --
		if (role.GetMovingLeft())
		{

			if (role.GetX3() <= SIZE_X / 2)
			{
				//mapManager.SetMovingLeft(true);
				//mapManager.SetMovingRight(false);
				if (CCamera::Instance()->GetX() > CameraBoundary_L && screenPosX > mapL)
				{
					CCamera::Instance()->AddX(-MOVE_DISTANCE);
					mapManager.OnMove();
					role.SetCanMoving(false);
				}
				else
				{
					role.SetCanMoving(true);
				}
				
				if (bossManager.targetBoss != NULL)
				{
					//bossManager.targetBoss->MoveWithMap("left");
					bossManager.targetBoss->OnMove();

				}
			}
			else
			{
				//mapManager.SetMovingLeft(false);
				//mapManager.SetMovingRight(false);
				role.SetCanMoving(true);
			}
		}
		#pragma endregion

		#pragma region -- Moving Right --

		if (role.GetMovingRight())
		{
			if (role.GetX3() >= SIZE_X / 2)
			{
				//mapManager.SetMovingLeft(false);
				//mapManager.SetMovingRight(true);
				if (CCamera::Instance()->GetX() < CameraBoundary_R && screenPosX < mapR)
				{
					CCamera::Instance()->AddX(MOVE_DISTANCE);
					mapManager.OnMove();
					role.SetCanMoving(false);
				}
				else
				{
					role.SetCanMoving(true);
				}
				if (bossManager.targetBoss != NULL)
				{
					//bossManager.targetBoss->MoveWithMap("right");
					bossManager.targetBoss->OnMove();
				}
			}
			else
			{
				//mapManager.SetMovingLeft(false);
				//mapManager.SetMovingRight(false);
				role.SetCanMoving(true);
			}
		}
		#pragma endregion

		/*
		if (CCamera::Instance()->GetX() > mapManager.GetSplitRight())
			CCamera::Instance()->SetXY(mapManager.GetSplitRight(), 0);
		if (CCamera::Instance()->GetX() < mapManager.GetSplitLeft())
			CCamera::Instance()->SetXY(mapManager.GetSplitLeft(), 0);*/

		/*#pragma region -- Moving Left --
		if (role.GetMovingLeft())
		{

			if (screenPosX > mapManager.GetSplitLeft() && screenPosX <= mapManager.GetSplitRight())
			{
				mapManager.SetMovingLeft(true);
				mapManager.SetMovingRight(false);
				mapManager.OnMove();
				role.SetCanMoving(false);
				if (bossManager.targetBoss != NULL)
				{
					bossManager.targetBoss->MoveWithMap("left");
				}
			}
			else
			{
				mapManager.SetMovingLeft(false);
				mapManager.SetMovingRight(false);
				role.SetCanMoving(true);
			}
		}
		#pragma endregion
		#pragma region -- Moving Right --
		
		if (role.GetMovingRight())
		{
			if (screenPosX >= mapManager.GetSplitLeft() && screenPosX < mapManager.GetSplitRight())
			{
				mapManager.SetMovingLeft(false);
				mapManager.SetMovingRight(true);
				mapManager.OnMove();
				role.SetCanMoving(false);
				if (bossManager.targetBoss != NULL)
				{
					bossManager.targetBoss->MoveWithMap("right");
				}
			}
			else
			{
				mapManager.SetMovingLeft(false);
				mapManager.SetMovingRight(false);
				role.SetCanMoving(true);
			}
		}
		#pragma endregion*/

		#pragma region -- ���k�ɦa���ˬd - �d���or���a�� --
		

		#pragma region --- �k�䦳�a�ϥB�H�����k��樫 ---
		if (role.GetX2() >= SIZE_X && mapManager.GetRightMap() >= 0 && role.GetMovingRight())
		{
			#pragma region ---- �W�L��� - ���a�� ----
			if (role.GetX1() >= SIZE_X)
			{
				ChangeMap("right");
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region --- �k��S���a�ϥB�H�����k��樫 ---
		if (role.GetX2() >= SIZE_X && role.GetMovingRight() && (mapManager.GetRightMap() < 0 || bossManager.IsBattle()))
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
				ChangeMap("left");
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region --- ����S���a�ϥB�H��������樫 ---
		if (role.GetX1() <= 0 && role.GetMovingLeft() && (mapManager.GetLeftMap() < 0 || bossManager.IsBattle()))
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
			if (role.GetY2() - role.GetVelocity() >= SIZE_Y && role.GetDrop())
			{
				role.SetMovingJump(false);
				role.SetCanJumping(true);
				role.GetAction()->SetAction("idle");	//żż
				role.SetXY(role.GetX1(), SIZE_Y - role.Height() - 1);
			}
			else
			{
				role.SetMovingJump(true);
				role.SetCanJumping(false);
			}

		}
		#pragma endregion
		
		#pragma region - boss cycle (move and attack) -
		if (bossManager.targetBoss != NULL && bossManager.IsBattle())
		{
			//bossManager.targetBoss->Attack1(&role);
			bossManager.targetBoss->OnCycle(&role);
		}
		#pragma endregion


		#pragma region - Collision -

		scallions = role.GetScallion();			//���X�������а��I��
		passerbys = mapManager.GetPasserby();	//���Xpasserby���иI��
		for (vector<CScallion*>::iterator scallionk = scallions->begin(); scallionk != scallions->end(); )
		{
			for (vector<CPasserby*>::iterator passerbyj = passerbys->begin(); passerbyj != passerbys->end(); )
			{
				if ((*scallionk)->IsCollision(*passerbyj) && (*passerbyj)->GetValid() && (*scallionk)->IsAlive())
				{
					CAudio::Instance()->Play("SOUND_HIT");
					role.AddScore((*passerbyj)->GetScore());
					(*scallionk)->SetIsAlive(false);
					
					mapManager.DeletePasserby(passerbyj); //�qmapManager��passerbyManager������passerby
					mapManager.AddPasserby();
					break;
				}
				else
				{
					passerbyj++;
				}
			}

			if (!(*scallionk)->IsAlive())
			{
				delete *scallionk;
				*scallionk = NULL;
				scallionk = scallions->erase(scallionk);
			}
			else
			{
				scallionk++;
			}
		}
		#pragma endregion

		#pragma region - Passerby Moving -
		for (vector<CPasserby*>::iterator passerbyj = passerbys->begin(); passerbyj != passerbys->end(); passerbyj++)
		{
			(*passerbyj)->SetMoving();
			(*passerbyj)->OnMove();
		}
		#pragma endregion
		
		#pragma region - NPC Moving -
		npcManager.OnMove(mapManager.GetNowMap());
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
			role.SetMovingDown(false);
		}
		
		if (nChar == KEY_Q)
		{
			CDialogManager::Instance()->Start(DIALOG_DATA_VSXingting1);
		}

		if (nChar == 67)
		{
			CAudio::Instance()->Pause();
			//CAudio::Instance()->Play("MUSIC_DeadLock");
		}

		if (nChar == KEY_B)
		{
			CAudio::Instance()->Resume();
		}

		if (nChar == 'U')
		{
			
		}
		if (nChar == 'T')
		{
			for (int i = 0; i < 30; i++)
			{
				role.SubHp();
			}
		}
		if (nChar == 'Y')
		{
			if (CDialogManager::Instance()->GetDialogState())
			{
				CDialogManager::Instance()->Stop();
			}
		}
		if (nChar == 86)
		{
			CAudio::Instance()->Play("MUSIC_DeadLock");
			//CAudio::Instance()->Play("SOUND_GAMEOVER");
		}

		if (nChar == KEY_Z) //dialog with npc
		{
			vector<CNPC*>* npc = npcManager.GetNpc(mapManager.GetNowMap());
			for (vector<CNPC*>::iterator npciter = npc->begin(); npciter != npc->end(); npciter++)
			{
				if (role.IsCollisionNPC(*npciter))
				{
					(*npciter)->RoleCollision();
				}
			}
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
		role.SetIsFire(true & !CDialogManager::Instance()->GetDialogState());
		if (CDialogManager::Instance()->GetDialogState())
		{
			CDialogManager::Instance()->Next();
		}
		else
		{
			
		}
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		role.SetIsFire(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
		role.SetMouseXY(point.x, point.y);
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{

	}

	void CGameStateRun::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
	{
		//���եΡG�W�uscore�W�[�A�U�u���
		if (zDelta > 0) //���ϥΪ̤�V�u -> ���U���� (�Ϥ� y -= dy)
		{
			role.AddScore(5);
		}
		else			//���� �ϥΪ̤�V�u -> ���W���� (�Ϥ� y += dy)
		{
			role.AddScore(-5);
		}
	}

	void CGameStateRun::PositionTrigger()
	{
		eventManager.trigger();
	}

	void CGameStateRun::ChangeMap(string dir)
	{
		int nowMap = mapManager.GetNowMap(), nextMap = nowMap;
		if (dir == "left")
		{
			nextMap = mapManager.GetLeftMap();
			role.SetXY(SIZE_X + (role.GetX2() - role.GetX1()), role.GetY1());
			mapManager.ChangeMap(mapManager.GetLeftMap(), "left");
		}
		else if (dir == "right")
		{
			nextMap = mapManager.GetRightMap();
			role.SetXY(0 - (role.GetX2() - role.GetX1()), role.GetY1());
			mapManager.ChangeMap(mapManager.GetRightMap(), "right");
		}
		bossManager.TargetBoss(mapManager.GetNowMap());
		npcManager.ChangeMap(nowMap, nextMap);
	}

	void CGameStateRun::SwitchState(int state)
	{
		if (bossManager.targetBoss != NULL)
		{
			bossManager.targetBoss->Clear();
			bossManager.targetBoss = NULL;
		}
		this->GotoGameState(state);
	}

	void CGameStateRun::SwitchTimer(CTimer *swtimer)
	{
		nowUsedTimer = swtimer;
		time_left.SetInteger(nowUsedTimer->GetTime(2), 2);
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
		
		CLayerManager::Instance()->ShowLayer();

		#pragma endregion

		//role.OnShow();

		#pragma region - paint time remain -
		//char roleScore[100], rolehp[100];
		//sprintf(roleScore, "%d", role.GetScore());
		//PaintText(roleScore, 500, 0, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��


		//sprintf(rolehp, "%d", role.GetHp());
		//PaintText(rolehp, 20, 100, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��
		#pragma endregion
		#pragma endregion

		CDialogManager::Instance()->ShowText();
		role.OnShow();
		time_left.ShowBitmap();	// �Ѿl�ɶ�\
		//hp_left.ShowBitmap();	// �ѾlHP	
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
		if (CDialogManager::Instance()->GetDialogState())
		{
			CDialogManager::Instance()->OnCycle();
			return;
		}

		#pragma region - CountDown for drawGameOverImage -
		if (!CEndManager::Instance()->GetEndingState())
		{
			timer_exit.CountDown();
			if (timer_exit.IsTimeOut())
			{
				canDrawGameOverImage = true;
			}
		}
		#pragma endregion

		/*counter--;
		if (counter < 0)
		{
			CAudio::Instance()->Stop("SOUND_GAMEOVER");
			GotoGameState(GAME_STATE_INIT);
		}*/
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
		alpha = 0;
		timer_exit.ResetTime(1.5);
		CAudio::Instance()->Play("SOUND_GAMEOVER");
		CAudio::Instance()->Stop("MUSIC_GAMEING");
		//CLayerManager::Instance()->Clear();

		canSwitchState = false;
		canDrawGameOverImage = false;

		#pragma region - init -
		CLayerManager::Instance()->Initialize();
		CDialogManager::Instance()->Initialize();
		//CEndManager::Instance()->Initialize();
		#pragma endregion		

	}

	void CGameStateOver::OnInit()
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//

		//CAudio::Instance()->Load(AUDIO_GAMEOVER, "sounds\\SLoWMoTIoN_Gameover.wav");
		overBitmap.LoadBitmap(".\\RES\\Map\\Gameover.bmp");
		gameOverImage.SetBmp("RES\\Map\\Gameover.bmp");
		gameOverImage.SetFadeInOut(60, -40);

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

	void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (CDialogManager::Instance()->GetDialogState())
		{
			CDialogManager::Instance()->Next();
		}
		else
		{
			if (canSwitchState)
			{
				GotoGameState(GAME_STATE_INIT);
			}
		}
	}

	void CGameStateOver::OnShow()
	{
		
		if (CEndManager::Instance()->GetEndingState())
		{
			CEndManager::Instance()->OnCycle();
		}

		#pragma region - Draw Dialog -
		CLayerManager::Instance()->ShowLayer(); //���layerManager
		if (CDialogManager::Instance()->GetDialogState()) //��ܤ�r
		{
			CDialogManager::Instance()->ShowText();
		}
		#pragma endregion

		#pragma region - DrawGameOverImage when Ending end -
		if (canDrawGameOverImage)
		{
			gameOverImage.DrawImage();
			gameOverImage.FadeIn();
			if (gameOverImage.GetAlpha() >= 255)
			{
				canSwitchState = true;
			}
		}
		#pragma endregion




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
		
		//if (time_remaining.GetTime() <= 1.5)	//������ timer �Ӻ⨺�i�����ӭn�h�[��A�}�l�H�X
		//{
		//	DrawBitmap(&ending1, alpha-=4);
		//}
		//else if (alpha < 255)
		//{
		//	DrawBitmap(&ending1, alpha+=3);
		//}
		//else
		//{
		//	DrawBitmap(&ending1, alpha);
		//}
		
		
		

		//overBitmap.SetTopLeft(0, 0);
		//overBitmap.ShowBitmap();
		//char str[100];
		//sprintf(str, "Score: %d", finalScore);
		//PaintText(str, 100, 360, "�L�n������", 20, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��
	}

}//End