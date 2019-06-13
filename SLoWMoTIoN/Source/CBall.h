#pragma once
#include "CLibrary.h"
#include "CEraser.h"
#include "CBoss.h"

namespace game_framework {

  #pragma region - ball -
  class CBall
  {
    public:
      CBall();
      bool IsAlive(); // �O�_����

      void SetCurrentXY(double, double);
      void SetXY(int nx, int ny); // �]�w��ߪ��y��
      void SetIsAlive(bool alive); // �]�w�O�_����
      CPoint GetCenterPoint() {
        return CPoint(x + animation.Width() / 2, y + animation.Height() / 2);
      };

      void LoadBitmap(string, string, int);
      void LoadBitmap(BitmapPath);
      CAnimate* GetAnimate();
      CLayer layer;
    protected:
      CAnimate animation;
      double currentX, currentY;  //��e���y��
      int x, y; // ��ߪ��y��
      bool is_alive;	// �O�_����
  };
  #pragma endregion

  #pragma region - CScallion -
  class CScallion : public CBall
  {
    public:
      CScallion();
      CScallion(BitmapPath, CPoint, CPoint, int = 2);	//����CPoint + int ���ѼƮɡA�Ĥ@�Ӭ��ثe�y�СA�ĤG�Ӭ��ƹ��y�СA�̫ᬰ���O�Ѽ�
      CScallion(BitmapPath, CPoint, int, int); //�Y�������O�ѼơA�Ĥ@��CPoint���ܡA���᭱�ܬ��ux��t, y��t�v

      void OnMove();
      void OnShow();
      void SetInitVelocity(int, int, int, int, int = 5);
      void SetInitVelocity(int, int);
      bool IsCollision(CMonster);
      bool IsCollision(CMonster*);
      bool IsCollision(CBoss*);
      bool IsCollision(CRect);
      void Clear();
      void Initialize(CPoint);
      int  GetAtk() {
        return atk;
      };
      void SetAtk(int k) {
        atk = k;
      };

    protected:
      int atk = 1;
      int velocity_x;
      int velocity_y;
      int gravity;
      int direction; //��X�ɭ��諸��V
  };
  #pragma endregion

  #pragma region - CBlackHole -
  class CBlackHole : public CScallion
  {
    public:
      CBlackHole();
      CBlackHole(BitmapPath, CPoint, CPoint, int = 4); //����CPoint + int ���ѼƮɡA�Ĥ@�Ӭ��ثe�y�СA�ĤG�Ӭ��ƹ��y�СA�̫ᬰ���O�Ѽ�
      CBlackHole(BitmapPath, CPoint, int, int); //�Y�������O�ѼơA�Ĥ@��CPoint���ܡA���᭱�ܬ��ux��t, y��t�v
      CRect GetCenterRect() {
        return centerRect;
      };
      void OnMove();
      void SetRole(CRole* _r) {
        role = _r;
      };
      void Initialize();
    private:
      CTimer selfBang; //���ztimer
      CRect centerRect;
      CRect centerRectSize;
      CRole* role;
      int csize = 100;
  };
  #pragma endregion

  #pragma region - CRay -
  class CRay
  {
    public:
      CRay();
      CRay(BitmapPath, CPoint, double = 0.1); //�ʵe���|, �_�l��m, �C�Ӽv�涡�j�ɶ�
      ~CRay();

      void OnMove();

      bool GetValid() {
        return animation.GetValid();
      };
      void SetValid(bool f) {
        animation.SetValid(f);
      };
      CAnimate* GetAnimate() {
        return &animation;
      };
      void SetXY(int _x, int _y) {
        x = _x;
        y = _y;
        animation.SetTopLeft(x, y);
      }
      bool GetAttackValid() {
        return AttackValid;
      };
    private:
      CAnimate animation;
      int x, y;
      bool AttackValid; //��������
  };
  #pragma endregion
}