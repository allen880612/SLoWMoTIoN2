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
#include <string>
#include <fstream>
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
		static bool isLoaded = false;

		CLayerManager::Instance()->Initialize();
		CAudio::Instance()->Initialize();
		CAudio::Instance()->Play("SLoWMoTIoN_Menu", true);
		
		//temp
		if (!isLoaded)
		{
			#pragma region Button Create
			buttonManager.CreateButton(BitmapPath("RES\\Button", "music", 2, RGB(214, 214, 214)), CPoint(250, 420), true, false);
			buttonManager.CreateButton(BitmapPath("RES\\Button", "sound", 2, RGB(214, 214, 214)), CPoint(450, 420), true, false);
			buttonManager.CreateButton(BitmapPath("RES\\Button", "play", 2, RGB(214, 214, 214)), CPoint(350, 190), false, true);
			buttonManager.CreateButton(BitmapPath("RES\\Button", "ending", 2, RGB(214, 214, 214)), CPoint(350, 260), false, true);
			buttonManager.CreateButton(BitmapPath("RES\\Button", "about", 2, RGB(214, 214, 214)), CPoint(350, 330), false, true);
			#pragma endregion
			buttonManager.Load();
			windowsEnding.LoadResource();
			//windowsEnding.PushButtonToButtonManager(&buttonManager);
			//frame.LoadBitmap(".\\RES\\UI\\blood_frame.bmp", RGB(214, 214, 214));
			//blood.LoadBitmap(".\\RES\\UI\\blood.bmp", RGB(214, 214, 214));
		}

		buttonManager.Initialize();		
		windowsEnding.Initialize(CPoint(60, 60));

		isLoaded = true;
		IsKeyCtrl = false;
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

	void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 'N')
		{
			GotoGameState(GAME_STATE_MAPEDITER);
		}
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
		if (windowsEnding.IsCollisionClose(point))
		{
			windowsEnding.Close();
		}
		else if (windowsEnding.IsOpen())	 
		{
			string endName = windowsEnding.GetCollisionButtonName(point);
			if (endName != "NoButtonClick") //有點到結局
			{
				CEndManager::Instance()->Start(endName);
				GotoGameState(GAME_STATE_OVER);
			}
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
		else if (buttonManager.IsCollisionMouse("ending"))	//他媽最好 close button 可以跟 這個同時觸發
		{
			windowsEnding.Open();
		}
		else if (buttonManager.IsCollisionMouse("play"))
		{
			GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
		}

	}

	void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
	}

	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
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
		if (zDelta > 0) //往使用者方向滾 -> 往下捲動 (圖片 y -= dy)
		{
			
		}
		else			//遠離 使用者方向滾 -> 往上捲動 (圖片 y += dy)
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
		logo.ShowBitmap();
		CLayerManager::Instance()->ShowLayer();
		windowsEnding.OnShow();
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

		//for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
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
		npcManager.Initialize(mapManager.GetNowMap()); //npcManager初始化所有NPC的同時，顯示nowMap上的NPC
		eventManager.Initialize();
		
		role.Initialize();
		uiManager.Initialize(&role, &bossManager);

		timer = CTimer(GAME_TIME); //ㄎㄧㄤ==
		nowUsedTimer = &timer;
		CAudio::Instance()->Stop("SLoWMoTIoN_Menu");
		CAudio::Instance()->Play("SLoWMoTIoN_Game");
		isWinXingting = false;
		isWinFacaiSeed = false;
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
		/*int i;
		for (i = 0; i < NUMBALLS; i++)
			ball[i].LoadBitmap();	*/							// 載入第i個球的圖形

		ShowInitProgress(50);										// 載入圖形
		
		#pragma region - Initialize - MapManager -
		mapManager.LoadMapBitmap();
		
		#pragma endregion

		#pragma region 載入角色
		role.Load();
		/*role.LoadAction("idle", BitmapPath("RES\\Role\\miku\\idle", "idle", 19, RGB(150, 200, 250)));
		role.LoadAction("run", BitmapPath("RES\\Role\\miku\\run", "run", 7, RGB(150, 200, 250)));
		role.LoadAction("jump", BitmapPath("RES\\Role\\miku\\jump", "jump", 7, RGB(150, 200, 250)));*/
		#pragma endregion

		uiManager.Load();
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
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
			CAudio::Instance()->Stop("SLoWMoTIoN_Game");
			if (isWinXingting)
			{
				CEndManager::Instance()->Start(END_NAME_WINXINGTING);
			}
			else
			{
				CEndManager::Instance()->Start(END_NAME_SALTEDFISH);
			}
			SwitchState(GAME_STATE_OVER);
		}
		/*if (nowUsedTimer->GetTime() == (int)nowUsedTimer->GetTime())
		{
			time_left.Add(-1);
		}*/


		#pragma endregion
		
		#pragma region - run boss endprocess -
		if (bossManager.targetBoss != NULL && bossManager.targetBoss->InEndProcess())
		{
			bossManager.targetBoss->EndProcess();
			return;
		}
		#pragma endregion

		bool roleCanMoving = true;

		#pragma region - role - dead -
		if (role.IsDead())
		{
			if (bossManager.targetBoss->GetBossId() == "Xingting")
			{
				CEndManager::Instance()->Start(END_NAME_LOSEXINGTING);
				role.SetXY(0, 0);
				SwitchState(GAME_STATE_OVER);
			}
			if (bossManager.targetBoss->GetBossId() == BOSS_FACAISEED)
			{
				CEndManager::Instance()->Start(END_NAME_LOSEXINGTING);
				role.SetXY(640, 0);
				SwitchState(GAME_STATE_OVER);
			}
		}
		#pragma endregion

		#pragma region - boos - dead -
		if (bossManager.targetBoss != NULL && bossManager.targetBoss->IsDead())
		{
			if (bossManager.targetBoss->GetBossId() == "Xingting")
			{
				isWinXingting = true;
			}
			if (bossManager.targetBoss->GetBossId() == BOSS_FACAISEED)
			{
				isWinFacaiSeed = true;
			}
			bossManager.BossDead();
			SwitchTimer(&timer);
		}
		#pragma endregion

		#pragma region - end battle -
		//boss 沒死 但達成其他終結battle條件
		if (bossManager.targetBoss != NULL && bossManager.targetBoss->IsEnd()) //有Boss 且 Boss終結
		{
			bossManager.BossDead();
			SwitchTimer(&timer);
		}
		#pragma endregion

		#pragma region - Position Trigger -
		PositionTrigger();
		#pragma endregion

		#pragma region - Moving -
		int screenPosX = ScreenX(mapManager.GetX1(), role.GetX3());

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
					if (bossManager.targetBoss->GetID() != "Xingting" && bossManager.targetBoss->GetID() != "FacaiSeed")
					{
						CCamera::Instance()->SetCanMoving(false);
						//role.SetCanMoving(false);
						roleCanMoving = false;
					}
				}
			}
		}
		#pragma endregion

		#pragma region - role - collision with block -
		vector<CBlock> *block = mapManager.GetBlockVector();
		bool isOnFloor = false;
		int floorY = SIZE_Y;
		for (vector<CBlock>::iterator bkiter = block->begin(); bkiter != block->end(); bkiter++)
		{
			if (role.IsCollisionBlock(&(*bkiter))) //collision with block
			{
				CCamera::Instance()->SetCanMoving(false);
				//role.SetCanMoving(false);
				roleCanMoving = false;
			}

			if (role.IsRoleOnBlock(&(*bkiter)) && role.GetDrop())
			{
				floorY = bkiter->y;
				isOnFloor = true;
			}

			if (role.IsCollisionBlockOnJumping(&(*bkiter)) && !role.GetDrop())
			{
				role.SetDrop();
			}
		}
		if (role.GetY2() >= SIZE_Y - 1)
		{
			floorY = SIZE_Y - 1;
			isOnFloor = true;
		}
		#pragma endregion

		#pragma region -- Moving Left --
		if (role.GetMovingLeft())
		{

			if (role.GetX3() <= SIZE_X / 2)
			{
				//mapManager.SetMovingLeft(true);
				//mapManager.SetMovingRight(false);
				if (/*CCamera::Instance()->GetX() > 0 &&*/ screenPosX > mapL)
				{
					CCamera::Instance()->AddX(-MOVE_DISTANCE);
					//role.SetCanMoving(false);
					roleCanMoving = false;
				}
				else
				{
					//role.SetCanMoving(true);
				}
				
				if (bossManager.targetBoss != NULL)
				{
					bossManager.targetBoss->OnMove();
				}
			}
			else
			{
				//role.SetCanMoving(true);
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
				if (/*CCamera::Instance()->GetX() < CameraBoundary_R &&*/ screenPosX < mapR)
				{
					CCamera::Instance()->AddX(MOVE_DISTANCE);
					//role.SetCanMoving(false);
					roleCanMoving = false;
				}
				else
				{
					//role.SetCanMoving(true);
				}
			}
			else
			{
				//mapManager.SetMovingLeft(false);
				//mapManager.SetMovingRight(false);
				//role.SetCanMoving(true);
			}
		}
		#pragma endregion

		mapManager.OnMove();

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
			//role.SetCanMoving(false); //沒有地圖，卡邊界
			roleCanMoving = false;
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
			//role.SetCanMoving(false); //沒有地圖，卡邊界
			roleCanMoving = false;
		}
		#pragma endregion
		#pragma endregion

		#pragma region -- role Moving --
		role.SetCanMoving(roleCanMoving);
		role.OnMove();
		#pragma endregion
		#pragma endregion

		#pragma region - Jump -
		bool isDrop = false;
		if (role.GetMovingJump())
		{
			if ((role.GetY2() - role.GetVelocity() >= SIZE_Y || isOnFloor) && role.GetDrop()) //on Floor
			{
				int ddy = 2;
				role.SetMovingJump(false);
				role.SetCanJumping(true);
				role.GetAction()->SetAction("idle");	//髒髒
				role.SetXY(role.GetX1(), floorY - role.Height() + ddy);
			}
			else //Jumping
			{
				role.SetMovingJump(true);
				role.SetCanJumping(false);
			}
		}
		else
		{
			if (!isOnFloor)
			{
				role.SetDrop();
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

		scallions = role.GetScallion();			//取出蔥的指標做碰撞
		passerbys = mapManager.GetPasserby();	//取出passerby指標碰撞
		for (vector<CScallion*>::iterator scallionk = scallions->begin(); scallionk != scallions->end(); )
		{
			for (vector<CPasserby*>::iterator passerbyj = passerbys->begin(); passerbyj != passerbys->end(); )
			{
				if ((*scallionk)->IsCollision(*passerbyj) && (*passerbyj)->GetValid() && (*scallionk)->IsAlive())
				{
					CAudio::Instance()->Play("hit");
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
		
		#pragma region - NPC Moving -
		npcManager.OnCycle(mapManager.GetNowMap(), CPoint(screenPosX, role.GetY1()));
		#pragma endregion

		uiManager.OnCycle(nowUsedTimer->GetTime(1));

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
		{
			bool canRoleJump = true;
			for (vector<CBlock>::iterator bkiter = mapManager.GetBlockVector()->begin(); bkiter != mapManager.GetBlockVector()->end(); bkiter++)
			{
				if (role.GetCanJumping())
				{
					if (role.IsCollisionBlockOnJumping(&(*bkiter)))
					{
						canRoleJump = false;
					}
				}
			}
			if(canRoleJump)
				role.SetMovingUp(true);
		}

		if (nChar == 'U')
		{
			nowUsedTimer->ResetTime(5.0);
			/*time_left.SetInteger(nowUsedTimer->GetTime(2));*/
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

		if (nChar == ' ') //open door
		{
			#pragma region - role - collision with door -
			vector<CDoor> *door = mapManager.GetDoorVector();
			for (vector<CDoor>::iterator bkiter = door->begin(); bkiter != door->end(); bkiter++)
			{
				if (role.IsCollisionDoor(&(*bkiter))) //collision with door
				{
					ChangeMap(bkiter->GetSwitchMapIndex());
				}
			}
			#pragma endregion
		}

		#pragma region - 沒路用ING -
		//if (nChar == KEY_S)
		//{
		//	role.SetMovingDown(false);
		//}

		//if (nChar == KEY_Q)
		//{
		//	CDialogManager::Instance()->Start(DIALOG_DATA_VSXingting1);
		//}

		//if (nChar == 67)
		//{
		//	CAudio::Instance()->Pause();
		//	//CAudio::Instance()->Play("MyVoiceIsDead");
		//}

		//if (nChar == KEY_B)
		//{
		//	CAudio::Instance()->Resume();
		//}
		//if (nChar == 86)
		//{
		//	CAudio::Instance()->Play("MyVoiceIsDead");
		//	//CAudio::Instance()->Play("SLoWMoTIoN_Gameover");
		//}

		//if (nChar == 'N')
		//{
		//	role.SetMovingJump(false);
		//	role.SetCanJumping(true);
		//}
		#pragma endregion
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

		if (nChar == 27)
		{
			GotoGameState(GAME_STATE_INIT);
		}

		if (nChar == KEY_A)
			role.SetMovingLeft(false);
		if (nChar == KEY_D)
			role.SetMovingRight(false);
		if (nChar == KEY_W)
			role.SetMovingUp(false);
		/*if (nChar == KEY_S)
		{
			role.SetMovingDown(false);
		}*/

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

	void CGameStateRun::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
	{
		//測試用：上滾score增加，下滾減少
		if (zDelta > 0) //往使用者方向滾 -> 往下捲動 (圖片 y -= dy)
		{
			role.AddScore(5);
		}
		else			//遠離 使用者方向滾 -> 往上捲動 (圖片 y += dy)
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
			//CCamera::Instance()->SetXY(mapManager.GetBitmapWidth() / 2, 0);
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

	void CGameStateRun::ChangeMap(int nextMap)
	{
		int nowMap = mapManager.GetNowMap();
		if (nextMap != -1 && nextMap < mapManager.GetBlockMapSize()) //nextMap in blockMap
		{
			role.SetXY(0, SIZE_Y - role.Height() - 1);
			mapManager.ChangeMap(nextMap, "right");
			bossManager.TargetBoss(mapManager.GetNowMap());
			npcManager.ChangeMap(nowMap, nextMap);
		}
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
		/*time_left.SetInteger(nowUsedTimer->GetTime(2), 2);*/
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
		//char roleScore[100], rolehp[100];
		//sprintf(roleScore, "%d", role.GetScore());
		//PaintText(roleScore, 500, 0, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色


		//sprintf(rolehp, "%d", role.GetHp());
		//PaintText(rolehp, 20, 100, "Consolas", 32, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色
		#pragma endregion
		#pragma endregion

		CDialogManager::Instance()->ShowText();
		role.OnShow();
		//time_left.ShowBitmap();	// 剩餘時間\
		//hp_left.ShowBitmap();	// 剩餘HP	
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
			CAudio::Instance()->Stop("SLoWMoTIoN_Gameover");
			GotoGameState(GAME_STATE_INIT);
		}*/
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
		alpha = 0;
		timer_exit.ResetTime(1.5);
		CAudio::Instance()->Play("SLoWMoTIoN_Gameover");
		CAudio::Instance()->Stop("SLoWMoTIoN_Game");
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
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//

		//CAudio::Instance()->Load(AUDIO_GAMEOVER, "sounds\\SLoWMoTIoN_Gameover.wav");
		overBitmap.LoadBitmap(".\\RES\\Map\\Gameover.bmp");
		gameOverImage.SetBmp("RES\\Map\\Gameover.bmp");
		gameOverImage.SetFadeInOut(60, -40);

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

	void CGameStateOver::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 'Y')
		{
			if (CDialogManager::Instance()->GetDialogState())
			{
				CDialogManager::Instance()->Stop();
			}
		}
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
		CLayerManager::Instance()->ShowLayer(); //顯示layerManager
		if (CDialogManager::Instance()->GetDialogState()) //顯示文字
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
		
		//if (time_remaining.GetTime() <= 1.5)	//之後放個 timer 來算那張圖應該要多久後，開始淡出
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
		//PaintText(str, 100, 360, "微軟正黑體", 20, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色
	}

	#pragma region - MapEditer -
	CGameStateMapEditer::CGameStateMapEditer(CGame * g) : CGameState(g)
	{
	}

	CGameStateMapEditer::~CGameStateMapEditer()
	{
	}

	void CGameStateMapEditer::OnInit()
	{
	}

	void CGameStateMapEditer::OnBeginState()
	{
		mapEditer.Initialize();
		//mapEditer.LoadMapInfo();
	}

	void CGameStateMapEditer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 'S')
		{
			mapEditer.OnSave();
		}

		if (nChar == 'A') // map move left
		{
			mapEditer.SetMapMoveDir("left");
		}
		if (nChar == 'D') //map move right
		{
			mapEditer.SetMapMoveDir("right");
		}

		if (nChar == 'K')
		{
		}

		if (nChar == 9) //Key Tab
		{
			mapEditer.isPrintNowMap = !mapEditer.isPrintNowMap;
		}

		//maybe ctrl = 11

		if (nChar == 46) //Key Delete
		{
			if (mapEditer.IsInSelectMapMode())
			{
				mapEditer.SetSelectMapZero();
			}
			else
			{
				mapEditer.DeleteBlock();
			}
		}

		#pragma region - Set upMap / downMap / leftMap / rightMap -
		if (!mapEditer.IsInSelectMapMode()) //不在設定地圖模式中
		{
			string dir[4] = { "up", "down", "left", "right" };
			//Key 1, 2, 3, 4
			if (nChar >= '1' && nChar <= '4')
			{
				mapEditer.SetSelectMapMode(dir[nChar - 48 - 1]);
			}
		}
		#pragma endregion
	}

	void CGameStateMapEditer::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 27) //Key ESC, return to stateInit
		{
			if (!mapEditer.IsInSelectMapMode())
			{
				mapEditer.StoreMapInformaion();
				mapEditer.CreateReloadMapInformation();
				GotoGameState(GAME_STATE_INIT);
			}
			else
			{
				mapEditer.SetSelectMapMode("none");
			}
		}
		if (nChar == 'A')
		{
			mapEditer.SetMapMoveDir("cleft");
		}
		if (nChar == 'D')
		{
			mapEditer.SetMapMoveDir("cright");
		}
	}

	void CGameStateMapEditer::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (mapEditer.IsInSelectMapMode()) //在選擇地圖的模式中
		{
			mapEditer.ClickArrow(point);
		}
		else
		{
			mapEditer.SetMouseState(true);
			mapEditer.SelectBlock(point);
		}
	}

	void CGameStateMapEditer::OnLButtonUp(UINT nFlags, CPoint point)
	{
		if (mapEditer.IsInSelectMapMode())
		{

		}
		else
		{
			mapEditer.SetMouseState(false);
		}
	}

	void CGameStateMapEditer::OnMouseMove(UINT nFlags, CPoint point)
	{
		mousePoint = point;
	}

	void CGameStateMapEditer::OnRButtonDown(UINT nFlags, CPoint point)
	{
		
	}

	void CGameStateMapEditer::OnRButtonUp(UINT nFlags, CPoint point)
	{
	}

	void CGameStateMapEditer::OnMove()
	{
		if(mapEditer.IsInSelectMapMode()) //選擇地圖中 - 下面功能不動作
		{
			return;
		}

		#pragma region - 判斷檔案是否存在 -
		bool isDataExist = false; //先假設檔案不存在
		fstream dataFileName;

		dataFileName.open("RES\\Map\\FileName.txt");
		#pragma region -- 檔案存在 --
		if (dataFileName.is_open()) //有檔案就load他
		{
			vector<string> fileContext;
			fileContext.clear();
			string fileName;
			while (dataFileName >> fileName)
			{
				fileContext.push_back(fileName);
			}
			mapEditer.AddImage(fileContext);
			isDataExist = true; //檔案存在
		}
		#pragma endregion
		dataFileName.close(); //關檔

		#pragma region -- 檔案真的存在 - 刪除檔案^^ --
		if (isDataExist)
		{
			DeleteFile("RES\\Map\\FileName.txt");
		}
		#pragma endregion

		#pragma endregion

		#pragma region - 拖曳block -
		if (mapEditer.GetMouseState()) //滑鼠按住中
		{
			mapEditer.DragBlock(mousePoint);
		}
		#pragma endregion
		
		mapEditer.OnMove();

	}

	void CGameStateMapEditer::OnShow()
	{
		mapEditer.OnShow();
		PaintText("EditerTest", 100, 360, "微軟正黑體", 20, RGB(255, 255, 255), RGB(255, 0, 0));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色

		if (mapEditer.isPrintNowMap)
		{
			char *address = ConvertCharPointToString(mapEditer.GetNowMap());
			PaintText(address, 320, 0, "微軟正黑體", 20, RGB(0, 0, 0), RGB(255, 255, 255));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色
			delete address;
		}

		if (mapEditer.IsInSelectMapMode())
		{
			char *address = ConvertCharPointToString(mapEditer.GetSelectMapMode());
			PaintText(address, 90, 0, "微軟正黑體", 20, RGB(0, 0, 0), RGB(255, 255, 255));		//Text ,位置, 文字字形(sp), 文字大小, 文字顏色, 背景顏色
			delete address;
		}
	}	

	#pragma endregion

}//End