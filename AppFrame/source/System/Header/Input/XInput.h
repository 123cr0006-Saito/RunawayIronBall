#pragma once
#include "dxlib.h"


//XInput�̃{�^���z�u

//#define XINPUT_BUTTON_DPAD_UP		               (0)	// �f�W�^�������{�^����
//#define XINPUT_BUTTON_DPAD_DOWN		       (1)	// �f�W�^�������{�^����
//#define XINPUT_BUTTON_DPAD_LEFT		           (2)	// �f�W�^�������{�^����
//#define XINPUT_BUTTON_DPAD_RIGHT	           (3)	// �f�W�^�������{�^���E
//#define XINPUT_BUTTON_START		                   (4)	// START�{�^��
//#define XINPUT_BUTTON_BACK		                   (5)	// BACK�{�^��
//#define XINPUT_BUTTON_LEFT_THUMB	           (6)	// ���X�e�B�b�N��������
//#define XINPUT_BUTTON_RIGHT_THUMB	       (7)	// �E�X�e�B�b�N��������
//#define XINPUT_BUTTON_LEFT_SHOULDER	   (8)	// LB�{�^��
//#define XINPUT_BUTTON_RIGHT_SHOULDER   (9)	// RB�{�^��
#define XINPUT_BUTTON_LT (10)
#define XINPUT_BUTTON_RT (11)
//#define XINPUT_BUTTON_A		                         (12)   // A�{�^��
//#define XINPUT_BUTTON_B		                         (13)   // B�{�^��
//#define XINPUT_BUTTON_X		                         (14)   // X�{�^��
//#define XINPUT_BUTTON_Y		                         (15)   // Y�{�^��

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

	//�Q�b�^�[
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

	// �X�e�B�b�N���͂̒l���u-1 ~ 1�v�ɕϊ�����
	struct STICK {
		float x, y;
	};
	STICK _lStick;
	STICK _rStick;

// xbox�R���g���[���[�̃{�^���z�u
// �ő吔

	unsigned char	 _trg[PAD_BUTTON_MAX], _rel[PAD_BUTTON_MAX];

	int pad_num;

};