#pragma once
#include "CLibrary.h"
namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H����L�ηƹ�������l
	// �����N�i�H��g���ۤv���{���F
	/////////////////////////////////////////////////////////////////////////////
	
	#pragma region - CEraser -
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
		int GetScore();
		int  Height();
		int  Width();
		bool GetMovingLeft();           // ���O�_�b������
		bool GetMovingRight();          // ���O�_�A���k��
		void Initialize();				// �]�w���l����l��
		void LoadBitmap();				// ���J�ϧ�
		// �Ѹ��|���J�ϧ�(��Ƶ�, name, �i��)
		void LoadBitmap(string, string, int, COLORREF);
		
		void OnMove();					// �������l
		void OnShow();					// �N���l�ϧζK��e��
		CAnimate* GetAnimate();
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w���l���W���y��

		void SetCanMoving(bool);
		bool GetCanMoving();
		CLayer layer;

		bool GetValid();
		void SetValid(bool);

	protected:
		CAnimate animation;			// ���l���ʵe
		int height, width;			// ���l�����B�e
		int x, y;					// ���l���W���y��
		int score;

		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		bool canMoving;				// �O�_�i�H����

	private:
		int move_distance = MOVE_DISTANCE;
	};
	#pragma endregion

	#pragma region - CRole -
	class CRole : public CEraser
	{
		friend class CScallion;
	public:
		CRole();
		~CRole();
		void OnMove();
		void OnShow();
		bool GetMovingJump();
		void SetMovingJump(bool);
		bool GetCanJumping();
		void SetCanJumping(bool);
		void SetMouseXY(int, int);
		void AddScore(int);
		void Initialize(unsigned, unsigned);

		bool GetIsFire();
		void SetIsFire(bool);
		void Fire(int, int);			//�ǤJ�ƹ��y��

		bool GetDrop();
		int GetVelocity();

		vector<CScallion*>* GetScallion();
		

	protected:
		vector<CScallion*>	scallion;
		bool isJumping;				// �O�_���b���D
		bool canJumping;			// �O�_�i�H���D
		bool isFire;				// �O�_���b�g��
		int init_velocity;			// ���W����t��
		int velocity;				// �t��
		int gravity;				// ���O
		char last_right_left;
		int mouse_x, mouse_y;
		int move_distance = MOVE_DISTANCE; //�C�����ʪ��Z��
		CTimer shoot_cd;
	private:
		unsigned AUDIO_THROW;
		unsigned AUDIO_JUMP;

	};
	#pragma endregion

	#pragma region - CNPC -
	class CNPC : public CEraser
	{
		friend class CScallion;
	public:
		CNPC();
		CNPC(int, int, string, string, int, int);
		void SetXY(int, int);
		void SetScore(int);
		void SetMoving();
		void OnMove();
		//void Initialize();
		~CNPC();
	private:
		int initX;
		CTimer moveTimer;
		CTimer stopTimer;
		CTimer recreateTimer;
		int move_distance = 2;
	};
	#pragma endregion
}