////////////////////////////
//定義常數
////////////////////////////
#pragma once
#define MAX_LAYER_NUMBER 10
#define MOVE_DISTANCE 15

#define MAX_MAP_NUMBER 4
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


#define GAME_TIME 10
#define AUDIO_NUM 6

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
#pragma endregion


#pragma region - dialog - mode -
#define RoleVSBoss "roleVsBoss"
#define Tips "InitTip"
#pragma endregion

#pragma region - dialog - add a text on cycle -
#define DIALOG_ADDTEXT_TIME 0.1
#pragma endregion



#pragma region - boss -
#define BOSS_DEPRATMENT "boss"
#pragma endregion


#pragma region - boss name -
#define BOSS_XINGTING "xingting"
#pragma endregion

#pragma region - block -
#define BLOCK_DEPRATMENT "block"
#pragma endregion

#pragma region - layer -
#define BULLET_LAYER 6
#pragma endregion





#define DEBUG_MODE true