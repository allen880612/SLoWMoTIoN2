#pragma once
#include "CBoss.h"
#include "CLibrary.h"
namespace game_framework {
  #pragma region - CEraser -
  class CBlock;
  class CEraser
  {
    public:
      CEraser();
      virtual ~CEraser();
      int  GetX1(); // 左上角 x 座標
      int  GetY1(); // 左上角 y 座標
      int  GetX3(); // 中心點 X 座標
      int  GetX2(); // 右下角 x 座標
      int  GetY2(); // 右下角 y 座標
      int  GetY3(); // 中心點 Y 座標
      int  GetScore();
      int  Height();
      int  Width();
      bool GetMovingLeft(); // 方方是否在往左走
      bool GetMovingRight(); // 方方是否再往右走
      void Initialize(); // 初始化
      // 由路徑載入圖形(資料夾, name, 張數)
      void LoadBitmap(string, string, int, COLORREF);
      void LoadBitmap(BitmapPath);
      void OnMove(); // 移動擦子
      void OnShow(); // 將擦子圖形貼到畫面
      CAnimate* GetAnimate();
      void SetMovingDown(bool flag); // 設定是否正在往下移動
      void SetMovingLeft(bool flag); // 設定是否正在往左移動
      void SetMovingRight(bool flag); // 設定是否正在往右移動
      void SetMovingUp(bool flag); // 設定是否正在往上移動
      void SetXY(int nx, int ny); // 設定擦子左上角座標
      void SetCanMoving(bool);
      bool GetCanMoving();
      CLayer layer;
      bool GetValid();
      void SetValid(bool);
      #pragma region - collision -
      virtual void ResetCollisionRect();
      bool IsCollisionBlock(CBlock*); //一般左右 跟block碰撞
      bool IsRoleOnBlock(CBlock*); //站在方塊上
      #pragma endregion
    protected:
      CAnimate animation; // 擦子的動畫
      int height, width; // 擦子的高、寬
      int x, y; // 擦子左上角座標
      int score;
      bool isMovingDown; // 是否正在往下移動
      bool isMovingLeft; // 是否正在往左移動
      bool isMovingRight; // 是否正在往右移動
      bool isMovingUp; // 是否正在往上移動
      bool canMoving; // 是否可以移動
      CRect collisionRect;
      CRect collisionDownRect;
    private:
      int move_distance = MOVE_DISTANCE;
  };
  #pragma endregion
  #pragma region - CNPC -
  class CNPC : public CEraser
  {
    public:
      CNPC();
      CNPC(CPoint, BitmapPath, string, double);
      virtual ~CNPC();
      void SetCurrentXY(int, int);
      void SetXY();
      void LookRole(CPoint);
      virtual void OnCycle(CPoint);
      virtual void OnMove();
      virtual void Initialize();
      virtual void RoleCollision() {}; //碰撞到後要做的事情
      virtual void SetValid(bool);
      void SetTalked(bool f) {
        isTalked = f;
      };
      bool IsTalked() {
        return isTalked;
      };
    protected:
      #pragma region - init information -
      CPoint initPoint;
      BitmapPath initLoadPath;
      double initResetTime;
      #pragma endregion
      CAnimate leftAnimate;
      CAnimate rightAnimate;
      int currentX, currentY;
      string id;
      string faceTo;
      bool isTalked;
  };
  #pragma endregion
  #pragma region - CRole -
  class CBlackHole;
  class CDoor;
  class CRole : public CEraser
  {
      friend class CScallion;
    public:
      CRole();
      ~CRole();
      int  GetX3(); // 中新點 X 座標
      int  GetX2(); // 擦子右下角 x 座標
      int  GetY2(); // 擦子右下角 y 座標
      int  GetY3(); // 中心點 Y 座標
      int  Height();
      int  Width();
      void SetXY(int _x, int _y); // 設定擦子左上角座標
      bool GetValid();
      void SetValid(bool);
      void Load();
      void LoadasMascot();
      void LoadAction(string, BitmapPath); //載入動應動作
      void OnMove();
      bool GetMovingJump();
      void SetMovingJump(bool);
      bool GetCanJumping();
      void SetCanJumping(bool);
      void SetMouseXY(int, int);
      void AddScore(int);
      void SubHp();
      int  GetHp() {
        return hp;
      };
      void SubEq();
      int  GetEq() {
        return eq;
      };
      void Initialize();
      bool GetIsFire();
      void SetIsFire(bool);
      void Fire(int, int); //傳入滑鼠座標
      void SetDrop();
      bool GetDrop();
      int GetVelocity();
      void SetVelocity(int v) {
        velocity = v;
      };
      bool IsMoving();
      bool IsDead() {
        return isDead;
      };
      bool IsZZ() {
        return isZZ;
      };
      void SetRoleNoSubHp() {
        isRoleNoSubHp = !isRoleNoSubHp;
      };
      CAction* GetAction() {
        return &action;
      };
      #pragma region - Collision -
      bool IsCollisionBoss(CBoss*);
      #pragma region -- Collision - Block --
      bool IsCollisionBlockOnJumping(CBlock*); //跳起來撞到方塊
      #pragma endregion
      bool IsCollisionDoor(CDoor*);
      bool IsCollisionLevel4(CScallion*);
      bool IsCollisionNPC(CNPC*);
      bool IsCollisionBlackHole(CBlackHole*);
      bool IsCollisionBlackHoleCenter(CBlackHole*);
      bool IsCollisionRay(CRay*);
      #pragma endregion
      vector<CScallion*>* GetScallion();
      void SetCatched(bool flag) {
        isCatched = flag;
      };
      bool IsCatched() {
        return isCatched;
      };
    protected:
      vector<CScallion*>	scallion;
      bool isJumping; // 是否正在跳躍
      bool canJumping; // 是否可以跳躍
      bool isFire; // 是否正在射擊
      int init_velocity; // 往上的初速度
      int velocity; // 速度
      int gravity; // 重力
      char last_right_left;
      int mouse_x, mouse_y;
      int move_distance = MOVE_DISTANCE; //每次移動的距離
      CTimer shoot_cd;
      CMovingBitmap decisionPoint;
      CRect collisionTopRect;
      CRect collisionDoorRect;
      void ResetCollisionRect();
    private:
      CAction  action;
      #pragma region - init information -
      int inithp = 20;
      int initEq = 20;
      bool isLoaded;
      string now_action;
      #pragma endregion
      int hp, eq;
      bool isCatched;
      bool isDead;
      bool isZZ;
      bool isRoleNoSubHp;
  };
  #pragma endregion
  #pragma region - CMonster -
  class CMonster : public CEraser
  {
      friend class CScallion;
    public:
      CMonster();
      CMonster(int, int);
      void SetBlock(vector<CBlock>* bkvector) {
        blockVector = bkvector;
      }
      void SetXY(int, int);
      void SetScore(int);
      void SetMoving();
      virtual void OnMove();
      virtual ~CMonster();
    protected:
      int initX;
      CTimer moveTimer;
      CTimer stopTimer;
      CTimer recreateTimer;
      int move_distance = 2;
      CAnimate leftAnimate;
      CAnimate rightAnimate;
      string faceto;
      bool onFloor;
      bool moveleft, moveright;
      vector<CBlock>* blockVector;
      void ResetCollisionRect();
  };
  #pragma region - CMonster1 - LUKA -
  class CMonsterType1 : public CMonster
  {
    public:
      CMonsterType1();
      CMonsterType1(int, int, BitmapPath, int);
      ~CMonsterType1();
  };
  #pragma endregion
  #pragma region - CMonster2 - mushroom -
  class CMonsterType2 : public CMonster
  {
    public:
      CMonsterType2();
      CMonsterType2(int, int, BitmapPath, int);
      void OnMove();
      ~CMonsterType2();
  };
  #pragma endregion
  #pragma endregion
  #pragma region - CNPC - No.1 -
  //CNPC1 > 普通的NPC (對話NPC)
  class CNPC1 : public CNPC
  {
    public:
      CNPC1();
      CNPC1(CPoint, BitmapPath, string, string, double resetTime = 0.05); //座標 路徑 id 對話文本 一個動畫的影格的時間
      ~CNPC1();
      void RoleCollision();
    private:
      string txt;
  };
  #pragma endregion
  #pragma region - CNPC - No.3 -
  //CNPC3 > 音樂播放NPC
  class CNPC3 : public CNPC
  {
    public:
      CNPC3();
      CNPC3(CPoint, BitmapPath, string, string, string); //座標 路徑 ID 音樂 對話文本
      ~CNPC3();
      void RoleCollision();
      void Initialize();
      void SetValid(bool);
      void OnMove();
    private:
      string openTxt;
      string openMusic;
      CMovingBitmap headphoneIcon;
      bool isLoadHeadPhoneIcon;
  };
  #pragma endregion
}