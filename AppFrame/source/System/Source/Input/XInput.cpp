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
	// �L�[�̓��́A�g���K���́A�����[�X���͂𓾂�
	unsigned char keyold[PAD_BUTTON_MAX];

	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		keyold[i] = _input.Buttons[i];
	}

	// ���͏�Ԃ��擾
	GetJoypadXInputState(pad_num, &_input);

	for (int i = 0; i < PAD_BUTTON_MAX; i++) {
		// �g���K���͂̎擾
		_trg[i] = (_input.Buttons[i] ^ keyold[i]) & _input.Buttons[i];
		// �����[�X���͂̎擾
		_rel[i] = (_input.Buttons[i] ^ keyold[i]) & ~_input.Buttons[i];
	}

	// LT�{�^��
	unsigned char triggerButton = _input.LeftTrigger == 0 ? 0 : 1;
	// �g���K���͂̎擾
	_trg[XINPUT_BUTTON_LT] = (triggerButton ^ keyold[XINPUT_BUTTON_LT]) & triggerButton;
	// �����[�X���͂̎擾
	_rel[XINPUT_BUTTON_LT] = (triggerButton ^ keyold[XINPUT_BUTTON_LT]) & ~triggerButton;

	// RT�{�^��
	triggerButton = _input.RightTrigger == 0 ? 0 : 1;
	// �g���K���͂̎擾
	_trg[XINPUT_BUTTON_RT] = (triggerButton ^ keyold[XINPUT_BUTTON_RT]) & triggerButton;
	// �����[�X���͂̎擾
	_rel[XINPUT_BUTTON_RT] = (triggerButton ^ keyold[XINPUT_BUTTON_RT]) & ~triggerButton;

	// �X�e�B�b�N���͒l��5%�ȉ��Ȃ���͂��J�b�g����i�f�b�h�]�[���ݒ�j
	_input.ThumbLX = abs(_input.ThumbLX) > SHRT_MAX * 0.05f ? _input.ThumbLX : 0;
	_input.ThumbLY = abs(_input.ThumbLY) > SHRT_MAX * 0.05f ? _input.ThumbLY : 0;
	_input.ThumbRX = abs(_input.ThumbRX) > SHRT_MAX * 0.05f ? _input.ThumbRX : 0;
	_input.ThumbRY = abs(_input.ThumbRY) > SHRT_MAX * 0.05f ? _input.ThumbRY : 0;

	// �X�e�B�b�N���͂̒l���u-1 ~ 1�v�ɕϊ�����
	_lStick.x = (float)_input.ThumbLX / (float)SHRT_MAX;
	_lStick.y = (float)_input.ThumbLY / (float)SHRT_MAX;
	_rStick.x = (float)_input.ThumbRX / (float)SHRT_MAX;
	_rStick.y = (float)_input.ThumbRY / (float)SHRT_MAX;

	return true;
};
