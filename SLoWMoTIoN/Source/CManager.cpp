#include "stdafx.h"
#include "Resource.h"
#include "Refactor.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <map>
#include <sstream>
#include <fstream>
#include "mygame.h"
#include "CManager.h"
using namespace myLibrary;

namespace game_framework
{
	#pragma region - mapManager -
	CMapManager::CMapManager()
	{
		max_map_number = getFolerFileNumber("RES\\Map\\Information\\");
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
		CPasserby *newPasserby = passerbyManager.AddPasserby(blockMap[nowMap].passerbyID, background.Width());
		newPasserby->SetBlock(GetBlockVector());
		passerbyManager.passerby.push_back(newPasserby);
	}

	void CMapManager::Initialize()
	{
		ReloadBlockMap(); //�C��init���ˬd�O�_�n���s���Jblockmap

		nowMap = NOW_MAP;
		loadMap = blockMap[nowMap].loadMap;
		x = 0;
		layer.SetLayer(0);
		background = blockMap[nowMap].backgroundBitmap;
		background.SetTopLeft(0, 0);
		CLayerManager::Instance()->AddObject(&background, layer.GetLayer());

		#pragma region - �]�mblock -
		for (int i = 0 ; i < max_map_number; i++)
		{
			for (vector<CBlock>::iterator bkiter = blockMap[i].block.begin(); bkiter != blockMap[i].block.end(); bkiter++)
			{
				bkiter->bmp.SetValid(false | (i == nowMap));
				bkiter->SetXY(bkiter->x, bkiter->y);
				CLayerManager::Instance()->AddObject(&(bkiter->bmp), 7);
			}
		}
		#pragma endregion

		#pragma region - �]�mDoor -
		for (int i = 0; i < max_map_number; i++)
		{
			for (vector<CDoor>::iterator bkiter = blockMap[i].door.begin(); bkiter != blockMap[i].door.end(); bkiter++)
			{
				bkiter->bmp.SetValid(false | (i == nowMap));
				bkiter->SetXY(bkiter->x, bkiter->y);
				CLayerManager::Instance()->AddObject(&(bkiter->bmp), 7);
			}
		}
		#pragma endregion

		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width(), GetBlockVector());
	}

	void CMapManager::ReloadBlockMap()
	{
		bool isreloadDataExist = false; //�����]�ɮפ��s�b

		fstream reloadData;
		reloadData.open("RES\\Map\\ReloadMapInformation.txt");
		#pragma region - �ɮצs�b - reload -
		if (reloadData.is_open()) //�ɮצs�b
		{
			isreloadDataExist = true;
			string line; //temp string

			#pragma region -- �ˬd�O�_���s�W���a�� --
			reloadData >> line; // load first string in reloadData
			int nowMapNumber = ConvertStringToInt(line); //�{�bMap - Information�����a���`��
			int nowBlockSize = blockMap.size();
			if (nowBlockSize < nowMapNumber) //blockMap��size �� �ثe���a���`�Ƥp > �s�W�a��
			{
				max_map_number = nowMapNumber;
				for (int mapIndex = nowBlockSize; mapIndex < nowMapNumber; mapIndex++)
				{
					blockMap.push_back(CBlockMap(mapIndex));
				}
			}
			#pragma endregion

			#pragma region -- ���s���J�s��L���a�� --
			while (reloadData >> line)
			{
				if (line != "")
				{
					int reloadMapIndex = ConvertStringToInt(line); //���o�n���s���J���a�Ͻs��
					if (reloadMapIndex < nowBlockSize) //small small ���b
					{
						blockMap[reloadMapIndex] = CBlockMap(reloadMapIndex);
					}
				}
			}
			#pragma endregion

			#pragma region -- �s�W&���s���J���� - ���J�s���Ϥ� --
			LoadMapBitmap();
			#pragma endregion

		}
		#pragma endregion
		reloadData.close();

		#pragma region - �ɮצs�b - �R�� -
		if (isreloadDataExist)
		{
			DeleteFile("RES\\Map\\ReloadMapInformation.txt");
		}
		#pragma endregion

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

	vector<CBlock>* CMapManager::GetBlockVector()
	{
		return &blockMap[nowMap].block;
	}

	vector<CDoor>* CMapManager::GetDoorVector()
	{
		return &blockMap[nowMap].door;
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
		#pragma region - �]�mblock -
		//�����쥻��
		for (vector<CBlock>::iterator bkiter = blockMap[nowMap].block.begin(); bkiter != blockMap[nowMap].block.end(); bkiter++)
		{
			bkiter->bmp.SetValid(false);
		}
		//���}��Ӫ�
		for (vector<CBlock>::iterator bkiter = blockMap[changeMap].block.begin(); bkiter != blockMap[changeMap].block.end(); bkiter++)
		{
			bkiter->bmp.SetValid(true);
		}
		#pragma endregion

		#pragma region - �]�mDoor -
		//�����쥻��
		for (vector<CDoor>::iterator bkiter = blockMap[nowMap].door.begin(); bkiter != blockMap[nowMap].door.end(); bkiter++)
		{
			bkiter->bmp.SetValid(false);
		}
		//���}��Ӫ�
		for (vector<CDoor>::iterator bkiter = blockMap[changeMap].door.begin(); bkiter != blockMap[changeMap].door.end(); bkiter++)
		{
			bkiter->bmp.SetValid(true);
		}
		#pragma endregion

		nowMap = changeMap;
		loadMap = blockMap[nowMap].loadMap;
		background = blockMap[nowMap].backgroundBitmap;
		CCamera::Instance()->Reset();
		if (nextMap == "left") //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ����� (���ʨ쥪�䪺�a��)
		{
			//x = init_x = -(GetBitmapWidth() - SIZE_X);
			CCamera::Instance()->SetXY((GetBitmapWidth() - SIZE_X), 0);
		}
		else if (nextMap == "right")  //�U�@�i�a��(�n�����a��)�A�b�쥻�a�Ϫ��k�� (���ʨ�k�䪺�a��)
		{
			//x = init_x =  0;
			CCamera::Instance()->SetXY(0, 0);
		}
		background.SetTopLeft(x, 0);

		passerbyManager.Clear();
		passerbyManager.CreatePasserby(blockMap[nowMap].passerbyMaxSize, blockMap[nowMap].passerbyID, blockMap[nowMap].backgroundBitmap.Width(), GetBlockVector());

		x = CCamera::Instance()->GetX();
		
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

		#pragma region - �ù��W�� background �n���� -
		int dx = CCamera::Instance()->GetX();
		x = -dx;
		SetXY(x, 0);
		#pragma endregion

		#pragma region - �ù��W�� block �]�n���� -
		for (vector<CBlock>::iterator bkiter = blockMap[nowMap].block.begin(); bkiter != blockMap[nowMap].block.end(); bkiter++)
		{
			bkiter->SetXY(bkiter->x, bkiter->y, dx);
		}
		#pragma endregion

		#pragma region - �ù��W�� door �]�n���� -
		for (vector<CDoor>::iterator bkiter = blockMap[nowMap].door.begin(); bkiter != blockMap[nowMap].door.end(); bkiter++)
		{
			bkiter->SetXY(bkiter->x, bkiter->y, dx);
		}
		#pragma endregion

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
		for (int mapIndex = 0; mapIndex < max_map_number; mapIndex++) //��l��blockMap���W�U���k�a�ϸ�T�A�W�[�iŪ�ʨϥ�switch�ԭz
		{
			//switch (mapIndex)
			//{				//���ǡG�ثe �W �U �� �k �a�ϤW���X��passerby�s�b�A -1��ܤ��s�b
			//case 0:
			//	blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 1, 2, 5, "RES\\Map", "IDB_MAP", mapIndex);
			//	break;

			//case 1:
			//	blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 4, 0, 2, "RES\\Map", "IDB_MAP", mapIndex);
			//	break;

			//case 2:
			//	blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 0, 3, 3, "RES\\Map", "IDB_MAP", mapIndex);
			//	break;

			//case 3:
			//	blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 2, 5, 0, "RES\\Map", "IDB_MAP", mapIndex);
			//	break;

			//case 4:
			//	blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, -1, 1, 0, "RES\\Map", "IDB_MAP", mapIndex);
			//	break;

			//case 5:
			//	blockMap[mapIndex] = CBlockMap(mapIndex, -1, -1, 3, -1, 0, "RES\\Map", "IDB_MAP", mapIndex);
			//	break;

			//default:
			//	blockMap[mapIndex] = CBlockMap(-1, -1, -1, -1, -1, 0, "RES\\Map", "IDB_MAP", 0);
			//	break;
			//}
			blockMap.push_back(CBlockMap(mapIndex));
		}
	}

	void CMapManager::LoadMapBitmap() //�p�r���N��ALoadMapBitmap�A�bGameStateRun:OnInit�B��A�@����load blockMap���Ҧ��Ϥ�
	{
		for (int mapIndex = 0; mapIndex < max_map_number; mapIndex++)
		{	
			//char *address = ConvertCharPointToString(blockMap[mapIndex].ziliaojia, blockMap[mapIndex].name, blockMap[mapIndex].number);
			/*char *address = ConvertCharPointToString(blockMap[mapIndex].loadPath);
			blockMap[mapIndex].backgroundBitmap.LoadBitmap(address);
			delete address;*/
			if (!blockMap[mapIndex].isLoad)
			{
				blockMap[mapIndex].LoadImg();
				blockMap[mapIndex].isLoad = true;
			}
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
			layerAction[i].clear();
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

	void CLayerManager::AddObject(CAction* object, int targetLayer)
	{
		layerAction[targetLayer].push_back(object);
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

			for (vector<CAction*>::iterator k = layerAction[i].begin(); k != layerAction[i].end();)
			{
				if ((*k)->IsNull())
				{
					k = layerAction[i].erase(k);
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
	CPasserby * CPasserbyManager::GetPasserbyType(int randomID)
	{
		if (randomID == 0)
		{
			return new CPasserbyType1(0, 0, BitmapPath("RES\\Role\\NPC\\LUKA", "LUKA", 2, RGB(214, 214, 214)), 10); //���Ыؤ@��default passerby
		}
		else if (randomID == 1)
		{
			return new CPasserbyType1(0, 0, BitmapPath("RES\\Role\\NPC\\RIN", "RIN", 2, RGB(214, 214, 214)), 15); //���Ыؤ@��default passerby
		}
		else if (randomID == 2)
		{
			return new CPasserbyType2(0, 0, BitmapPath("RES\\Role\\NPC\\mushroom", "mushroom", 5, RGB(214, 214, 214)), 20); //���Ыؤ@��default passerby
		}
		return new CPasserbyType1(0, 0, BitmapPath("RES\\Role\\NPC\\LUKA", "LUKA", 2, RGB(214, 214, 214)), 10); //���Ыؤ@��default passerby
	}
	CPasserby* CPasserbyManager::AddPasserby(vector<int> id, int mapWidth)
	{
		#pragma region Create a Passerby
		int randomID = GetRandom(0, id.size() - 1); //random �M�w passerby���� (1��or2��)
		//CPasserby *newPasserby = new CPasserby(0, 0, ziliaojia + name[id[randomID]], name[id[randomID]], getFolerFileNumber(ziliaojia + name[id[randomID]]), (id[randomID] + 1) * 10); //���Ыؤ@��default passerby
		CPasserby *newPasserby = GetPasserbyType(id[randomID]);
		int randomX = GetRandom(0, mapWidth - newPasserby->Width()); //random �M�wpasserby���X�{��m
		newPasserby->SetXY(randomX, -newPasserby->Height()); //set passerby x, y
		return newPasserby;
		#pragma endregion
	}
	void CPasserbyManager::CreatePasserby(int createNumber, vector<int> id, int mapWidth, vector<CBlock> *bkvector)
	{
		for (int i = 0; i < createNumber; i++)
		{
			CPasserby* newPasserby = AddPasserby(id, mapWidth);
			newPasserby->SetBlock(bkvector);
			newPasserby->SetValid(false);
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

	#pragma region - CDialogManager -
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
		dialogmap.clear();
		nowDialog = NULL;
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
		#pragma region - Load avatar -
		vector<string> avatarName;
		string avatarFolderPath = "RES\\Dialog\\Avatar\\";
		getFolderFile(avatarFolderPath, &avatarName);
		
		for (unsigned i = 0; i < avatarName.size(); i++)
		{
			int strLength = avatarName[i].length();
			string ext = avatarName[i].substr(strLength - 4, strLength);
			if (ext == ".bmp")
			{
				string avatar = getFileName(avatarName[i]);
				dialogAvatar[avatar] = CMovingBitmap();
				char *address = ConvertCharPointToString((avatarFolderPath + avatarName[i]));
				dialogAvatar[avatar].LoadBitmap(address, RGB(214, 214, 214));
				delete address;
			}
		}
		#pragma endregion

		#pragma region - load image -

		
		#pragma region - load dialog background -
		dialog_background.LoadBitmap("RES\\Dialog", "ground", RGB(255, 255, 255));
		#pragma endregion
		#pragma region - load null -
		avatar_null.LoadBitmap("RES\\Dialog\\Avatar", "null");
		#pragma endregion

		#pragma region - load next animation -
		textNext.LoadBitmap("RES\\Dialog\\next", "next", 4, DIALOG_BACKGROUND_COLOR);
		textNext.SetTopLeft(500, 450);
		#pragma endregion

		#pragma endregion

		IsBitmapLoaded = true;
	}

	void CDialogManager::LoadDialog()
	{
		//dialogmap.clear();

		vector<string> dialogTxt;
		string dialogFolderPath = "RES\\Dialog\\Txt\\";
		getFolderFile(dialogFolderPath, &dialogTxt);

		for (unsigned int i = 0; i < dialogTxt.size(); i++)
		{
			string dialogFileName = getFileName(dialogTxt[i]);
			dialogmap[dialogFileName] = CDialog(dialogFolderPath + dialogTxt[i], dialogFileName);
		}

		IsDialogLoad = true;
	}


	void CDialogManager::LoadDialog(string dialogName, string loadPath)
	{

		dialogmap[dialogName] = CDialog(loadPath, dialogName);
		/*bool flag = dialogmap[dialogName].GetTriggered();

		flag = !flag;*/
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

	void CDialogManager::DialogWithSound()
	{
		if (nowTxtName == DIALOG_DATA_FAQAI)
		{
			if (step == 3 || step == 5)
			{
				CAudio::Instance()->Play("faqai");
			}
		}
	}

	void CDialogManager::Initialize()
	{
		if (IsBitmapLoaded == false)
		{
			Load_Image();
			#pragma region - Init - Image Point -
			dialog_background.SetTopLeft(0, SIZE_Y - dialog_background.Height()); //reset dbg's xy
			for (map<string, CMovingBitmap>::iterator dbmp = dialogAvatar.begin(); dbmp != dialogAvatar.end(); dbmp++)
			{
				dbmp->second.SetTopLeft(dialog_background.Left() + MARGIN_DIALOG_AVATAR, dialog_background.Top() + MARGIN_DIALOG_AVATAR);
			}
			#pragma endregion
		}

		#pragma region - reload - dialog -
		if (IsDialogLoad == false)
		{
			LoadDialog();
			IsDialogLoad = true;
		}
		else
		{
			for (map<string, CDialog>::iterator dialogiter = dialogmap.begin(); dialogiter != dialogmap.end(); dialogiter++)
			{
				dialogiter->second.Initialize();
			}
		}
		#pragma endregion

		avatar_null.SetValid(false);
		dialog_background.SetValid(false);
		textNext.SetValid(false);
		avatar = avatar_null;

		backgroundLayer.SetLayer(8);
		avatarLayer.SetLayer(backgroundLayer.GetLayer() + 1);

		CLayerManager::Instance()->AddObject(&dialog_background, backgroundLayer.GetLayer());
		CLayerManager::Instance()->AddObject(&avatar, avatarLayer.GetLayer());
		CLayerManager::Instance()->AddObject(&textNext, avatarLayer.GetLayer());

		step = 0;
		nowShowTextSize = 0;
		IsPrintTips = false;
		musicFromNPC = "";
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
		nowTxtName = mode;
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
		IsPrintTips = false;
		nowDialog->SetTriggered();
		nowDialog = NULL;
		nowShowTextSize = 0;
		step = 0;
		nowTxtName = "";
		if (musicFromNPC != "")
		{
			CAudio::Instance()->Stop(musicFromNPC);
			CAudio::Instance()->Play("SLoWMoTIoN_Game");
			musicFromNPC = "";
		}
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
			
			for (int i = 0; i < nowShowTextSize;)
			{
				if (charindex < DIALOG_MAX_TEXT)
				{
					//�N�r���[�Jtempk, ��r���W�L�@�w�q���ɭԵ�split-showtext(����)
					if (showtext[i] == '_') //�ҥ~�B�z�A�N���u�����ť� (�奻���ॴ�ť�)
					{
						tempk[charindex++] = ' ';
						i++;
					}
					else  if (showtext[i] == '@') //�Nat ��������
					{
						charindex += 999;
						i++;
					}
					else 
					{
						if (showtext[i] > 0)
						{
							tempk[charindex++] = showtext[i++];
						}
						else
						{
							if (charindex + 1 < DIALOG_MAX_TEXT) //add a chinese
							{
								tempk[charindex++] = showtext[i++];
								tempk[charindex++] = showtext[i++];
							}
							else
							{
								charindex += 999;
							}
						}
					}
						
					
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
						charindex += 999;
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

			#pragma region - Set Print Text Color -
			COLORREF txtColor = DIALOG_TEXT_COLOR;
			if (nowDialog != NULL)
			{
				txtColor = nowDialog->GetDialogColor(step);
			}
			#pragma endregion
			#pragma region - draw text -
			for (unsigned int i = 0; i < split_showtext.size(); i++)
			{
				PaintText(split_showtext[i], avatar.Left() + avatar.Width() + MARGIN_DIALOG_TEXT_X, avatar.Top() + MARGIN_DIALOG_TEXT_Y + i * 35, "�L�n������", DIALOG_TEXT_SIZE, txtColor, DIALOG_BACKGROUND_COLOR);
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
			DialogWithSound();
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
		bossInformation[BOSS_XINGTING] = new CXingting(450, 250, 8787, "Xingting", BitmapPath("RES\\Boss", "xingting", 2, RGB(214, 214, 214)));
		#pragma endregion
		targetBoss = NULL;
		isBattle = false;
	}

	CBossManager::~CBossManager()
	{
		if (targetBoss != NULL)
		{
			targetBoss->Clear();
		}
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
		isBattle = false;
	}

	void CBossManager::Clear()
	{
		//�Ѻc
		for (map<string, CBoss*>::iterator bossiter = bossInformation.begin(); bossiter != bossInformation.end(); bossiter++)
		{
			delete bossiter->second;
			bossiter->second = NULL;
		}

		bossInformation.clear();
		targetBoss = NULL;
	}

	void CBossManager::TargetBoss(int nowMap)
	{
		if (nowMap == BOSS_MAP_XINGTING && bossInformation[BOSS_XINGTING]->GetAlive())
		{
			targetBoss = bossInformation[BOSS_XINGTING];
			targetBoss->GetAnimate()->SetValid(true);
		}
		else
		{
			if (targetBoss != NULL)
			{
				targetBoss->GetAnimate()->SetValid(false);
				targetBoss->Clear();
				isBattle = false;
				targetBoss = NULL;
			}
		}
	}

	void CBossManager::BossDead()
	{
		targetBoss->Clear();
		targetBoss = NULL;
		SetBattle(false);
	}
	#pragma endregion

	#pragma region - CNPCManager -
	CNPCManager::CNPCManager()
	{
		Clear();
		LoadNPC();
	}	

	CNPCManager::~CNPCManager()
	{
		Clear();
	}
	void CNPCManager::Clear()
	{
		for (int i = 0; i < 99; i++)
		{
			for (vector<CNPC*>::iterator npciter = npc[i].begin(); npciter != npc[i].end(); ++npciter)
			{
				delete (*npciter);
				(*npciter) = NULL;
			}
			npc[i].clear();
		}
	}

	void CNPCManager::Initialize(int nowMap)
	{
		map_max_Number = getFolerFileNumber("RES\\Map\\Information\\");
		for (int i = 0; i < 99; i++)
		{
			for (vector<CNPC*>::iterator npciter = npc[i].begin(); npciter != npc[i].end(); npciter++)
			{
				(*npciter)->Initialize();
			}
		}
		SetNPCValid(nowMap, true);
	}

	void CNPCManager::ChangeMap(int nowMap, int nextMap)
	{
		SetNPCValid(nowMap, false);
		SetNPCValid(nextMap, true);
	}

	void CNPCManager::SetNPCValid(int thisMap, bool flag)
	{
		for (vector<CNPC*>::iterator npciter = npc[thisMap].begin(); npciter != npc[thisMap].end(); npciter++)
		{
			(*npciter)->SetValid(flag);
		}
	}

	void CNPCManager::OnCycle(int thisMap, CPoint rolePoint)
	{
		for (vector<CNPC*>::iterator npciter = npc[thisMap].begin(); npciter != npc[thisMap].end(); npciter++)
		{
			(*npciter)->OnCycle(rolePoint);
		}
	}

	void CNPCManager::LoadNPC()
	{
		npc[4].push_back(new CNPC1(CPoint(50, 388), BitmapPath("RES\\NPC\\test", "test", 1, RGB(255, 255, 255)), "frog", FROG));
		npc[4].push_back(new CNPC3(CPoint(450, 388), BitmapPath("RES\\NPC\\test2", "test2", 1, RGB(255, 255, 255)), "deadlock", "MyVoiceIsDead", MyVoiceIsDead));


		npc[3].push_back(new CNPC1(CPoint(50, 350), BitmapPath("RES\\NPC\\studentB", "studentB", 1, RGB(255, 255, 255)), DIALOG_AVATAR_NAME_STUDENTB, DIALOG_DATA_MEETSTB));
		npc[3].push_back(new CNPC1(CPoint(450, 350), BitmapPath("RES\\NPC\\studentG", "studentG", 1, RGB(255, 255, 255)), DIALOG_AVATAR_NAME_STUDENTG, DIALOG_DATA_STGHAVEBREAKFAST));
		//npc[3].push_back(new CNPC1(CPoint(700, 388), BitmapPath("RES\\NPC\\test", "test", 1, RGB(255, 255, 255)), "zy", DIALOG_DATA_STGSUBMIT));

		npc[3].push_back(new CNPC1(CPoint(680, 360), BitmapPath("RES\\NPC\\faqai", "faqai", 20, RGB(255, 255, 255)), "faqai", DIALOG_DATA_FAQAI));

		npc[7].push_back(new CNPC1(CPoint(400, 360), BitmapPath("RES\\NPC\\an", "an", 6, RGB(214, 214, 214)), "an", "EasyAndroidHomework"));

		npc[8].push_back(new CNPC3(CPoint(680, 360), BitmapPath("RES\\NPC\\remilia_music", "remilia", 4, RGB(254, 254, 254)), "remilia", "Septette", "Music_Septette"));
		npc[8].push_back(new CNPC3(CPoint(900, 360), BitmapPath("RES\\NPC\\flandre_music", "flandre", 4, RGB(214, 214, 214)), "flandre", "UN_OwenWasHer", "Music_UN"));
		npc[8].push_back(new CNPC3(CPoint(340, 340), BitmapPath("RES\\NPC\\sakuya_music", "sakuya", 6, RGB(214, 214, 214)), "sakuya", "FloweringNight", "Music_FloweringNight"));
		npc[8].push_back(new CNPC3(CPoint(120, 360), BitmapPath("RES\\NPC\\suwako_music", "suwako", 2, RGB(214, 214, 214)), "suwako", "NativeFaith", "Music_NativeFaith"));
	}
	#pragma endregion

	#pragma region - CEventManager -
	CEventManager::CEventManager()
	{
		gameState = NULL;
	}

	CEventManager::~CEventManager()
	{
	}

	void CEventManager::Initialize()
	{
		if (gameState != NULL)
		{
			dialogWithXingting = false;
		}
	}

	

	void CEventManager::SetGameStateRun(CGameStateRun *state)
	{
		if (gameState == NULL)
		{
			gameState = state;
		}
	}
	void CEventManager::trigger()
	{
		if (gameState != NULL)
		{
			#pragma region - local variable -
			int nowMap = gameState->mapManager.GetNowMap();
			int rolePosition = ScreenX(gameState->mapManager.GetX1(), gameState->role.GetX3());
			#pragma endregion

			#pragma region - dialog - tips -
			if (tips)
			{
				CDialogManager::Instance()->Start(Tips);
				tips = false;
			}
			#pragma endregion

			#pragma region - dialog - vs xingting -
			if (!dialogWithXingting && nowMap == BOSS_MAP_XINGTING && gameState->role.IsDead() == false && gameState->bossManager.IsBattle() == false)
			{
				if (rolePosition >= 100)
				{
					CDialogManager::Instance()->Start(DIALOG_DATA_VSXingting1);
					dialogWithXingting = true;
					gameState->bossManager.SetBattle(true);
					gameState->SwitchTimer(gameState->bossManager.targetBoss->GetAliveTimer());
				}
			}
			#pragma endregion
		}
	}
	#pragma endregion

	#pragma region  - CEndManager -
	CEndManager CEndManager::endManager;
	CEndManager::CEndManager()
	{
		endBmp.SetFadeInOut(30, -40);
		Initialize();
	}

	CEndManager::~CEndManager()
	{
		//����map���HaveEnd������ > ��Ū�� �b�g�J
		#pragma region - load -
		fstream haveEnd_open;
		string tempName, nonstr;
		vector<string> endName;
		haveEnd_open.open("RES\\End\\HaveEnd.txt", ios::in);
		while (haveEnd_open >> tempName >> nonstr)
		{
			endName.push_back(tempName);
		}
		haveEnd_open.close();
		#pragma endregion

		#pragma region - write -
		fstream haveEnd_write;
		haveEnd_write.open("RES\\End\\HaveEnd.txt", ios::out);
		for (unsigned int i = 0; i < endName.size(); i++)
		{
			string writeLine = endName[i] + " " + (endmap[endName[i]].IsGetEnd() ? "true" : "false");
			haveEnd_write << writeLine << "\n";
		}
		haveEnd_write.close();
		#pragma endregion

	}

	void CEndManager::Initialize()
	{
		if (isLoadEnd == false)
		{
			LoadEnd();
			isLoadEnd = true;
		}

		nowEnd = nullptr;
		isEnding = false;
		step = 0;
		alpha = 0;
		time_remaining = CTimer(1.0);
		isFadeIn = true; 
		isFadeOut = false;
		isOpenDialog = false;
	}

	void CEndManager::LoadEnd()
	{
		endmap[END_NAME_WINXINGTING] = CEnd(END_NAME_WINXINGTING);
		endmap[END_NAME_LOSEXINGTING] = CEnd(END_NAME_LOSEXINGTING);
		endmap[END_NAME_SALTEDFISH] = CEnd(END_NAME_SALTEDFISH);

		#pragma region - open HaveEnd.txt to Load The End is Get -
		fstream haveEnd;
		haveEnd.open("RES\\End\\HaveEnd.txt", ios::in);
		string lineData;
		map<string, CEnd>::iterator iter;
		while (getline(haveEnd, lineData))
		{
			//lineInfo[0] = name, [1] = have end (true or false)
			vector<string> lineInfo = SplitString(lineData);
			#pragma region - find lineInfo[0] is the key of map -
			iter = endmap.find(lineInfo[0]);
			if (iter != endmap.end()) //map have this key
			{
				iter->second.SetGetEnd(ConvertStringToBoolen(lineInfo[1]));
			}
			#pragma endregion

		}
		haveEnd.close();
		#pragma endregion

	}

	void CEndManager::OnCycle()
	{
		Play();
	}

	bool CEndManager::IsPassEnd(string endName)
	{
		if (!endmap.count(endName))
			return false;
		return endmap[endName].IsGetEnd();
	}

	vector<string> CEndManager::GetAllEndName()
	{
		vector<string> endName;
		for (map<string, CEnd>::iterator iter = endmap.begin(); iter != endmap.end(); iter++)
		{
			endName.push_back(iter->first);
		}
		return endName;
	}

	void CEndManager::Start(string endName)
	{
		nowEnd = &endmap[endName];
		isEnding = true;
		endBmp.SetBmp(nowEnd->GetBmpPath(0));

		step = 0;
		isFadeIn = true;
		isFadeOut = false;
	}
	
	void CEndManager::Play()
	{
		if (nowEnd->GetBmpPath(step) != END_EOF) //���o�����|��m���٤��O�Ÿ��|
		{

			if (isFadeIn)
			{
				endBmp.FadeIn();
				if (endBmp.GetAlpha() >= 255)
				{
					if (!CDialogManager::Instance()->GetDialogState())
					{
						if (nowEnd->GetTxt(step) != END_EOF && isOpenDialog == false) //���`����+��� (�������)
						{
							CDialogManager::Instance()->Start(nowEnd->GetTxt(step));
							isOpenDialog = true;
						}
						else if (nowEnd->GetTxt(step) == END_EOF && isOpenDialog == false) //�Ȧ������A�S����� (���]�p�ɾ�)
						{
							time_remaining.ResetTime(1.5);
							isOpenDialog = true; //! �}��true�� �U�@��thread�|�i�J��U�誺else�íp��
						}
						else
						{
							time_remaining.CountDown(); //��ܵ����� ����@�q�ɶ� �~�����U�@�i
							if (time_remaining.IsTimeOut()) //1.5s �ɶ��� �����A
							{
								isFadeIn = false;
								isFadeOut = true;
								isOpenDialog = false;
								time_remaining.ResetTime(1.0);
							}
						}
					}
				}
			}
			else if (isFadeOut)
			{
				endBmp.FadeOut();
				if (endBmp.GetAlpha() <= 0) //�Ϥ������z�� �����A ���Ϥ�
				{
					step++;
					if (nowEnd->GetBmpPath(step) != END_EOF) //���step �b���@�����b
					{
						endBmp.SetBmp(nowEnd->GetBmpPath(step));
						isFadeIn = true;
						isFadeOut = false;
					}
				}
			}
			
			endBmp.DrawImage();
		}
		else
		{
			nowEnd->SetGetEnd();
			Stop();
		}
	}
	
	void CEndManager::Stop()
	{
		isEnding = false;
	}

	#pragma endregion

	#pragma region - Button Manager -
	CButtonManager::CButtonManager()
	{
		Initialize();
	}

	CButtonManager::~CButtonManager()
	{
		Clear();
	}

	void CButtonManager::Load()
	{
		for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
		{
			(btniter->second)->LoadBitmap();
		}
	}

	void CButtonManager::Initialize()
	{
		for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
		{
			(btniter->second)->Initialize();
		}
	}

	void CButtonManager::Clear()
	{
		for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
		{
			delete btniter->second;
			btniter->second = NULL;
		}
		buttons.clear();
	}

	void CButtonManager::AddButton(CButton* _button)
	{
		if(_button->name != "")
			buttons[_button->name] = _button;
	}

	void CButtonManager::CreateButton(BitmapPath _loadpath, CPoint point, bool _state, bool _needCollision)
	{
		AddButton(new CButton(_loadpath, point, _state, _needCollision));
	}

	void CButtonManager::CreateButton(CButton *button)
	{
		AddButton(button);
	}

	void CButtonManager::ClickButton(string _btnName)
	{
		(buttons[_btnName])->ClickButton();
	}

	bool CButtonManager::IsCollisionMouse(string _btnName)
	{
		return (buttons[_btnName])->IsCollisionMouse(mouse);
	}

	bool CButtonManager::GetState(string _btnName)
	{
		return (buttons[_btnName])->state;
		/*return (buttons[_btnName])->GetState();*/
	}

	void CButtonManager::UpdateState(CPoint _m)
	{
		mouse = _m;
		for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
		{
			(btniter->second)->CollisonMouse(_m);
		}
	}

	void CButtonManager::ShowButton()
	{
		for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
		{
			(btniter->second)->OnShow();
		}
	}

	void CButtonManager::OnCycle()
	{
		for (map<string, CButton*>::iterator btniter = buttons.begin(); btniter != buttons.end(); btniter++)
		{
			(btniter->second)->OnMove();
		}
	}
	#pragma endregion

	#pragma region - CMapEditer -
	CMapEditer::CMapEditer()
	{
		arrow[0] = Arrow("up");
		arrow[1] = Arrow("down");
		arrow[2] = Arrow("left");
		arrow[3] = Arrow("right");
	}

	CMapEditer::~CMapEditer()
	{
		mapObj.clear();
		if (!blockMap.empty())
		{
			StoreMapInformaion();
		}
	}

	void CMapEditer::Initialize()
	{
		nowMap = getFolerFileNumber("RES\\Map\\Information\\");

		#pragma region - blockMap - ReLoad -
		blockMap.clear();
		for (int i = 0; i < nowMap; i++)
		{
			blockMap.push_back(CBlockMap(i));
			blockMap[i].LoadImg();
		}
		selectNowMap = 0;
		selectMapMode = "none";
		#pragma endregion

		#pragma region - Load arrow -
		for (int i = 0; i < 4; i++)
		{
			arrow[i].Initialize();
		}
		SetArrowCanShow();
		#pragma endregion

		LoadReloadMapInformation();
		NewMapInit();
	}

	void CMapEditer::NewMapInit()
	{
		mapObj.clear(); //ImageInfo - block
		bkmap.Initialize();

		printNowMap = &nowMap;
		isPrintNowMap = false;
		saveTxtName = EDITER_PRESET_SAVETXTNAME;
		isMouseDown = false;
		isSaved = false;
		haveBG = false;
		isMapRight = isMapLeft = false;
		selectObj = NULL;
		cameraX = 0;
	}

	void CMapEditer::AddImage(vector<string> path)
	{
		//���� load (load map)
		//����(�I�� / block), �Ϥ����|
		if (path[0] == "background")
		{
			//NewMapInit();
			haveBG = true;
			ImageInfo tempcv;
			tempcv = ImageInfo(path[1], "background");
			background = tempcv;
			background.SetXY(0, 0, 0);
		}
		else if (path[0] == "block")
		{
			ImageInfo tempObj = ImageInfo(path[1], "block");
			tempObj.SetXY(cameraX, tempObj.y);
			mapObj.push_back(tempObj);
		}
		else if (path[0] == "load")
		{
			NewMapInit();
			LoadBlockMap(path[1]);
			saveTxtName = path[1];
			background.SetXY(0, 0, 0);
		}
		else if (path[0] == "upDoor" || path[0] == "downDoor")
		{
			ImageInfo tempObj = ImageInfo(path[1], path[0]);
			tempObj.SetXY(cameraX, tempObj.y);
			mapObj.push_back(tempObj);
		}
	}

	void CMapEditer::SetMapMoveDir(string dir)
	{
		if (dir == "right")
		{
			isMapRight = true;
		}
		else if (dir == "left")
		{
			isMapLeft = true;
		}
		else if (dir == "cleft")
		{
			isMapLeft = false;
		}
		else if (dir == "cright")
		{
			isMapRight = false;
		}
	}

	void CMapEditer::OnSave()
	{
		if (IsInSelectMapMode())
		{
			if (selectMapMode == "up")
			{
				bkmap.upMap = selectNowMap;
				blockMap[selectNowMap].downMap = nowMap;
			}
			else if (selectMapMode == "down")
			{
				bkmap.downMap = selectNowMap;
				blockMap[selectNowMap].upMap = nowMap;
			}
			else if (selectMapMode == "left")
			{
				bkmap.leftMap = selectNowMap;
				blockMap[selectNowMap].rightMap = nowMap;
			}
			else if (selectMapMode == "right")
			{
				bkmap.rightMap = selectNowMap;
				blockMap[selectNowMap].leftMap = nowMap;
			}
			reloadMap[selectNowMap] = true;
			SetSelectMapMode("none");
		}
		else
		{
			bkmap.nowMap = nowMap;
			CreateObjToBkmap();
			if (!isSaved) //�S���x�s�L�a��
			{
				CString saveDir = ".txt";
				string tempSaveName = "map" + std::to_string(nowMap) + ".txt"; //�w��
				CString saveName = tempSaveName.c_str();
				CString saveExt = "txt (*.txt)|*.txt||";
				CFileDialog saveDlg(false, saveDir, saveName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, saveExt);
				int result = saveDlg.DoModal();

				if (result == IDOK)
				{
					saveTxtName = saveDlg.GetFileName();
					isSaved = true;
					nowMapNumber++;
					bkmap.CreateInformation(saveTxtName);
				}
				if (result == IDCANCEL)
				{
					return;
				}
			}
			if (nowMap < (int)reloadMap.size())
			{
				reloadMap[nowMap] = true;
			}
			//bkmap.CreateInformation(saveTxtName);
			
			#pragma region - bkmap write to blockmap -
			if (nowMap < (int)blockMap.size()) //�ק�@�i�a��
			{
				blockMap[nowMap] = bkmap; 
			}
			else //�n�s�W�@�i�a��
			{
				blockMap.push_back(bkmap);
			}
			#pragma endregion
		}
	}

	void CMapEditer::CreateObjToBkmap()
	{
		bkmap.block.clear();
		bkmap.door.clear();
		bkmap.loadPath = background.path;

		#pragma region - Load Map Simple Object -
		for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
		{
			if (mbiter->id == "block")
			{
				bkmap.block.push_back(CBlock(mbiter->path, mbiter->x, mbiter->y));
			}
			else if (mbiter->id == "upDoor")
			{
				bkmap.door.push_back(CDoor(mbiter->path, mbiter->x, mbiter->y, mbiter->id, bkmap.upMap));
			}
			else if (mbiter->id == "downDoor")
			{
				bkmap.door.push_back(CDoor(mbiter->path, mbiter->x, mbiter->y, mbiter->id, bkmap.downMap));
			}
		}
		#pragma endregion

	}

	void CMapEditer::LoadBlockMap(string mapName)
	{
		//char *fff = ConvertCharPointToString(fileName);
		sscanf(mapName.c_str(), "map%d.txt", &nowMap);
		//delete fff;
		bkmap = blockMap[nowMap];
		//bkmap.LoadInformation(mapName);
		isSaved = true;
		background = ImageInfo(bkmap.loadPath, "background"); //load background
		haveBG = true;

		#pragma region - Load block -
		for (vector<CBlock>::iterator mbiter = bkmap.block.begin(); mbiter != bkmap.block.end(); mbiter++)
		{
			ImageInfo tempk = ImageInfo(mbiter->path, "block");
			tempk.SetXY(mbiter->x, mbiter->y, cameraX);
			mapObj.push_back(tempk);
		}
		#pragma endregion

		#pragma region - Load Door -
		for (vector<CDoor>::iterator mbiter = bkmap.door.begin(); mbiter != bkmap.door.end(); mbiter++)
		{
			ImageInfo tempk = ImageInfo(mbiter->path, mbiter->GetType());
			tempk.SetXY(mbiter->x, mbiter->y, cameraX);
			mapObj.push_back(tempk);
		}
		#pragma endregion

	}

	void CMapEditer::StoreMapInformaion()
	{
		for (unsigned int i = 0; i < blockMap.size(); i++)
		{
			blockMap[i].CreateInformation();
		}
		blockMap.clear();
	}

	void CMapEditer::OnMove()
	{
		if (isMapRight)
		{
			if (cameraX >= 0 && (cameraX + SIZE_X) < background.bmp.Width())
			{
				cameraX += MOVE_DISTANCE;
			}
			else
			{
				cameraX = background.bmp.Width() - SIZE_X;
			}
		}
		if (isMapLeft)
		{
			if (cameraX > 0 && cameraX + SIZE_X <= background.bmp.Width())
			{
				cameraX -= MOVE_DISTANCE;
			}
			else
			{
				cameraX = 0;
			}
		}

		if (haveBG)
		{
			background.bmp.SetTopLeft(background.x - cameraX, background.y);
		}
		for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
		{
			mbiter->bmp.SetTopLeft(mbiter->x - cameraX, mbiter->y);
		}
	}

	#pragma region - �S����ING -
	////�S����ING
	//string CMapEditer::WriteSaveInfo(string type, string path, CPoint point)
	//{
	//	string returnStr = type + " " + path + " " + std::to_string(point.x) + " " + std::to_string(point.y) + "\n";
	//	return returnStr;
	//}

	//void CMapEditer::LoadMapInfo(string fileName)
	//{
	//	fstream mapData;
	//	string path = "RES\\Map\\Information\\" + fileName;
	//	mapData.open(path, ios::in);
	//	if (!mapData.is_open()) //�����} �^�h
	//	{
	//		mapData.close();
	//		return;
	//	}

	//	string lineData;
	//	while (mapData >> lineData)
	//	{
	//		if (lineData == "background")
	//		{
	//			haveBG = true;
	//			mapData >> lineData;
	//			string path = lineData;
	//			background = ImageInfo(path, "background"); //LoadPath
	//			string _x, _y;
	//			mapData >> _x >> _y;
	//			background.SetXY(ConvertStringToInt(_x), ConvertStringToInt(_y), 0);
	//		}
	//		else if (lineData == "block")
	//		{
	//			ImageInfo tempk;
	//			mapData >> lineData;
	//			string path = lineData;
	//			tempk = ImageInfo(path, "block"); //LoadPath
	//			string _x, _y;
	//			mapData >> _x >> _y;
	//			tempk.SetXY(ConvertStringToInt(_x), ConvertStringToInt(_y), 0);
	//			block.push_back(tempk);
	//		}
	//	}
	//	mapData.close();
	//}
	#pragma endregion

	void CMapEditer::OnShow()
	{
		if (!IsInSelectMapMode()) //���b��ܦa�Ϫ��Ҧ���
		{
			if (haveBG)
			{
				background.bmp.ShowBitmap();
			}

			for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
			{
				mbiter->bmp.ShowBitmap();
			}
		}
		else //��ܦa�Ϥ�
		{
			if (selectNowMap < (int)blockMap.size())
			{
				blockMap[selectNowMap].backgroundBitmap.ShowBitmap();
			}

			for (int i = 0; i < 4; i++)
			{
				if (arrow[i].CanShow())
				{
					arrow[i].GetBmp()->ShowBitmap();
				}
			}
		}
	}

	void CMapEditer::SelectBlock(CPoint mouse)
	{
		for (vector<ImageInfo>::reverse_iterator mbiter = mapObj.rbegin(); mbiter != mapObj.rend(); mbiter++)
		{
			if (IsPointInRect(mouse, mbiter->bmp.GetRect()))
			{
				selectObj = &(*mbiter);
				SetDPoint_MouseToTopLeft(mouse);
				return;
			}
		}
		selectObj = NULL;
		SetDPoint_MouseToTopLeft();
	}

	void CMapEditer::DragBlock(CPoint mouse)
	{
		if (selectObj == NULL)
			return;

		selectObj->SetXY(mouse.x - dpoint_mouseToTopleft.x, mouse.y - dpoint_mouseToTopleft.y, cameraX);
	}

	void CMapEditer::DeleteBlock()
	{
		if (selectObj != NULL)
		{
			for (vector<ImageInfo>::iterator mbiter = mapObj.begin(); mbiter != mapObj.end(); mbiter++)
			{
				if (selectObj == &(*mbiter))
				{
					mbiter = mapObj.erase(mbiter);
					selectObj = NULL;
					return;
				}
			}
		}
	}

	string CMapEditer::GetNowMap()
	{
		return "now map: " + std::to_string(*printNowMap);
	}

	void CMapEditer::CreateReloadMapInformation()
	{
		fstream reloadData;
		reloadData.open("RES\\Map\\ReloadMapInformation.txt", ios::out);
		
		reloadData << std::to_string(nowMapNumber) + "\n"; //write�ثe���h�ֱi�a�ϣ{
		for (unsigned int i = 0; i < reloadMap.size(); i++)
		{
			if (reloadMap[i])
			{
				reloadData << std::to_string(i) + "\n"; //write ���Ǧa�ϭnreload
			}
		}
		reloadData.close();
	}

	void CMapEditer::LoadReloadMapInformation()
	{
		fstream reloadData;
		reloadData.open("RES\\Map\\ReloadMapInformation.txt");
		if (!reloadData.is_open()) //�}�ҥ���
		{
			nowMapNumber = nowMap;
			reloadMap.clear();
			reloadMap.resize(nowMapNumber, false);
		}
		reloadData.close();
	}

	void CMapEditer::SetSelectMapZero()
	{
		selectNowMap = -1;
		OnSave();
	}

	void CMapEditer::SetSelectMapMode(string _mode)
	{
		if (_mode == "up" || _mode == "down" || _mode == "left" || _mode == "right")
			printNowMap = &selectNowMap;
		else
		{
			selectNowMap = 0;
			printNowMap = &nowMap;
		}

		selectMapMode = _mode;
	}

	void CMapEditer::ClickArrow(CPoint _mouse)
	{
		for (int i = 0; i < 4; i++)
		{
			if (arrow[i].CanShow() && IsPointInRect(_mouse, arrow[i].GetBmp()->GetRect()))
			{
				SwitchMap(arrow[i].GetDir());
			}
		}
	}

	void CMapEditer::SwitchMap(string _dir)
	{
		if (_dir == "up")
		{
			GotoMap(blockMap[selectNowMap].upMap);
		}
		else if (_dir == "down")
		{
			GotoMap(blockMap[selectNowMap].downMap);
		}
		else if (_dir == "left")
		{
			GotoMap(blockMap[selectNowMap].leftMap);
		}
		else if (_dir == "right")
		{
			GotoMap(blockMap[selectNowMap].rightMap);
		}
	}

	void CMapEditer::GotoMap(int mapIndex)
	{
		if (mapIndex != -1)
		{
			selectNowMap = mapIndex;
			SetArrowCanShow();
		}
	}

	void CMapEditer::SetArrowCanShow()
	{
		arrow[0].SetCanShow(blockMap[selectNowMap].upMap >= 0 && blockMap[selectNowMap].upMap < (int)blockMap.size());
		arrow[1].SetCanShow(blockMap[selectNowMap].downMap >= 0 && blockMap[selectNowMap].downMap < (int)blockMap.size());
		arrow[2].SetCanShow(blockMap[selectNowMap].leftMap >= 0 && blockMap[selectNowMap].leftMap < (int)blockMap.size());
		arrow[3].SetCanShow(blockMap[selectNowMap].rightMap >= 0 && blockMap[selectNowMap].rightMap < (int)blockMap.size());
	}
	#pragma endregion
}
