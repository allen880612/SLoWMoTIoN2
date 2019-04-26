////////////////////////////
//定義常數
////////////////////////////
#pragma once
#define MAX_LAYER_NUMBER 10
#define MOVE_DISTANCE 15

#define MAX_MAP_NUMBER 6
#define NOW_MAP 2

#define KEY_SPACE 32
#define KEY_LEFT 37		// keyboard左箭頭
#define KEY_UP 38		// keyboard上箭頭
#define KEY_RIGHT 39	// keyboard右箭頭
#define KEY_DOWN 40		// keyboard下箭頭
#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68
#define KEY_Q 81
#define KEY_Z 90
#define KEY_C 67
#define KEY_B 66


#define GAME_TIME 60
#define AUDIO_NUM 7

#pragma region - Layer -
#define BULLET_LAYER 6
#define BOSS_LAYER 5
#pragma endregion


#pragma region - dialog - margin -
#define MARGIN_DIALOG_AVATAR 16
// 16 + avatar.width()
#define MARGIN_DIALOG_TEXT_X 16	
#define MARGIN_DIALOG_TEXT_Y 8
#pragma endregion

#pragma region - dialog - setting -
#define DIALOG_TEXT_SIZE 16
#define DIALOG_TEXT_COLOR RGB(254, 254, 254)
//MAX TEXT - ENGLISH SIZE (1 CHINESE SIZE = 2 ENGLISH SIZE)
#define DIALOG_MAX_TEXT 30
#define DIALOG_BACKGROUND_COLOR RGB(66, 66, 66)

#pragma endregion

#pragma region - dialog - avatar name -
#define DIALOG_AVATAR_NAME_ROLE "role"
#define DIALOG_AVATAR_NAME_XINGTING "xingting"
#define DIALOG_AVATAR_NAME_LOCK "lock"
#define DIALOG_AVATAR_NAME_QUESTION "question"
#define DIALOG_AVATAR_NAME_STUDENTB "StudendB"
#define DIALOG_AVATAR_NAME_STUDENTG "StudentG"
#pragma endregion


#pragma region - dialog - txt data - 
#define DIALOG_DATA_VSXingting1 "roleVsXingting1"
#define DIALOG_DATA_VSXingting2 "roleVsXingting2"
#define Tips "InitTip"
#define FROG "frog"
#define Music_Deadlock "MyVoiceIsDead"
#define DIALOG_DATA_STGHAVEBREAKFAST "StudentGHaveBreakfast"
#define DIALOG_DATA_MEETSTB "StudentSmoke"
#define DIALOG_DATA_STGSUBMIT "StudentSubmit"
#pragma endregion

#pragma region - dialog - add a text on cycle -
#define DIALOG_ADDTEXT_TIME 0.1
#pragma endregion



#pragma region - boss -
#define BOSS_DEPRATMENT "boss"
#define BOSS_MAP_XINGTING 5
#pragma endregion


#pragma region - boss name -
#define BOSS_XINGTING "xingting"
#pragma endregion

#pragma region - block -
#define BLOCK_DEPRATMENT "block"
#pragma endregion

#pragma region - layer -
#define INTERFACE_LAYER 9
#define BULLET_LAYER 6
#define NPC_LAYER 4
#pragma endregion





#define DEBUG_MODE false