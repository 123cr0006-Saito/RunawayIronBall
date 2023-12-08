#include "../../Header/Input/XInput.h"
XInput::XInput(int number) {
	pad_num = number;
	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		_trg[i] = 0;
		_rel[i] = 0;
		_input.Buttons[i] = 0;
	}
};


bool XInput::Input() {
	// キーの入力、トリガ入力、リリース入力を得る
	unsigned char keyold[PAD_BUTTON_MAX];

	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		keyold[i] = _input.Buttons[i];
	}

	// 入力状態を取得
	GetJoypadXInputState(pad_num, &_input);

	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		// トリガ入力の取得
		_trg[i] = (_input.Buttons[i] ^ keyold[i]) & _input.Buttons[i];
		// リリース入力の取得
		_rel[i] = (_input.Buttons[i] ^ keyold[i]) & ~_input.Buttons[i];
	}

	// LTボタン
	unsigned char triggerButton = _input.LeftTrigger == 0 ? 0 : 1;
	// トリガ入力の取得
	_trg[XINPUT_BUTTON_LT] = (triggerButton ^ keyold[XINPUT_BUTTON_LT]) & triggerButton;
	// リリース入力の取得
	_rel[XINPUT_BUTTON_LT] = (triggerButton ^ keyold[XINPUT_BUTTON_LT]) & ~triggerButton;

	// RTボタン
	triggerButton = _input.RightTrigger == 0 ? 0 : 1;
	// トリガ入力の取得
	_trg[XINPUT_BUTTON_RT] = (triggerButton ^ keyold[XINPUT_BUTTON_RT]) & triggerButton;
	// リリース入力の取得
	_rel[XINPUT_BUTTON_RT] = (triggerButton ^ keyold[XINPUT_BUTTON_RT]) & ~triggerButton;

	// スティック入力値が5%以下なら入力をカットする（デッドゾーン設定）
	_input.ThumbLX = abs(_input.ThumbLX) > SHRT_MAX * 0.05f ? _input.ThumbLX : 0;
	_input.ThumbLY = abs(_input.ThumbLY) > SHRT_MAX * 0.05f ? _input.ThumbLY : 0;
	_input.ThumbRX = abs(_input.ThumbRX) > SHRT_MAX * 0.05f ? _input.ThumbRX : 0;
	_input.ThumbRY = abs(_input.ThumbRY) > SHRT_MAX * 0.05f ? _input.ThumbRY : 0;

	// スティック入力の値を「-1 ~ 1」に変換する
	_lStick.x = (float)_input.ThumbLX / (float)SHRT_MAX;
	_lStick.y = (float)_input.ThumbLY / (float)SHRT_MAX;
	_rStick.x = (float)_input.ThumbRX / (float)SHRT_MAX;
	_rStick.y = (float)_input.ThumbRY / (float)SHRT_MAX;

	return true;
};
