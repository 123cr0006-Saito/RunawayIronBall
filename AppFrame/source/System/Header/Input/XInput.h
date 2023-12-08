#pragma once
#include "dxlib.h"


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
#define XINPUT_BUTTON_LT (10)
#define XINPUT_BUTTON_RT (11)
//#define XINPUT_BUTTON_A		                         (12)   // Aボタン
//#define XINPUT_BUTTON_B		                         (13)   // Bボタン
//#define XINPUT_BUTTON_X		                         (14)   // Xボタン
//#define XINPUT_BUTTON_Y		                         (15)   // Yボタン

#define PAD_BUTTON_MAX		16
const int PAYER_1 = 1;
const int PAYER_2 = 2;
const int PAYER_3 = 3;
const int PAYER_4 = 4;

class XInput
{
public:
	XInput(int number);
	virtual bool Input();

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

	//-----------------------------------------------------

private:
	XINPUT_STATE _input;

	// スティック入力の値を「-1 ~ 1」に変換する
	struct STICK {
		float x, y;
	};
	STICK _lStick;
	STICK _rStick;

// xboxコントローラーのボタン配置
// 最大数

	unsigned char	 _trg[PAD_BUTTON_MAX], _rel[PAD_BUTTON_MAX];

	int pad_num;

};