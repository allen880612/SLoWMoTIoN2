/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/
#pragma once
#include "CEraser.h"
#include "CBall.h"
#include "CManager.h"
#include "CBoss.h"
#include "Refactor.h"
namespace game_framework {
  class CGameStateInit : public CGameState {
    public:
      CGameStateInit(CGame* g);
      ~CGameStateInit();
      void OnBeginState(); // 設定每次重玩所需的變數
      void OnInit(); // 遊戲的初值及圖形設定
      void OnKeyDown(UINT, UINT, UINT); // 處理鍵盤Down的動作
      void OnKeyUp(UINT, UINT, UINT); // 處理鍵盤Up的動作
      void OnLButtonDown(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnLButtonUp(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnRButtonDown(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnMouseMove(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
    protected:
      void OnMove();
      void OnShow(); // 顯示這個狀態的遊戲畫面
    private:
      CButtonManager buttonManager;
      CPoint mouse;
      CMovingBitmap background; // background
      CRole miku;
      CScrollWindow windowsEnding;
      CSwitchWindow windowsHandbook;
  };

  class CGameStateRun : public CGameState {
      friend class CEventManager;
    public:
      CGameStateRun(CGame* g);
      ~CGameStateRun();
      void OnBeginState(); // 設定每次重玩所需的變數
      void OnInit(); // 遊戲的初值及圖形設定
      void OnKeyDown(UINT, UINT, UINT);
      void OnKeyUp(UINT, UINT, UINT);
      void OnLButtonDown(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnLButtonUp(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnMouseMove(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnRButtonDown(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnRButtonUp(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
    protected:
      void OnMove(); // 移動遊戲元素
      void OnShow(); // 顯示這個狀態的遊戲畫面
    private:
      CRole role;
      CInteger time_left;	// 剩下的撞擊數
      CTimer timer; //stateRun狀態下的timer (其實好像在什麼狀態下都無所謂) (gameRun timer)
      CTimer* nowUsedTimer; //現在被使用的計時器 (BOSS戰鬥的時候 原本的遊戲計時器不會倒數)
      vector<CScallion*>* scallions;
      vector<CMonster*>* passerbys;
      CMapManager mapManager;
      CBossManager bossManager;
      CNPCManager npcManager;
      CEventManager eventManager;
      CStatusBoard roleStatus;
      CBossBoard bossStatus;
      UIManager uiManager;
      CPanel panel;
      bool isWinXingting;
      bool isWinFacaiSeed;
      #pragma region - 自定義函數 -
      void PositionTrigger();
      void ChangeMap(string);
      void ChangeMap(int);
      void SwitchState(int);
      void SwitchTimer(CTimer*);
      void GoToEnd();
      #pragma endregion
  };

  class CGameStateOver : public CGameState {
    public:
      CGameStateOver(CGame* g);
      void OnBeginState(); // 設定每次重玩所需的變數
      void OnInit();
      void OnKeyDown(UINT, UINT, UINT);
      void OnLButtonDown(UINT nFlags, CPoint point);
    protected:
      void OnMove(); // 移動遊戲元素
      void OnShow(); // 顯示這個狀態的遊戲畫面
    private:
      int counter; // 倒數之計數器
      int alpha;
      bool canSwitchState = false;
      bool canDrawGameOverImage = false;
      CTimer timer_exit; //結局結束後要多久時間離開GameOver
      CMovingBitmap overBitmap;
      CToumeiImage gameOverImage;
      CPanel panel;
  };

  class CGameStateMapEditer : public CGameState
  {
    public:
      CGameStateMapEditer(CGame* g);
      ~CGameStateMapEditer();
      void OnInit(); // 遊戲的初值及圖形設定
      void OnBeginState(); // 設定每次重玩所需的變數
      void OnKeyDown(UINT, UINT, UINT);
      void OnKeyUp(UINT, UINT, UINT);
      void OnLButtonDown(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnLButtonUp(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnMouseMove(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnRButtonDown(UINT nFlags, CPoint point); // 處理滑鼠的動作
      void OnRButtonUp(UINT nFlags, CPoint point); // 處理滑鼠的動作
    protected:
      void OnMove();
      void OnShow();
    private:
      CMapEditer mapEditer;
      CPoint mousePoint;
  };
}