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
      int  GetX1(); // ���W�� x �y��
      int  GetY1(); // ���W�� y �y��
      int  GetX3(); // �����I X �y��
      int  GetX2(); // �k�U�� x �y��
      int  GetY2(); // �k�U�� y �y��
      int  GetY3(); // �����I Y �y��
      int  GetScore();
      int  Height();
      int  Width();
      bool GetMovingLeft(); // ���O�_�b������
      bool GetMovingRight(); // ���O�_�A���k��
      void Initialize(); // ��l��
      // �Ѹ��|���J�ϧ�(��Ƨ�, name, �i��)
      void LoadBitmap(string, string, int, COLORREF);
      void LoadBitmap(BitmapPath);
      void OnMove(); // �������l
      void OnShow(); // �N���l�ϧζK��e��
      CAnimate* GetAnimate();
      void SetMovingDown(bool flag); // �]�w�O�_���b���U����
      void SetMovingLeft(bool flag); // �]�w�O�_���b��������
      void SetMovingRight(bool flag); // �]�w�O�_���b���k����
      void SetMovingUp(bool flag); // �]�w�O�_���b���W����
      void SetXY(int nx, int ny); // �]�w���l���W���y��
      void SetCanMoving(bool);
      bool GetCanMoving();
      CLayer layer;
      bool GetValid();
      void SetValid(bool);
      #pragma region - collision -
      virtual void ResetCollisionRect();
      bool IsCollisionBlock(CBlock*); //�@�륪�k ��block�I��
      bool IsRoleOnBlock(CBlock*); //���b����W
      #pragma endregion
    protected:
      CAnimate animation; // ���l���ʵe
      int height, width; // ���l�����B�e
      int x, y; // ���l���W���y��
      int score;
      bool isMovingDown; // �O�_���b���U����
      bool isMovingLeft; // �O�_���b��������
      bool isMovingRight; // �O�_���b���k����
      bool isMovingUp; // �O�_���b���W����
      bool canMoving; // �O�_�i�H����
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
      virtual void RoleCollision() {}; //�I�����n�����Ʊ�
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
      int  GetX3(); // ���s�I X �y��
      int  GetX2(); // ���l�k�U�� x �y��
      int  GetY2(); // ���l�k�U�� y �y��
      int  GetY3(); // �����I Y �y��
      int  Height();
      int  Width();
      void SetXY(int _x, int _y); // �]�w���l���W���y��
      bool GetValid();
      void SetValid(bool);
      void Load();
      void LoadasMascot();
      void LoadAction(string, BitmapPath); //���J�����ʧ@
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
      void Fire(int, int); //�ǤJ�ƹ��y��
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
      bool IsCollisionBlockOnJumping(CBlock*); //���_�Ӽ�����
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
      bool isJumping; // �O�_���b���D
      bool canJumping; // �O�_�i�H���D
      bool isFire; // �O�_���b�g��
      int init_velocity; // ���W����t��
      int velocity; // �t��
      int gravity; // ���O
      char last_right_left;
      int mouse_x, mouse_y;
      int move_distance = MOVE_DISTANCE; //�C�����ʪ��Z��
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
  //CNPC1 > ���q��NPC (���NPC)
  class CNPC1 : public CNPC
  {
    public:
      CNPC1();
      CNPC1(CPoint, BitmapPath, string, string, double resetTime = 0.05); //�y�� ���| id ��ܤ奻 �@�Ӱʵe���v�檺�ɶ�
      ~CNPC1();
      void RoleCollision();
    private:
      string txt;
  };
  #pragma endregion
  #pragma region - CNPC - No.3 -
  //CNPC3 > ���ּ���NPC
  class CNPC3 : public CNPC
  {
    public:
      CNPC3();
      CNPC3(CPoint, BitmapPath, string, string, string); //�y�� ���| ID ���� ��ܤ奻
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