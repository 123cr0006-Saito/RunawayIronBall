#include "Camera.h"
Camera::Camera(XInput* input) {
	this->_input = input;//�R���X�g���N�^�œ��͗p�̃N���X�̃|�C���^���擾

	_cameraDirX = 0.0f;
	_cameraDirY = 0.0f;
	_pointDistance = VGet(0, 300, -400);
	_gazeShift = VGet(0, 200, 0);
};

Camera::~Camera() {
	_input = nullptr;
};

bool Camera::Process(VECTOR pos) {

	//���͂��瓾���l�ňړ��l��Ԃ��֐�
	auto move_speed_process = [](float pos, float pos_max, float max_speed) {return pos * max_speed / pos_max; };

	//32768��short�^�̍ő�l �ړ����x�̍ő��0.02
	if (_input->GetRx() != 0) {
		_cameraDirY += move_speed_process(_input->GetRx(), 32768, 0.02);
	}
	if (_input->GetRy() != 0) {
		_cameraDirX += move_speed_process(_input->GetRy(), 32768, 0.02);
	}

	//�f�o�b�N�p
	// �\���L�[�Ŕ������ł���

	//if (input->GetKey(XINPUT_BUTTON_DPAD_DOWN)) {
	//	_cameraDirY -= 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_UP)) {
	//	_cameraDirY += 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_LEFT)) {
	//	camera_dir_x -= 0.02;
	//}
	//if (input->GetKey(XINPUT_BUTTON_DPAD_RIGHT)) {
	//	_cameraDirX += 0.02;
	//}

	//�g���K���͂ŃJ�����̋�����ύX
	//�J�����������Ɉړ�
	if (_input->GetRTrg() > 25) {
		if (_pointDistance.z > -800) {
			_pointDistance.z -= _input->GetRTrg() / 25;
		}
	}
	//�J�������߂��Ɉړ�
	if (_input->GetLTrg() > 25) {
		if (_pointDistance.z < -200) {
			_pointDistance.z += _input->GetLTrg() / 25;
		}
	}

	//�}��1���������O�x�ɕϊ�
	if (abs(Math::RadToDeg(_cameraDirX)) > 360) {
		_cameraDirX = 0;
	}
	if (abs(Math::RadToDeg(_cameraDirY)) > 360) {
		_cameraDirY = 0;
	}

	//�J�����̈ʒu���v�Z
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	//�����Ƃ��ăv���Z�X���œ���̂͂߂�ǂ��������̂�
	//�ł���΃v���C���[�N���X������������Ă������ł�
	VECTOR playerPos = pos;

	//�s��̊|���Z
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//�����_����̋����ɍs���ϊ�����
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//�����_�̈ʒu�Ɉړ�
	VECTOR VecAdd = VAdd(Vecter, playerPos);
	//�J�����̃Z�b�g
	SetCameraPositionAndTarget_UpVecY(VecAdd, VAdd(playerPos, _gazeShift));

	return true;
};