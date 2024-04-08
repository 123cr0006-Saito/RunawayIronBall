#include "../../Header/Input/XInput.h"
#include "../../Header/Function/mymath.h"

XInput* XInput::_instance = NULL;

XInput::XInput(int number) {
	_instance = this;

	pad_num = number;
	for (int i = 0; i < DXINPUT_BUTTON_MAX; i++) {
		_trg[i] = 0;
		_rel[i] = 0;
		_input.Buttons[i] = 0;
	}
};


bool XInput::Input() {
	// キーの入力、トリガ入力、リリース入力を得る
	unsigned char keyold[PAD_BUTTON_MAX];

	for (int i = 0; i < DXINPUT_BUTTON_MAX; i++) {
		keyold[i] = _input.Buttons[i];
	}
	for (int i = 0; i < 4; i++) {
		keyold[DXINPUT_BUTTON_MAX + i] = _stickInput[i];
	}

	// 入力状態を取得
	GetJoypadXInputState(pad_num, &_input);

	for (int i = 0; i < DXINPUT_BUTTON_MAX; i++) {
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

	_adjustedLStick.x = (float)_input.ThumbLX / (float)SHRT_MAX;
	_adjustedLStick.y = (float)_input.ThumbLY / (float)SHRT_MAX;
	_adjustedRStick.x = (float)_input.ThumbRX / (float)SHRT_MAX;
	_adjustedRStick.y = (float)_input.ThumbRY / (float)SHRT_MAX;

	// 誤差によって範囲を超えた場合にクランプをする
	_adjustedLStick.x = Math::Clamp(-1.0f, 1.0f, _adjustedLStick.x);
	_adjustedLStick.y = Math::Clamp(-1.0f, 1.0f, _adjustedLStick.y);
	_adjustedRStick.x = Math::Clamp(-1.0f, 1.0f, _adjustedRStick.x);
	_adjustedRStick.y = Math::Clamp(-1.0f, 1.0f, _adjustedRStick.y);


	// スティック入力の値を「-1.0f ~ 1.0f」に変換する
	// スティック入力の取得
	// 上　下　左　右
	short stickParam[4] = { _input.ThumbLY, _input.ThumbLX,-_input.ThumbLY,  -_input.ThumbLX };
	short ParamToZero[2] = { _input.ThumbLY,_input.ThumbLX };
	for (int i = 0; i < 4; i++) {
		_stickInput[i] = std::signbit(static_cast<float>(stickParam[i])) ? 0 : 1;
		_stickInput[i] = (ParamToZero[i % 2] != 0 ? 1 : 0) & _stickInput[i];
		// トリガ入力の取得
		_trg[DXINPUT_BUTTON_MAX + i] = (_stickInput[i] ^ keyold[DXINPUT_BUTTON_MAX + i]) & _stickInput[i];
		// リリース入力の取得
		_rel[DXINPUT_BUTTON_MAX + i] = (_stickInput[i] ^ keyold[DXINPUT_BUTTON_MAX + i]) & ~_stickInput[i];
	}

	return true;
};
