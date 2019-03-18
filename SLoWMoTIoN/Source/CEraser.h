#pragma once
#include "CLibrary.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�������l
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////
	
	class CEraser
	{
	public:
		CEraser();
		~CEraser();
		int  GetX1();					// ���l���W�� x �y��
		int  GetY1();					// ���l���W�� y �y��
		int  GetX3();                   // ���s�I X �y��
		int  GetX2();					// ���l�k�U�� x �y��
		int  GetY2();					// ���l�k�U�� y �y��
		int  Height();
		int  Width();
		bool GetMovingLeft();           // ���O�_�b������
		bool GetMovingRight();          // ���O�_�A���k��
		void Initialize();				// �]�w���l����l��
		void LoadBitmap();				// ���J�ϧ�
		// �Ѹ��|���J�ϧ�(��Ƶ�, name, �i��)
		void LoadBitmap(string, string, int);
		
		void OnMove();					// �������l
		void OnShow();					// �N���l�ϧζK��e��
		CAnimate* GetAnimate();
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w���l���W���y��

		bool GetMovingJump();
		void SetMovingJump(bool);
		bool GetCanJumping();
		void SetCanJumping(bool);
		
		void SetCanMoving(bool);
		bool GetCanMoving();
		CLayer layer;

	protected:
		CAnimate animation;			// ���l���ʵe
		int height, width;			// ���l�����B�e
		int x, y;					// ���l���W���y��
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		bool canMoving;				// �O�_�i�H����

		bool isJumping;				// �O�_���b���D
		bool canJumping;			// �O�_�i�H���D
		int init_velocity;			// ���W����t��
		int velocity;				// �t��
		int gravity;				// ���O
	};
}