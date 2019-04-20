/*
* mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////
	int finalScore = 0;

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
		
	}

	void CGameStateInit::OnBeginState()
	{
		CAudio::Instance()->Initialize();
		CAudio::Instance()->Play("MUSIC_MENU", true);
		btn_music = CButton(BitmapPath("RES\\Button", "music", 2, RGB(214, 214, 214)), CPoint(250, 420), true);
		btn_sound = CButton(BitmapPath("RES\\Button", "sound", 2, RGB(214, 214, 214)), CPoint(450, 420), true);
		btn_music.Initialize();
		btn_sound.Initialize();
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
								//
								// 開始載入資料
								//
		logo.LoadBitmap(".\\RES\\Map\\Menu.bmp");
		
		
		//Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
								//
								// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
								//
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;

		if (nChar == KEY_SPACE)
		{
			//CAudio::Instance()->Stop(AUDIO_MENU);
			GotoGameState(GAME_STATE_RUN);			// 切換至GAME_STATE_RUN
		}						
		else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (IsPointInRect(mouse, btn_music.GetAnimate()->GetRect()))
		{
			btn_music.SetState(!btn_music.GetState());	//ㄎㄧㄤ 可能改 ChageState 會好點?
			CAudio::Instance()->SetIsPlayMusic(btn_music.GetState());

		}
		if (IsPointInRect(mouse, btn_sound.GetAnimate()->GetRect()))
		{
			btn_sound.SetState(!btn_sound.GetState());
			CAudio::Instance()->SetIsPlaySound(btn_sound.GetState());
		}
		
		if (!(IsPointInRect(mouse, btn_music.GetAnimate()->GetRect()) || IsPointInRect(mouse, btn_sound.GetAnimate()->GetRect())))
		{
			GotoGameState(GAME_STATE_RUN);
		}

		//CAudio::Instance()->Stop("AUDIO_MENU");
		//GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
	}

	void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
	}

	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		mouse = point;

	}

	void CGameStateInit::OnMove()
	{
		if (!btn_music.GetState())	//music mute
		{
			CAudio::Instance()->Pause();
		}
		else
		{
			CAudio::Instance()->Resume();
			//CAudio::Instance()->Play("MUSIC_MENU", true);
		}
		logo.SetTopLeft(0, 0);
		btn_music.OnMove();
		btn_sound.OnMove();
		/*btn_music.SetXY(200, 200);
		btn_sound.SetXY(400, 200);*/
	}

	void CGameStateInit::OnShow()
	{
		/*CEraser *fuck;
		CNPC ddd;
		fuck = &ddd;*/
		//
		// 貼上logo
		//
		//logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
		logo.ShowBitmap();
		btn_music.OnShow();
		btn_sound.OnShow();


		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		
		/*
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(120, 220, "Please click mouse or press SPACE to begin.");
		pDC->TextOut(5, 395, "Press Ctrl-F to switch in between window mode and full screen mode.");
		if (enable_game_pause)
			pdc->textout(5, 425, "press ctrl-q to pause the game.");
		pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
		*/
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
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
	}

	void CGameStateRun::OnBeginState()
	{
		finalScore = 0;
		const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int TIME_LEFT = 20;			//遊戲秒數
		const int TIME_LEFT_X = 590;
		const int TIME_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;

		for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
			int x_pos = i % BALL_PER_ROW;
			int y_pos = i / BALL_PER_ROW;
			ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
			ball[i].SetDelay(x_pos);
			ball[i].SetIsAlive(true);
		}

		CLayerManager::Instance()->Initialize();
		CDialogManager::Instance()->Initialize();
		CCamera::Instance()->Initialize();
		mapManager.Initialize();
		bossManager.Initialize();
		npcManager.Initialize(mapManager.GetNowMap()); //npcManager初始化所有NPC的同時，顯示nowMap上的NPC

		role.Initialize();
		//role.Initialize(AUDIO_THROW, AUDIO_JUMP);
		//background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
		//help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
		//time_left.SetInteger(TIME_LEFT);					// 指定剩下的撞擊數
		//time_left.SetTopLeft(TIME_LEFT_X, TIME_LEFT_Y);		// 指定剩下撞擊數的座標
		////CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
		////CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
		////CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
		timer = CTimer(GAME_TIME); //ㄎㄧㄤ==
		CAudio::Instance()->Stop("MUSIC_MENU");
		CAudio::Instance()->Play("MUSIC_GAMEING");
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//

		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
								//
								// 開始載入資料
								//
		//miku.LoadBitmap();
		int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].LoadBitmap();								// 載入第i個球的圖形
																//role.LoadBitmap();
		//role.LoadBitmap("Role", "MIKU", 13, RGB(255, 255, 255));
		
		//background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
																//
																// 完成部分Loading動作，提高進度
																//
		ShowInitProgress(50);
		//Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
		//			//
		//			// 繼續載入其他資料
		//			//
		//help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
		//corner.LoadBitmap(IDB_CORNER);								// 載入角落圖形
		//corner.ShowBitmap(background);								// 將corner貼到background
		//															//bball.LoadBitmap();										// 載入圖形
		//time_left.LoadBitmap();

		/*CAudio::Instance()->Load(AUDIO_GAMEING, "sounds\\SLoWMoTIoN_Game.wav");
		CAudio::Instance()->Load(AUDIO_THROW, "sounds\\throw.wav");
		CAudio::Instance()->Load(AUDIO_JUMP, "sounds\\jump.wav"); 
		CAudio::Instance()->Load(AUDIO_HIT, "sounds\\hit.wav");*/
												
																	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
																	//
		#pragma region - Initialize - MapManager -
		mapManager.LoadMapBitmap();
		role.Initialize();
		#pragma endregion

		
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR));

		#pragma region - pause game state in dialoging -
		if (CDialogManager::Instance()->GetDialogState())
		{
			CDialogManager::Instance()->OnCycle();
			return;
		}
		#pragma endregion

		
		#pragma region - timer count down -
		timer.CountDown();
		if (timer.IsTimeOut())
		{
			finalScore = role.GetScore();
			CAudio::Instance()->Stop("MUSIC_GAMEING");
			GotoGameState(GAME_STATE_OVER);
		}
		/*if (counter % 30 == 0)
			time_left.Add(-1);*/
		#pragma endregion
		
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		
		//
		// 移動背景圖的座標
		//
		
		// 移動MIKU
		//miku.OnMove();

		// 移動球
		//
		//int i;
		//for (i = 0; i < NUMBALLS; i++)
		//	ball[i].OnMove();
		//
		// 移動擦子
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
		//int CameraBoundary_right = SIZE_X + ((mapManager.GetBitmapWidth() - SIZE_X) / 2) - SIZE_X;//鏡頭右邊界
		//int CameraBoundary_Left = -(((mapManager.GetBitmapWidth() - SIZE_X) / 2) - SIZE_X) - SIZE_X;//鏡頭左邊界
		int mapR = mapManager.GetSplitRight();
		int mapL = mapManager.GetSplitLeft();
		int CameraBoundary_R = (mapManager.GetBitmapWidth() / 2 - mapL) + (mapR - mapManager.GetBitmapWidth() / 2);//鏡頭右邊界
		int CameraBoundary_L = -CameraBoundary_R;//鏡頭左邊界
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

		#pragma region -- 左右界地圖檢查 - 卡邊界or換地圖 --
		

		#pragma region --- 右邊有地圖且人物往右邊行走 ---
		if (role.GetX2() >= SIZE_X && mapManager.GetRightMap() >= 0 && role.GetMovingRight())
		{
			#pragma region ---- 超過邊界 - 換地圖 ----
			if (role.GetX1() >= SIZE_X)
			{
				ChangeMap("right");
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region --- 右邊沒有地圖且人物往右邊行走 ---
		if (role.GetX2() >= SIZE_X && role.GetMovingRight() && (mapManager.GetRightMap() < 0 || bossManager.IsBattle()))
		{
			role.SetCanMoving(false); //沒有地圖，卡邊界
		}
		#pragma endregion

		#pragma region --- 左邊有地圖且人物往左邊行走 ---
		if (role.GetX1() <= 0 && mapManager.GetLeftMap() >= 0 && role.GetMovingLeft())
		{
			#pragma region ---- 超過邊界 - 換地圖 ----
			if (role.GetX2() <= 0) //超過邊界，換地圖
			{
				ChangeMap("left");
			}
			#pragma endregion
		}
		#pragma endregion

		#pragma region --- 左邊沒有地圖且人物往左邊行走 ---
		if (role.GetX1() <= 0 && role.GetMovingLeft() && (mapManager.GetLeftMap() < 0 || bossManager.IsBattle()))
		{
			role.SetCanMoving(false); //沒有地圖，卡邊界
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
				role.SetXY(role.GetX1(), SIZE_Y - role.Height() - 1);
			}
			else
			{
				role.SetMovingJump(true);
				role.SetCanJumping(false);
			}

		}
		#pragma endregion
		
		#pragma region - boss attack -
		if (bossManager.targetBoss != NULL && bossManager.IsBattle())
		{
			//bossManager.targetBoss->Attack1(&role);
			bossManager.targetBoss->Attack2(&role);
		}
		#pragma endregion


		#pragma region - Collision -

		scallions = role.GetScallion();			//取出蔥的指標做碰撞
		passerbys = mapManager.GetPasserby();	//取出passerby指標碰撞
		for (vector<CScallion*>::iterator scallionk = scallions->begin(); scallionk != scallions->end(); )
		{
			for (vector<CPasserby*>::iterator passerbyj = passerbys->begin(); passerbyj != passerbys->end(); )
			{
				if ((*scallionk)->IsCollision(*passerbyj) && (*passerbyj)->GetValid() && (*scallionk)->IsAlive())
				{
					CAudio::Instance()->Play("SOUND_HIT");
					role.AddScore((*passerbyj)->GetScore());
					(*scallionk)->SetIsAlive(false);
					
					mapManager.DeletePasserby(passerbyj); //從mapManager的passerbyManager中移除passerby
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
		
		//
		// 判斷擦子是否碰到球
		//
		//for (i = 0; i < NUMBALLS; i++)
		//	if (ball[i].IsAlive() && ball[i].HitEraser(&role)) {
		//		ball[i].SetIsAlive(false);
		//		CAudio::Instance()->Play(AUDIO_DING);
		//		time_left.Add(0);
		//		//
		//		// 若剩餘碰撞次數為0，則跳到Game Over狀態
		//		//
		//		if (time_left.GetInteger() <= 0) {
		//			CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
		//			CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
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
			CDialogManager::Instance()->Start(RoleVSBoss);
		}

		if (nChar == KEY_Z) //dialog with npc
		{
			vector<CNPC*>* npc = npcManager.GetNpc(mapManager.GetNowMap());
			for (vector<CNPC*>::iterator npciter = npc->begin(); npciter != npc->end(); npciter++)
			{
				if (role.IsCollisionNPC(*npciter))
				{
					(*npciter)->collision();
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

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
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

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		role.SetIsFire(false);
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
		role.SetMouseXY(point.x, point.y);
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{

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

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		//
		//  貼上背景圖、撞擊數、球、擦子、彈跳的球
		//
		
		//background.ShowBitmap();			// 貼上背景圖
		//help.ShowBitmap();					// 貼上說明圖
		//

		//corner.SetTopLeft(0, 0);
		//corner.ShowBitmap();
		//corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
		//corner.ShowBitmap();
		//貼上MIKU
		//miku.onShow();

		#pragma region - paint object -
		
		CLayerManager::Instance()->ShowLayer();

		#pragma endregion

		//role.OnShow();

		#pragma region - paint time remain -
		char str[100], roleScore[100], rolehp[100];
		sprintf(str, "%d", timer.GetTime(2));
		sprintf(roleScore, "%d", role.GetScore());
		PaintText(str, 300, 0, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色
		PaintText(roleScore, 500, 0, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色


		sprintf(rolehp, "%d", role.GetHp());
		PaintText(rolehp, 20, 100, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色
		#pragma endregion
		#pragma endregion

		CDialogManager::Instance()->ShowText();
		//time_left.ShowBitmap();			// 原本的碰撞剩餘次數

	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;
		if (counter < 0)
		{
			CAudio::Instance()->Stop("SOUND_GAMEOVER");
			GotoGameState(GAME_STATE_INIT);
		}
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
		CAudio::Instance()->Play("SOUND_GAMEOVER");
	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//

		//CAudio::Instance()->Load(AUDIO_GAMEOVER, "sounds\\SLoWMoTIoN_Gameover.wav");
		overBitmap.LoadBitmap(".\\RES\\Map\\Gameover.bmp");
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
								//
								// 開始載入資料
								//
		Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
								//
								// 最終進度為100%
								//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		/*
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f, *fp;
		f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 0));
		char str[80];								// Demo 數字對字串的轉換
		sprintf(str, "Game Over ! (%d)", counter / 30);
		pDC->TextOut(240, 210, str);
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
		*/
		
		overBitmap.SetTopLeft(0, 0);
		overBitmap.ShowBitmap();
		char str[100];
		sprintf(str, "Score: %d", finalScore);
		PaintText(str, 100, 360, "微軟正黑體", 20, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色
	}

}//End