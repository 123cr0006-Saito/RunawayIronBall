#pragma once
#include "dxlib.h"
#include "../Function/mymath.h"
#include <cmath>

//XInputのボタン配置

//#define XINPUT_BUTTON_DPAD_UP		               (0)	// デジタル方向ボタン上
//#define XINPUT_BUTTON_DPAD_DOWN		       (1)	// デジタル方向ボタン下
//#define XINPUT_BUTTON_DPAD_LEFT		           (2)	// デジタル方向ボタン左
//#define XINPUT_BUTTON_DPAD_RIGHT	           (3)	// デジタル方向ボタン右
//#define XINPUT_BUTTON_START		                   (4)	// STARTボタン
//#define XINPUT_BUTTON_BACK		                   (5)	// BACKボタン
//#define XINPUT_BUTTON_LEFT_THUMB	           (6)	// 左スティック押し込み
//#define XINPUT_BUTTON_RIGHT_THUMB	       (7)	// 右スティック押し込み
//#define XINPUT_BUTTON_LEFT_SHOULDER	   (8)	// LBボタン
//#define XINPUT_BUTTON_RIGHT_SHOULDER   (9)	// RBボタン
#define XINPUT_BUTTON_LT (10)                          // 左トリガ
#define XINPUT_BUTTON_RT (11)                         // 右トリガ 
//#define XINPUT_BUTTON_A		                         (12)   // Aボタン
//#define XINPUT_BUTTON_B		                         (13)   // Bボタン
//#define XINPUT_BUTTON_X		                         (14)   // Xボタン
//#define XINPUT_BUTTON_Y		                         (15)   // Yボタン
#define XINPUT_BUTTON_STICK_UP (16)             //上スティック
#define XINPUT_BUTTON_STICK_LEFT (17)         //左スティック
#define XINPUT_BUTTON_STICK_DOWN (18)      //下スティック
#define XINPUT_BUTTON_STICK_RIGHT (19)      //右スティック

#define DXINPUT_BUTTON_MAX		16
#define PAD_BUTTON_MAX		20
const int PLAYER_1 = 1;
const int PLAYER_2 = 2;
const int PLAYER_3 = 3;
const int PLAYER_4 = 4;

class XInput
{
public:
	// スティック入力値を「-1 ~ 1」に変換して保持する
	struct STICK {
		float x, y;
	};

	XInput(int number);
	virtual bool Input();

	static XInput* _instance;
	static XInput* GetInstance() { return (XInput*)_instance; }

	//ゲッター
	//-------------------------------------------------

	virtual unsigned char GetKey(int index) { return _input.Buttons[index]; }
	virtual unsigned char GetTrg(int index) { return _trg[index]; }
	virtual unsigned char GetRel(int index) { return _rel[index]; }

	short GetRx() { return _input.ThumbRX; }
	short GetRy() { return _input.ThumbRY; }
	short GetLx() { return _input.ThumbLX; }
	short GetLy() { return _input.ThumbLY; }
	unsigned char GetLTrg() { return _input.LeftTrigger; }
	unsigned char GetRTrg() { return _input.RightTrigger; }




	STICK GetAdjustedStick_L() { return _adjustedLStick; }
	STICK GetAdjustedStick_R() { return _adjustedRStick; }

	//-----------------------------------------------------

private:
	XINPUT_STATE _input;

	unsigned char Buttons[PAD_BUTTON_MAX];


	STICK _adjustedLStick;
	STICK _adjustedRStick;

// xboxコントローラーのボタン配置
// 最大数

	unsigned char	 _trg[PAD_BUTTON_MAX], _rel[PAD_BUTTON_MAX];

	int pad_num;

	unsigned char _stickInput[4];

};