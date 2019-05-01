#pragma once
#include "CBoss.h"
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
		int  GetY3();                   // �����I Y �y��
		int GetScore();
		int  Height();
		int  Width();
		bool GetMovingLeft();           // ���O�_�b������
		bool GetMovingRight();          // ���O�_�A���k��
		void Initialize();				// �]�w���l����l��
		void LoadBitmap();				// ���J�ϧ�
		// �Ѹ��|���J�ϧ�(��Ƶ�, name, �i��)
		void LoadBitmap(string, string, int, COLORREF);
		void LoadBitmap(BitmapPath);
		
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

	#pragma region - CNPC -
	class CNPC : public CEraser
	{
	public:
		CNPC();
		CNPC(CPoint, BitmapPath, string);
		~CNPC();
		void Initialize();
		void SetCurrentXY(int, int);
		void SetXY();
		void OnMove();
		virtual void RoleCollision() {}; //�I�����n�����Ʊ�

	protected:
	#pragma region - init information -
		CPoint initPoint;
		BitmapPath initLoadPath;
	#pragma endregion

		int currentX, currentY;
		string id;
	};
	#pragma endregion

	#pragma region - CRole -
	class CBlackHole;
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
		void SubHp();
		int  GetHp() { return hp; }; //���i�{
		//void LoadBitmap();

		void Initialize();

		bool GetIsFire();
		void SetIsFire(bool);
		void Fire(int, int);			//�ǤJ�ƹ��y��
		
		bool GetDrop();
		int GetVelocity();

		#pragma region - Collision -
		bool IsCollisionBoss(CBoss*);
		bool IsCollisionLevel4(CScallion*);

		bool IsCollisionNPC(CNPC*);

		bool IsCollisionBlackHole(CBlackHole*);
		bool IsCollisionBlackHoleCenter(CBlackHole*);
		#pragma endregion

		vector<CScallion*>* GetScallion();
		

		void SetCatched(bool flag) { isCatched = flag; };
		bool IsCatched() { return isCatched; };

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

		CMovingBitmap decisionPoint;
		CRect collisionRect;
		void ResetCollisionRect();
	private:
		//unsigned AUDIO_THROW;\
		//unsigned AUDIO_JUMP;
		CAction  action;
		CInteger hp_left;
		CInteger scoreInteger;

		#pragma region - init information-
		int inithp = 20;
		bool isLoaded;
		#pragma endregion

		int hp;
		
		bool isCatched;

	};
	#pragma endregion

	#pragma region - CPasserby -
	class CPasserby : public CEraser
	{
		friend class CScallion;
	public:
		CPasserby();
		CPasserby(int, int, string, string, int, int);
		void SetXY(int, int);
		void SetScore(int);
		void SetMoving();
		void OnMove();
		//void Initialize();
		~CPasserby();
	private:
		int initX;
		CTimer moveTimer;
		CTimer stopTimer;
		CTimer recreateTimer;
		int move_distance = 2;
	};
	#pragma endregion

	#pragma region - CNPC - No.1 -
	//CNPC1 > ���q��NPC (���NPC)
	class CNPC1 : public CNPC
	{
	public:
		CNPC1();
		CNPC1(CPoint, BitmapPath, string, string);
		~CNPC1();
		void RoleCollision();
	private:
		string txt;
	};
	#pragma endregion

	#pragma region - CNPC - No.2 -

	#pragma endregion


	#pragma region - CNPC - No.3 -
	//CNPC3 > ���ּ���NPC
	class CNPC3 : public CNPC
	{
	public:
		CNPC3();
		CNPC3(CPoint, BitmapPath, string, string, string);
		~CNPC3();
		void RoleCollision();
	private:
		string openTxt;
		string openMusic;
	};
	#pragma endregion

}