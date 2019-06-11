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
#include <string>
#include <fstream>
#include <vector>
using namespace std;
using namespace myLibrary;
namespace game_framework {

	int finalScore = 0;

	#pragma region - CGame Init -
	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
		#pragma region Button Create
		buttonManager.CreateButton(BitmapPath("RES\\Button", "music", 2, RGB(214, 214, 214)), CPoint(250, 420), true, false);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "sound", 2, RGB(214, 214, 214)), CPoint(450, 420), true, false);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "play", 2, RGB(214, 214, 214)), CPoint(230, 200), false, true);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "ending", 2, RGB(214, 214, 214)), CPoint(230, 270), false, true);
		buttonManager.CreateButton(BitmapPath("RES\\Button", "tutorial", 2, RGB(214, 214, 214)), CPoint(230, 340), false, true);
		#pragma endregion

		#pragma region - reset -
		DeleteFile("RES\\Map\\FileName.txt");
		DeleteFile("RES\\Map\\ReloadMapInformation.txt");
		#pragma endregion

	}

	CGameStateInit::~CGameStateInit()
	{
		buttonManager.Clear();
	}

	void CGameStateInit::OnBeginState()
	{
		static bool isLoaded = false;

		CLayerManager::Instance()->Initialize();
		CEndManager::Instance()->Initialize();
		CAudio::Instance()->Initialize();
		CAudio::Instance()->Play("SLoWMoTIoN_Menu", true);
		
		if (!isLoaded)
		{
			buttonManager.Load();
			windowsEnding.LoadResource();
			windowsHandbook.LoadResource("RES\\Handbook\\introduction\\");
			miku.LoadasMascot();
		}

		buttonManager.Initialize();	
		buttonManager.SetValid(true);
		windowsEnding.Initialize(CPoint(0, 0));
		windowsHandbook.Initialize(CPoint(0, 0));
		miku.Initialize();
		miku.SetXY(50, 320);
		miku.GetAction()->SetFaceTo("_R");

		isLoaded = true;
	}

	void CGameStateInit::OnInit()
	{
		ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%

		background.LoadBitmap(".\\RES\\Map\\Menu_3.bmp");
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
			GotoGameState(GAME_STATE_RUN);			// ������GAME_STATE_RUN
		}						
		else if (nChar == KEY_ESC)
		{
			if (windowsEnding.IsOpen())
			{
				windowsEnding.Close();
			}
			else if (windowsHandbook.IsOpen())
			{
				windowsHandbook.Close();
			}
			else
			{
				PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
			}
		}
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (windowsEnding.IsCollisionClose(point))
		{
			windowsEnding.Close();
		}
		else if (windowsHandbook.IsCollisionClose(point))
		{
			windowsHandbook.Close();
		}
		else if (windowsEnding.IsOpen())	 
		{
			string endName = windowsEnding.GetCollisionButtonName(point);
			if (endName != "NoButtonClick") //���I�쵲��
			{
				CAudio::Instance()->Stop("SLoWMoTIoN_Menu");
				CEndManager::Instance()->Start(endName);
				GotoGameState(GAME_STATE_OVER);
			}
		}
		else if (windowsHandbook.IsOpen())
		{
			windowsHandbook.ClickWindows(point, "left");
		}
		else if (buttonManager.GetCollisionButtonName() != "NoButtonClick")
		{
			string btnName = buttonManager.GetCollisionButtonName();
			if (btnName == "music")
			{
				buttonManager.ClickButton("music");
				CAudio::Instance()->SetIsPlayMusic(buttonManager.GetState("music"));
			}
			else if (btnName == "sound")
			{
				buttonManager.ClickButton("sound");
				CAudio::Instance()->SetIsPlaySound(buttonManager.GetState("sound"));
			}
			else if (btnName == "ending")
			{
				windowsEnding.Open();
			}
			else if (btnName == "play")
			{
				GotoGameState(GAME_STATE_RUN);	// ������GAME_STATE_RUN
			}
			else if (btnName == "tutorial")
			{
				windowsHandbook.Open();
			}		
		}
	}

	void CGameStateInit::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
	}

	void CGameStateInit::OnRButtonDown(UINT nFlags, CPoint point)
	{
		if (windowsHandbook.IsOpen())
		{
			windowsHandbook.ClickWindows(point, "right");
		}
	}

	void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		if (windowsEnding.IsOpen())
		{
			windowsEnding.CollisionClose(point);
			return;
		}
		else if (windowsHandbook.IsOpen())
		{
			windowsHandbook.CollisionClose(point);
			return;
		}

		buttonManager.UpdateState(point);
	}

	void CGameStateInit::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
	{	
		//���ϥΪ̤�V�u -> ���U���� (�Ϥ� y -= dy)
		//���� �ϥΪ̤�V�u -> ���W���� (�Ϥ� y += dy)
		windowsEnding.OnScrolling(zDelta);
	}

	void CGameStateInit::OnMove()
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
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

		background.SetTopLeft(0, 0);
		buttonManager.OnCycle();
		buttonManager.SetValid(!(windowsEnding.IsOpen() || windowsHandbook.IsOpen()));
		windowsEnding.OnCycle();
		windowsHandbook.OnCycle();
		miku.GetAction()->OnMove("run");
	}

	void CGameStateInit::OnShow()
	{
		background.ShowBitmap();
		CLayerManager::Instance()->ShowLayer();
		windowsEnding.OnShow();
	}
	#pragma endregion

	#pragma region - CGame Run -
	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g)
	{
		#pragma region - Set eventManager -
		eventManager.SetGameStateRun(this);
		#pragma endregion
		panel.CreatButton();
	}

	CGameStateRun::~CGameStateRun()
	{
		npcManager.Clear();
		bossManager.Clear();
		panel.Clear();
	}

	void CGameStateRun::OnBeginState()
	{
		finalScore = 0;

		CLayerManager::Instance()->Initialize();
		CDialogManager::Instance()->Initialize();
		CCamera::Instance()->Initialize();
		mapManager.Initialize();
		bossManager.Initialize();
		npcManager.Initialize(mapManager.GetNowMap()); //npcManager��l�ƩҦ�NPC���P�ɡA���nowMap�W��NPC
		eventManager.Initialize();
		
		role.Initialize();
		uiManager.Initialize(&role, &bossManager);
		panel.Initialize(CPoint(200, 50));

		timer = CTimer(GAME_TIME);
		nowUsedTimer = &timer;
		CAudio::Instance()->Stop("SLoWMoTIoN_Menu");
		CAudio::Instance()->Play("SLoWMoTIoN_Game");
		isWinXingting = false;
		isWinFacaiSeed = false;
	}

	void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
	{
		ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%

		ShowInitProgress(50);										// ���J�ϧ�
		
		#pragma region - Initialize - MapManager -
		mapManager.LoadMapBitmap();
		
		#pragma endregion

		#pragma region ���J����
		role.Load();
		#pragma endregion

		uiManager.Load();
		panel.LoadResource();
	}

	void CGameStateRun::OnMove() // ���ʹC������
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		
		#pragma region - on panel -
		panel.OnCycle();
		if (panel.IsOpen())
		{
			return;
		}
		#pragma endregion

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
			GoToEnd();
			SwitchState(GAME_STATE_OVER);
		}
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
			if (bossManager.targetBoss != NULL)
			{
				if (bossManager.targetBoss->GetBossId() == "Xingting")
				{
					CEndManager::Instance()->Start(END_NAME_LOSEXINGTING);
					role.SetXY(0, 0);
					SwitchState(GAME_STATE_OVER);
					return;
				}
				if (bossManager.targetBoss->GetBossId() == BOSS_FACAISEED)
				{
					CEndManager::Instance()->Start(END_NAME_LOSE_FACAISEED);
					role.SetXY(640, 0);
					SwitchState(GAME_STATE_OVER);
					return;
				}
			}
			else
			{
				CEndManager::Instance()->Start(END_NAME_SALTEDFISH);
				role.SetXY(0, 0);
				SwitchState(GAME_STATE_OVER);
				return;
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
		//boss �S�� ���F����L�׵�battle����
		if (bossManager.targetBoss != NULL && bossManager.targetBoss->IsEnd()) //��Boss �B Boss�׵�
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
		int mapR = mapManager.GetSplitRight();
		int mapL = mapManager.GetSplitLeft();
		int CameraBoundary_R = (mapManager.GetBitmapWidth() / 2 - mapL) + (mapR - mapManager.GetBitmapWidth() / 2);//���Y�k���
		int CameraBoundary_L = -CameraBoundary_R; //���Y�����
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
				if (screenPosX > mapL)
				{
					CCamera::Instance()->AddX(-MOVE_DISTANCE);
					roleCanMoving = false;
				}
				
				if (bossManager.targetBoss != NULL)
				{
					bossManager.targetBoss->OnMove();
				}
			}
		}
		#pragma endregion

		#pragma region -- Moving Right --

		if (role.GetMovingRight())
		{
			if (role.GetX3() >= SIZE_X / 2)
			{
				if (screenPosX < mapR)
				{
					CCamera::Instance()->AddX(MOVE_DISTANCE);
					//role.SetCanMoving(false);
					roleCanMoving = false;
				}
			}
		}
		#pragma endregion

		mapManager.OnMove();

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
			roleCanMoving = false;
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
				role.GetAction()->SetAction("idle");
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
			bossManager.targetBoss->OnCycle(&role);
		}
		#pragma endregion


		#pragma region - Collision -

		scallions = role.GetScallion();	//���X�������а��I��
		passerbys = mapManager.GetPasserby(); //���Xpasserby���иI��
		for (vector<CScallion*>::iterator scallionk = scallions->begin(); scallionk != scallions->end(); )
		{
			for (vector<CMonster*>::iterator passerbyj = passerbys->begin(); passerbyj != passerbys->end(); )
			{
				if ((*scallionk)->IsCollision(*passerbyj) && (*passerbyj)->GetValid() && (*scallionk)->IsAlive())
				{
					CAudio::Instance()->Play("hit");
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
		for (vector<CMonster*>::iterator passerbyj = passerbys->begin(); passerbyj != passerbys->end(); passerbyj++)
		{
			(*passerbyj)->SetMoving();
			(*passerbyj)->OnMove();
		}
		#pragma endregion
		
		#pragma region - NPC Moving -
		npcManager.OnCycle(mapManager.GetNowMap(), CPoint(screenPosX, role.GetY1()));
		#pragma endregion

		uiManager.OnCycle(nowUsedTimer->GetTime(1));
		panel.OnCycle();
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
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
					(*npciter)->SetTalked(true);
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

		if (nChar == 'K')
		{
			role.SetRoleNoSubHp();
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 27)	//ESC
		{
			if (panel.IsOpen())
				panel.Close();
			else
				panel.Open();
		}

		if (nChar == KEY_A)
			role.SetMovingLeft(false);
		if (nChar == KEY_D)
			role.SetMovingRight(false);
		if (nChar == KEY_W)
			role.SetMovingUp(false);
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		if (panel.IsOpen() && panel.IsCollisionClose(point))
		{
			panel.Close();
			return;
		}
		
		role.SetIsFire(true & !CDialogManager::Instance()->GetDialogState());
		if (CDialogManager::Instance()->GetDialogState())
		{
			CDialogManager::Instance()->Next();
		}
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		
		role.SetIsFire(false);

		if (!panel.IsOpen())
		{
			return;
		}
		

		if (panel.GetCollisionButtonName() != "NoButtonClick")
		{
			string btnName = panel.GetCollisionButtonName();
			if (btnName == "resume")
			{
				panel.Close();

			}
			else if (btnName == "restart")	//�L���̦n close button �i�H�� �o�ӦP��Ĳ�o
			{
				panel.Close();
				if (CDialogManager::Instance()->GetDialogState())
				{
					CDialogManager::Instance()->Stop();
				}
				GotoGameState(GAME_STATE_RUN);
			}
			else if (btnName == "menu")
			{
				panel.Close();
				if (CDialogManager::Instance()->GetDialogState())
				{
					CDialogManager::Instance()->Stop();
				}
				GotoGameState(GAME_STATE_INIT);	// ������GAME_STATE_RUN
			}
			else if (btnName == "exit")
			{
				panel.Clear();
				PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
			}
		}
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		role.SetMouseXY(point.x, point.y);

		if (panel.IsOpen())
		{
			panel.CollisionClose(point);
			panel.UpdateMouse(point);
		}
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
	{
		
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		
	}

	void CGameStateRun::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
	{
		if (zDelta > 0)
		{
			role.AddScore(5);
		}
		else if (zDelta < 0)
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
			mapManager.ChangeMap(mapManager.GetLeftMap(), "left");
			role.SetXY(SIZE_X - (role.GetX3() - role.GetX1()), role.GetY1() - 1);
		}
		else if (dir == "right")
		{
			nextMap = mapManager.GetRightMap();
			mapManager.ChangeMap(mapManager.GetRightMap(), "right");

			role.SetXY(0 - (role.GetX3() - role.GetX1()), role.GetY1() - 1);
		}
		bossManager.TargetBoss(mapManager.GetNowMap());
		npcManager.ChangeMap(nowMap, nextMap);
	}

	void CGameStateRun::ChangeMap(int nextMap)
	{
		int nowMap = mapManager.GetNowMap();
		if (nextMap != -1 && nextMap < mapManager.GetBlockMapSize()) //nextMap in blockMap
		{
			mapManager.ChangeMap(nextMap, "right");
			bossManager.TargetBoss(mapManager.GetNowMap());
			npcManager.ChangeMap(nowMap, nextMap);

			role.SetXY(0, SIZE_Y - role.Height());
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
	}

	void CGameStateRun::GoToEnd()
	{
		#pragma region - is the role talking to all npc -
		bool IsTalkedAllNpc = true;
		vector<CNPC*>* npc = npcManager.GetNpc(mapManager.GetNowMap());
		for (vector<CNPC*>::iterator npciter = npc->begin(); npciter != npc->end(); npciter++)
		{
			if (!(*npciter)->IsTalked())
			{
				IsTalkedAllNpc = false;
			}
		}
		#pragma endregion

		if (isWinFacaiSeed)
		{
			CEndManager::Instance()->Start(END_NAME_WIN_FACAISEED);
		}
		else if (isWinXingting)
		{
			CEndManager::Instance()->Start(END_NAME_WINXINGTING);
		}
		else if (IsTalkedAllNpc)
		{
			CEndManager::Instance()->Start(END_NAME_SALTEDFISH);
		}
		else if (role.GetScore() > 999)
		{
			CEndManager::Instance()->Start(END_NAME_SALTEDFISH);
		}
		else
		{
			CEndManager::Instance()->Start(END_NAME_SALTEDFISH);
		}
	}

	void CGameStateRun::OnShow()
	{
		panel.OnShow();

		#pragma region - paint layer -
		CLayerManager::Instance()->ShowLayer();
		#pragma endregion
	}
	#pragma endregion

	#pragma region - CGame Over -
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
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 5; // 5 seconds
		alpha = 0;
		timer_exit.ResetTime(1.5);
		CAudio::Instance()->Play("SLoWMoTIoN_Gameover");
		CAudio::Instance()->Stop("SLoWMoTIoN_Game");

		canSwitchState = false;
		canDrawGameOverImage = false;

		#pragma region - init -
		CLayerManager::Instance()->Initialize();
		CDialogManager::Instance()->Initialize();
		#pragma endregion		

	}

	void CGameStateOver::OnInit()
	{
		overBitmap.LoadBitmap(".\\RES\\Map\\Gameover.bmp");
		gameOverImage.SetBmp("RES\\Map\\Gameover.bmp");
		gameOverImage.SetFadeInOut(60, -40);

		ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%

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
		CLayerManager::Instance()->ShowLayer(); //���layerManager
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
	}
	#pragma endregion

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

		if (nChar == 9) //Key Tab
		{
			mapEditer.isPrintNowMap = !mapEditer.isPrintNowMap;
		}

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
		if (!mapEditer.IsInSelectMapMode()) //���b�]�w�a�ϼҦ���
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
		if (mapEditer.IsInSelectMapMode()) //�b��ܦa�Ϫ��Ҧ���
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
		if (!mapEditer.IsInSelectMapMode())
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
		if(mapEditer.IsInSelectMapMode()) //��ܦa�Ϥ� - �U���\�ण�ʧ@
		{
			return;
		}

		#pragma region - �P�_�ɮ׬O�_�s�b -
		bool isDataExist = false; //�����]�ɮפ��s�b
		fstream dataFileName;

		dataFileName.open("RES\\Map\\FileName.txt");
		#pragma region -- �ɮצs�b --
		if (dataFileName.is_open()) //���ɮ״Nload�L
		{
			vector<string> fileContext;
			fileContext.clear();
			string fileName;
			while (dataFileName >> fileName)
			{
				fileContext.push_back(fileName);
			}
			mapEditer.AddImage(fileContext);
			isDataExist = true; //�ɮצs�b
		}
		#pragma endregion
		dataFileName.close(); //����

		#pragma region -- �ɮׯu���s�b - �R���ɮ�^^ --
		if (isDataExist)
		{
			DeleteFile("RES\\Map\\FileName.txt");
		}
		#pragma endregion

		#pragma endregion

		#pragma region - �즲block -
		if (mapEditer.GetMouseState()) //�ƹ�����
		{
			mapEditer.DragBlock(mousePoint);
		}
		#pragma endregion
		
		mapEditer.OnMove();

	}

	void CGameStateMapEditer::OnShow()
	{
		mapEditer.OnShow();
		PaintText("MapEditer", 0, 445, "�L�n������", 20, RGB(0, 0, 0), RGB(255, 255, 255));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��

		if (mapEditer.isPrintNowMap)
		{
			char *address = ConvertCharPointToString(mapEditer.GetNowMap());
			PaintText(address, 320, 0, "�L�n������", 20, RGB(0, 0, 0), RGB(255, 255, 255));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��
			delete address;
		}

		if (mapEditer.IsInSelectMapMode())
		{
			char *address = ConvertCharPointToString(mapEditer.GetSelectMapMode());
			PaintText(address, 90, 0, "�L�n������", 20, RGB(0, 0, 0), RGB(255, 255, 255));		//Text ,��m, ��r�r��(sp), ��r�j�p, ��r�C��, �I���C��
			delete address;
		}
	}	

	#pragma endregion
}//End