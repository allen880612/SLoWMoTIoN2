#include "stdafx.h"
#include "Resource.h"
#include "Refactor.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <map>
#include "CManager.h"
using namespace myLibrary;

namespace game_framework
{
	#pragma region - mapManager -
	CMapManager::CMapManager()
	{
		InitializeCBlockMap();
		//Initialize();
	}


	CMapManager::~CMapManager()
	{
	}

	vector<CPasserby*>* CMapManager::GetPasserby()
	{
		return &passerbyManager.passerby;
	}

	void CMapManager::DeletePasserby(vector<CPasserby*>::iterator passerbyj)
	{
		passerbyManager.DeletePasserby(passerbyj);
	}

	void CMapManager::AddPasserby()
	{
		passerbyManager.passerby.push_back(passerbyManager.AddPasserby(blockMap[nowMap].passerbyID, background.Width()));
	}

	void CMapManager::Initialize()
	{
		nowMap = 2;
		loadMap = blockMap[nowMap].loadMap;
		x = 0;
		layer.SetLayer(0);
		background = blockMap[nowMap].backgroundBitmap;
		background.SetTopLeft(0, 0);
		CLayerManager::Instance()->AddObject(&background, layer.GetLayer());
		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width());
	}

	#pragma region - GetMap -

	int CMapManager::GetNowMap()
	{
		return nowMap;
	}

	int CMapManager::GetUpMap()
	{
		return blockMap[nowMap].upMap;
	}

	int CMapManager::GetDownMap()
	{
		return blockMap[nowMap].downMap;
	}

	int CMapManager::GetLeftMap()
	{
		return blockMap[nowMap].leftMap;
	}

	int CMapManager::GetRightMap()
	{
		return blockMap[nowMap].rightMap;
	}

	int CMapManager::GetLoadMap()
	{
		return loadMap;
	}

	int CMapManager::GetNpcNumber()
	{
		return passerbyManager.passerby.size();
	}

	int CMapManager::GetNpcLayer(int npcIndex)
	{
		return passerbyManager.passerby[npcIndex]->layer.GetLayer();
	}

	bool CMapManager::GetNpcValid(int npcIndex)
	{
		return passerbyManager.passerby[npcIndex]->GetValid();
	}

	CMovingBitmap* CMapManager::GetBitmap()
	{
		return &background;
	}

	CAnimate* CMapManager::GetNpc(int npcIndex)
	{
		return (passerbyManager.passerby[npcIndex]->GetAnimate());
	}

	int CMapManager::GetSplitLeft()
	{
		return 0 + (SIZE_X / 2);
	}

	int CMapManager::GetSplitRight()
	{
		return background.Width() - (SIZE_X / 2);
	}

	int CMapManager::GetX1()
	{
		return x;
	}

	int CMapManager::GetX2()
	{
		return x + (SIZE_X);
	}

	int CMapManager::GetBitmapWidth()
	{
		return background.Width();
	}
	#pragma endregion

	void CMapManager::ChangeMap(int changeMap, string nextMap)
	{
		nowMap = changeMap;
		loadMap = blockMap[nowMap].loadMap;
		background = blockMap[nowMap].backgroundBitmap;
		if (nextMap == "left") //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ����� (���ʨ쥪�䪺�a��)
		{
			x = init_x = -(GetBitmapWidth() - SIZE_X);
		}
		else if (nextMap == "right")  //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ��k�� (���ʨ�k�䪺�a��)
		{
			x = init_x =  0; 
		}
		background.SetTopLeft(x, 0);

		passerbyManager.Clear();
		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width());

		CCamera::Instance()->Reset();
	}

	void CMapManager::SetMovingLeft(bool _flag)
	{
		isMovingLeft = _flag;
	}

	void CMapManager::SetMovingRight(bool _flag)
	{
		isMovingRight = _flag;
	}

	void CMapManager::OnMove()
	{
		
		//if (isMovingLeft)
		//{
		//	x += directionX;
		//	//for(unsigned int npcIndex = 0; npcIndex < blockMap[nowMap].npc.size(); npcIndex++)
		//		//blockMap[nowMap].npc[npcIndex].SetXY(blockMap[nowMap].npc[npcIndex].GetX1() + directionX, blockMap[nowMap].npc[npcIndex].GetY1());
		//	for (unsigned int npcIndex = 0; npcIndex < passerbyManager.passerby.size(); npcIndex++)
		//		passerbyManager.passerby[npcIndex]->SetXY(passerbyManager.passerby[npcIndex]->GetX1() + directionX, passerbyManager.passerby[npcIndex]->GetY1());
		//}
		//if (isMovingRight)
		//{
		//	x -= directionX;
		//	for (unsigned int npcIndex = 0; npcIndex < passerbyManager.passerby.size(); npcIndex++)
		//		passerbyManager.passerby[npcIndex]->SetXY(passerbyManager.passerby[npcIndex]->GetX1() - directionX, passerbyManager.passerby[npcIndex]->GetY1());
		//}
		//SetXY(x - CCamera::Instance()->GetX(), 0);

		int dx = CCamera::Instance()->GetX();
		x = init_x - dx;
		SetXY(x, 0);
	}

	void CMapManager::SetXY(int _x, int _y)
	{
		x = _x;
		if (x <= -((background.Width()) - 640))
			x = -((background.Width()) - 640);
		if (x >= 0)
			x = 0;
		background.SetTopLeft(x, _y);
	}

	bool CMapManager::IsMapEnd()
	{
		return x < 0 || x > ((background.Width()) - 640);
	}

	void CMapManager::OnShow() //��ܹ����쪺blockMap�Ϥ� (nowMap = 1, ���blockMap[1]��background, ����)
	{
		background.SetTopLeft(0, 0);
		background.ShowBitmap();
	}

	void CMapManager::InitializeCBlockMap()
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++) //��l��blockMap���W�U���k�a�ϸ�T�A�W�[�iŪ�ʨϥ�switch�ԭz
		{
			switch (mapIndex)
			{				//���ǡG�ثe �W �U �� �k �a�ϤW���X��passerby�s�b�A -1��ܤ��s�b
			case 0:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, 5, "RES", "IDB_MAP", mapIndex);
				break;

			case 1:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, -1, 0, 2, "RES", "IDB_MAP", mapIndex);
				break;

			case 2:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 0, 3, 3, "RES", "IDB_MAP", mapIndex);
				break;

			case 3:
				blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 2, -1, 0, "RES", "IDB_MAP", mapIndex);
				break;

			default:
				blockMap[mapIndex] = CBlockMap(-1, -1, -1, -1, -1, 0, "RES", "IDB_MAP", 0);
				break;
			}
		}
	}

	void CMapManager::LoadMapBitmap() //�p�r���N��ALoadMapBitmap�A�bGameStateRun:OnInit�B��A�@����load blockMap���Ҧ��Ϥ�
	{
		for (int mapIndex = 0; mapIndex < MAX_MAP_NUMBER; mapIndex++)
		{
			char *address = ConvertCharPointToString(blockMap[mapIndex].ziliaojia, blockMap[mapIndex].name, blockMap[mapIndex].number);
			blockMap[mapIndex].backgroundBitmap.LoadBitmap(address);
			delete address;
		}
	}
	#pragma endregion

	#pragma region - layerManager -
	CLayerManager CLayerManager::layerManager;
	CLayerManager::CLayerManager()
	{
		Clear();
	}
	CLayerManager::~CLayerManager()
	{

	}
	void CLayerManager::Clear()
	{
		for (int i = 0; i < MAX_LAYER_NUMBER; i++)
		{
			layerBitmap[i].clear();
			layerAnimate[i].clear();
		}
	}

	void CLayerManager::AddObject(CMovingBitmap* object, int targetLayer)
	{
		layerBitmap[targetLayer].push_back(object);
	}

	void CLayerManager::AddObject(CAnimate* object, int targetLayer)
	{
		layerAnimate[targetLayer].push_back(object);
	}

	void CLayerManager::ShowLayer()
	{
		for (int i = 0; i < MAX_LAYER_NUMBER; i++)
		{
			for (vector<CMovingBitmap*>::iterator k = layerBitmap[i].begin(); k != layerBitmap[i].end(); k++)
			{
				if ((*k)->GetValid())
				{
					(*k)->ShowBitmap();
				}
			}

			for (vector<CAnimate*>::iterator k = layerAnimate[i].begin(); k != layerAnimate[i].end();)
			{
				if ((*k)->IsNull())
				{
					k = layerAnimate[i].erase(k);
				}
				else
				{
					if ((*k)->GetValid())	//if valid then show
					{
						(*k)->OnShow();
					}
					k++;
				}
			}
		}
	}
	CLayerManager * CLayerManager::Instance()
	{
		return &layerManager;
	}
	void CLayerManager::Initialize()
	{
		Clear();
	}
	#pragma endregion

	#pragma region - passerbyManager -
	CPasserbyManager::CPasserbyManager()
	{
		layer.SetLayer(4);
		passerby.clear();
	}
	CPasserbyManager::~CPasserbyManager()
	{
		Clear();
	}
	void CPasserbyManager::Clear()
	{
		for (vector<CPasserby*>::iterator it = passerby.begin(); it != passerby.end(); it++)
		{
			delete *it;
			(*it) = NULL;
		}
		vector<CPasserby*> del;
		passerby.swap(del);
		passerby.clear();
	}
	CPasserby* CPasserbyManager::AddPasserby(vector<int> id, int mapWidth)
	{
		#pragma region Create a Passerby
		int randomID = GetRandom(0, id.size() - 1); //random �M�w passerby���� (1��or2��)
		CPasserby *newPasserby = new CPasserby(0, 0, ziliaojia, name[id[randomID]], 2, (id[randomID] + 1) * 10); //���Ыؤ@��default passerby
		//newPasserby->LoadBitmap(ziliaojia, name[id[randomID]], 2, RGB(255, 255, 255)); //load passerby���Ϥ�
		int randomX = GetRandom(0, mapWidth - newPasserby->Width()); //random �M�wpasserby���X�{��m
		newPasserby->SetXY(randomX, 480 - newPasserby->Height()); //set passerby x, y
		return newPasserby;
		#pragma endregion
	}
	void CPasserbyManager::CreatePasserby(int createNumber, vector<int> id, int mapWidth)
	{
		for (int i = 0; i < createNumber; i++)
		{
			CPasserby* newPasserby = AddPasserby(id, mapWidth);
			newPasserby->SetValid(true);
			passerby.push_back(newPasserby);
		}
	}
	void CPasserbyManager::DeletePasserby(vector<CPasserby*>::iterator passerbyj)
	{
		delete *passerbyj;
		*passerbyj = NULL;
		passerbyj = passerby.erase(passerbyj);
	}
	vector<CPasserby*> CPasserbyManager::GetPasserby()
	{
		return passerby;
	}
	#pragma endregion

	#pragma region - DialogManager -
	CDialogManager CDialogManager::dialogManager;

	CDialogManager::CDialogManager()
	{
		IsBitmapLoaded = false;
		IsDialoging = false;
		IsPrintTips = false;
		nowDialog = NULL;
		showtext = "";
		txt.clear();
		nowShowTextSize = 0;
		AddShowTextTimer = CTimer(DIALOG_ADDTEXT_TIME);
	}

	CDialogManager::~CDialogManager()
	{
		
	}

	void CDialogManager::OnCycle()
	{
		#pragma region - add show text -
		if (showtext != "" && nowShowTextSize < (int)showtext.size())
		{
			AddShowTextTimer.CountDown();
			if (AddShowTextTimer.IsTimeOut()) //add show text
			{
				//doit
				if (showtext[nowShowTextSize] < 0) //�DASCII��t�����r
				{
					nowShowTextSize += 2;
				}
				else
				{
					nowShowTextSize++;
				}
				AddShowTextTimer.ResetTime();
				ShowText();
			}
		}
		#pragma endregion

		#pragma region - print finish tips (next) -
		if (showtext != "" && nowShowTextSize >= (int)showtext.size())
		{
			IsPrintTips = true;
		}
		#pragma endregion

	}

	void CDialogManager::Load_Image()
	{
		#pragma region - define avatar map -
		dialogAvatar[DIALOG_AVATAR_NAME_ROLE] = CMovingBitmap();
		dialogAvatar[DIALOG_AVATAR_NAME_XINGTING] = CMovingBitmap();
		#pragma endregion

		#pragma region - load image -

		char *address;
		#pragma region - load dialog background -
		address = ConvertCharPointToString("Dialog", "ground", 0);
		dialog_background.LoadBitmap(address, RGB(255, 255, 255));
		delete address;
		#pragma endregion

		#pragma region - load xingting - avatar -
		address = ConvertCharPointToString("Dialog", "xingting", 0);
		dialogAvatar[DIALOG_AVATAR_NAME_XINGTING].LoadBitmap(address);
		delete address;
		#pragma endregion

		#pragma region - load role avatar -
		address = ConvertCharPointToString("Dialog", "mikuAvatar", 0);
		dialogAvatar[DIALOG_AVATAR_NAME_ROLE].LoadBitmap(address);
		delete address;
		#pragma endregion

		#pragma region - load null -
		address = ConvertCharPointToString("Dialog", "bmp", 0);
		avatar_null.LoadBitmap(address);
		delete address;
		#pragma endregion

		#pragma region - load next animation -
		textNext.LoadBitmap("Dialog\\next", "next", 4, DIALOG_BACKGROUND_COLOR);
		textNext.SetTopLeft(500, 450);
		#pragma endregion

		#pragma endregion

		IsBitmapLoaded = true;
	}

	void CDialogManager::LoadDialog()
	{
		dialogmap.clear();
		dialogmap[RoleVSBoss] = CDialog("Txt\\RoleVsBoss.txt", RoleVSBoss, false);
	}

	void CDialogManager::ShowText_Next()
	{
		if (IsPrintTips)
		{
			//PaintText("Next", 550, 450, "�L�n������", 14, RGB(0, 0, 0), DIALOG_BACKGROUND_COLOR);
			textNext.SetValid(true);
			textNext.OnMove();
		}
		else
		{
			textNext.SetValid(false);
		}
	}

	void CDialogManager::Initialize()
	{
		if (IsBitmapLoaded == false)
		{
			Load_Image();
		}
		#pragma region - reload - dialog -
		LoadDialog();
		#pragma endregion

		avatar_null.SetValid(false);
		dialog_background.SetValid(false);
		textNext.SetValid(false);
		avatar = avatar_null;

		#pragma region - Init - Image Point -
		dialog_background.SetTopLeft(0, SIZE_Y - dialog_background.Height()); //reset dbg's xy
		for (map<string, CMovingBitmap>::iterator dbmp = dialogAvatar.begin(); dbmp != dialogAvatar.end(); dbmp++)
		{
			dbmp->second.SetTopLeft(dialog_background.Left() + MARGIN_DIALOG_AVATAR, dialog_background.Top() + MARGIN_DIALOG_AVATAR);
		}
		#pragma endregion

		backgroundLayer.SetLayer(8);
		avatarLayer.SetLayer(backgroundLayer.GetLayer() + 1);

		CLayerManager::Instance()->AddObject(&dialog_background, backgroundLayer.GetLayer());
		CLayerManager::Instance()->AddObject(&avatar, avatarLayer.GetLayer());
		CLayerManager::Instance()->AddObject(&textNext, avatarLayer.GetLayer());

		step = 0;
		nowShowTextSize = 0;
		IsPrintTips = false;
		AddShowTextTimer.ResetTime(DIALOG_ADDTEXT_TIME);
	}

	CDialogManager * CDialogManager::Instance()
	{
		return &dialogManager;
	}

	void CDialogManager::Start(string mode)
	{
		if (DebugMode)
			return;

		nowDialog = &dialogmap[mode];
		if (nowDialog->GetTriggered()) //�QĲ�o�L �^�h
		{
			nowDialog = NULL;
			return;
		}
		IsDialoging = true;
		IsPrintTips = false;
		dialog_background.SetValid(true);
		Dialog();
	}

	void CDialogManager::Next()
	{
		if(IsDialoging)
		{
			if (nowShowTextSize < (int)showtext.size()) //��r�٨S��ܧ����ɭԡA�N�w�g�I���e�� > ������ܧ�������r
			{
				nowShowTextSize = (int)showtext.size();
			}
			else //�r�w�g��ܧ��F�A�i�H����
			{
				step++;
				IsPrintTips = false;
				nowShowTextSize = 0;
				Dialog();
			}
		}	
	}

	void CDialogManager::Stop()
	{
		avatar = avatar_null;
		dialog_background.SetValid(false);
		textNext.SetValid(false);
		IsDialoging = false;
		nowDialog->SetTriggered();
		nowDialog = NULL;
		nowShowTextSize = 0;
	}

	void CDialogManager::ShowText()
	{
		if (showtext != "")
		{
			#pragma region - split showtext -
			vector<char*> split_showtext;
			int charindex = 0;
			int vectorindex = 0;
			char tempk[DIALOG_MAX_TEXT + 5];
			memset(tempk, '\0', sizeof(tempk));
			
			for (int i = 0; i < nowShowTextSize; i++)
			{
				if (charindex < DIALOG_MAX_TEXT)
				{
					//�N�r���[�Jtempk, ��r���W�L�@�w�q���ɭԵ�split-showtext(����)
					if (showtext[i] != '_')
						tempk[charindex++] = showtext[i];
					else  //�ҥ~�B�z�A�N���u�����ť� (�奻���ॴ�ť�)
						tempk[charindex++] = ' ';
					
					#pragma region -- �w��˼ƲĤG�Ӧr���O�^��A���̫�@�Ӧr�O���媺�B�~�]�w --
					#pragma region --- ���� & ���� ---
					/*
					1. �ثe��r�O�^��
					2. �U�@�Ӧr�O����
					3. �U�@�Ӧr�b29
					�����G���D�o�ͪ���]�O�]���^�媺size�O1�A������O2�A�ӧڳ]�w��size��30�N�|����
					�]���p�Gsize=28���ɭԸˤ@�ӭ^��r(�ˤ@�ӭ^��r��size=29)
					���U�@�Ӧr�O���媺�ɭԡA�Ӳz���n�˨��size�~�|���T�A���]���u��ˤ@�ӡA�t�@�ӷ|��U�@��~��
					���ɤ���r���|���`��ܡA�|�ܦ��ýX
					�ѨM�覡�G�ŦX�W�z����N����������
					*/
					#pragma endregion
					if (showtext[i] > 0 && showtext[i + 1] < 0 && charindex + 1 >= DIALOG_MAX_TEXT)
					{
						charindex += 2;
					}
					#pragma endregion
				}
				if (charindex >= DIALOG_MAX_TEXT)
				{
					char *tteemmppkkk = new char[DIALOG_MAX_TEXT + 5];
					strcpy(tteemmppkkk, tempk);
					split_showtext.push_back(tteemmppkkk);

					charindex = 0;
					memset(tempk, '\0', sizeof(tempk));
				}
			}
			char *tteemmppkkk = new char[DIALOG_MAX_TEXT + 5];
			strcpy(tteemmppkkk, tempk);
			split_showtext.push_back(tteemmppkkk);
			#pragma endregion

			//�U�����å���եΪ�
			//PaintText(split_showtext[0], 0 + 20, 100, "�L�n������", DIALOG_TEXT_SIZE, RGB(0, 0, 0), RGB(0, 255, 0));

			#pragma region - draw text -
			//char* showpointer;
			//showpointer = new char[showtext.length() + 1];
			//strcpy(showpointer, showtext.c_str());
			////x = 160, endx = 480
			//PaintText(showpointer, avatar.Left() + avatar.Width() + MARGIN_DIALOG_TEXT_X, avatar.Top() + MARGIN_DIALOG_TEXT_Y, "�L�n������", DIALOG_TEXT_SIZE, RGB(0, 0, 0), RGB(232, 232, 232));
			//delete showpointer;
			for (unsigned int i = 0; i < split_showtext.size(); i++)
			{
				PaintText(split_showtext[i], avatar.Left() + avatar.Width() + MARGIN_DIALOG_TEXT_X, avatar.Top() + MARGIN_DIALOG_TEXT_Y + i * 35, "�L�n������", DIALOG_TEXT_SIZE, DIALOG_TEXT_COLOR, DIALOG_BACKGROUND_COLOR);
				delete split_showtext[i];
			}
			#pragma endregion

			#pragma region - draw finish tips text -
			ShowText_Next();
			#pragma endregion

		}
	}

	bool CDialogManager::GetDialogState()
	{
		return IsDialoging;
	}

	void CDialogManager::Dialog()
	{
		if (step < nowDialog->GetTxtSize())
		{
			avatar = dialogAvatar[nowDialog->GetAvatar(step)];
			showtext = nowDialog->GetDialogTxt(step);
		}
		else
		{
			step = 0;
			showtext = "";
			Stop();
		}
		//if (nowDialog->GetMode() == RoleVSBoss) //���O�d==
		//{
			//if (step == 0)
			//{
			//	avatar = avatar_xingting;
			//	//showtext = "�ڬO�d���@, I'm a Fucking Teacher�AAHHH~~~~FSDFD##@!!!";
			//	//showtext = "�ڬO�d���@�ڬO�d���@�ڬO�d���@�ڬO�d���@";
			//	//showtext = "WWWWWWWWWWWWWWWWWWWWWWWWWWWWW";
			//}
			//else if (step == 1)
			//{
			//	avatar = avatar_role;
			//	showtext = "Bhhh~~~";
			//}
			//else if (step == 2)
			//{
			//	avatar = avatar_xingting;
			//	showtext = "Chhh~~~";
			//}
			//else
			//{
			//	step = 0;
			//	showtext = "";
			//	Stop();
			//}
		//}

	}
	#pragma endregion

	#pragma region - BossManager -
	CBossManager::CBossManager()
	{
		#pragma region -- Create Boss --
		bossInformation[BOSS_XINGTING] = new CXingting(450, 250, 8787, BitmapPath("Boss", "xingting", 2), RGB(255, 255, 255));
		#pragma endregion
		targetBoss = NULL;
	}

	CBossManager::~CBossManager()
	{
		Clear();
	}

	void CBossManager::Initialize() //CGameRun::OnBeginState�� initialize
	{
		//(second(value)�����СA�ҥHsecond�᭱��-> )
		//�u���A��C plus plus
		for (map<string, CBoss*>::iterator bossiter = bossInformation.begin(); bossiter != bossInformation.end(); bossiter++)
		{
			bossiter->second->Initialize(); //��l��boss 
			bossiter->second->GetAnimate()->SetValid(false); //�N�Ҧ��Ϥ����]��false
			CLayerManager::Instance()->AddObject(bossiter->second->GetAnimate(), bossiter->second->layer.GetLayer());
		}
		targetBoss = NULL;
	}

	void CBossManager::Clear()
	{
		//�Ѻc
		for (map<string, CBoss*>::iterator bossiter = bossInformation.begin(); bossiter != bossInformation.end(); bossiter++)
		{
			delete bossiter->second;
		}

		bossInformation.clear();
		targetBoss = NULL;
	}

	void CBossManager::TargetBoss(int nowMap)
	{
		if (nowMap == 3)
		{
			targetBoss = bossInformation[BOSS_XINGTING];
			targetBoss->GetAnimate()->SetValid(true);
		}
		else
		{
			if (targetBoss != NULL)
			{
				targetBoss->GetAnimate()->SetValid(false);
				targetBoss = NULL;
			}
		}
	}
	#pragma endregion
	
}
