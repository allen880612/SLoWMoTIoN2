#pragma once
#include "CLibrary.h"
#include "CEraser.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class����¶��骺�y
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////

	#pragma region - ball -
	class CBall
	{
	public:
		CBall();
		//bool HitEraser(CEraser *eraser);						// �O�_�I�����l
		bool IsAlive();											// �O�_����
		void LoadBitmap();										// ���J�ϧ�

		void OnMove();											// ����
		void OnShow();											// �N�ϧζK��e��
		void SetCurrentXY(double, double);
		void SetXY(int nx, int ny);								// �]�w��ߪ��y��
		void SetIsAlive(bool alive);							// �]�w�O�_����
		void SetDelay(int d);									// �]�w���઺�t��
		CPoint GetCenterPoint() { return CPoint(x + animation.Width() / 2, y + animation.Height() / 2); };

		void LoadBitmap(string, string, int);
		void LoadBitmap(BitmapPath);
		CAnimate* GetAnimate();
		CLayer layer;
	protected:
		CAnimate animation;
		double currentX, currentY;     // /*��e���y��*/ //
		int x, y;					// ��ߪ��y��
		int dx, dy;					// �y�Z����ߪ��첾�q
		int index;					// �y���u���סv�A0-17���0-360��
		int delay_counter;			// �վ����t�ת��p�ƾ�
		int delay;					// ���઺�t��
		bool is_alive;				// �O�_����
	private:
		//bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
	};
#pragma endregion

	#pragma region - CScallion -
	class CScallion : public CBall
	{
	public:
		CScallion();
		CScallion(BitmapPath, CPoint, CPoint, int = 4);	//����CPoint + int ���ѼƮɡA�Ĥ@�Ӭ��ثe�y�СA�ĤG�Ӭ��ƹ��y�СA�̫ᬰ���O�Ѽ�
		CScallion(BitmapPath, CPoint, int, int);			//�Y�������O�ѼơA�Ĥ@��CPoint���ܡA���᭱�ܬ��ux��t, y��t�v
		//~CScallion();

		void OnMove();
		void OnShow();
		void SetInitVelocity(int, int, int, int, int = 5);
		void SetInitVelocity(int, int);
		bool IsCollision(CPasserby);
		bool IsCollision(CPasserby*);
		void Clear();
		void Initialize(CPoint);

	protected:

		int velocity_x;
		int velocity_y;
		int gravity;
		int direction;			//��X�ɭ��諸��V
	};
	#pragma endregion

	#pragma region - CBlackHole -
	class CBlackHole : public CScallion
	{
	public:
		CBlackHole();
		CBlackHole(BitmapPath, CPoint, CPoint, int = 4);	//����CPoint + int ���ѼƮɡA�Ĥ@�Ӭ��ثe�y�СA�ĤG�Ӭ��ƹ��y�СA�̫ᬰ���O�Ѽ�
		CBlackHole(BitmapPath, CPoint, int, int);			//�Y�������O�ѼơA�Ĥ@��CPoint���ܡA���᭱�ܬ��ux��t, y��t�v
		CRect GetCenterRect() { return centerRect; };
		void OnMove();
		void SetRole(CRole* _r) { role = _r; };
		int GetX3() { return x + animation.Width() / 2; };
		bool IsDead() { return isDead; };
		void Initialize();
	private:
		CTimer selfBang; //���ztimer
		CRect centerRect;
		CRect centerRectSize;
		CRole *role;
		int csize = 100;
		bool isDead;
	};
	#pragma endregion

}