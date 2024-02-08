#include "Camera.h"
Camera::Camera() {
	this->_input = XInput::GetInstance();//�R���X�g���N�^�œ��͗p�̃N���X�̃|�C���^���擾

	_cameraDirX = 0.53f;
	_cameraDirY = 0.0f;
	//_pointDistance = VGet(0, 0, -2360);
	_pointDistance = VGet(0, 0, -600);
	_gazeShift = VGet(0, 80, 0);
	_reverseX = -1;
	_reverseY = 1;
};

Camera::~Camera() {
	_input = nullptr;
};

bool Camera::Process(VECTOR pos, int map) {

	if (_IsEvent) {
		UpdateCameraToEvent();
	}
	else {
		UpdateCameraToMatrix(pos, map);
	}
	return true;
};

bool Camera::UpdateCameraToMatrix(VECTOR pos, int map) {
	//���͂��瓾���l�ňړ��l��Ԃ��֐�
	auto move_speed_process = [](float pos, float pos_max, float max_speed) {return pos * max_speed / pos_max; };

	//32768��short�^�̍ő�l �ړ����x�̍ő��0.02
	if (_input->GetRx() != 0) {
		_cameraDirY += move_speed_process(_input->GetRx(), 32768, 0.02) * _reverseY;
	}
	if (_input->GetRy() != 0) {
		_cameraDirX += move_speed_process(_input->GetRy(), 32768, 0.02) * _reverseX;
	}
	//�g���K���͂ŃJ�����̋�����ύX
	//�J�����������Ɉړ�
	if (_input->GetRTrg() > 25) {
		if (_pointDistance.z > -2500) {
			_pointDistance.z -= _input->GetRTrg() / 25;
		}
	}
	//�J�������߂��Ɉړ�
	if (_input->GetLTrg() > 25) {
		if (_pointDistance.z < -150) {
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

	if (_cameraDirX >= 1.39491415f) {
		_cameraDirX = 1.39491415f;
	}
	else if (_cameraDirX <= -1.39491415f) {
		_cameraDirX = -1.39491415f;
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
	float shiftY = 0;


	VECTOR target = VAdd(playerPos, VAdd(_gazeShift, VGet(0, shiftY, 0)));


	MV1_COLL_RESULT_POLY_DIM HitPolyDim;
	float cameraR = 75.0f;
	HitPolyDim = MV1CollCheck_Capsule(map, -1, target, VecAdd, cameraR);
	if (HitPolyDim.HitNum >= 1) {
		VECTOR dirVec = VSub(target, VecAdd);
		//�|���S���̈ʒu�ɐݒ�
		VecAdd.y = VScale(VAdd(VAdd(HitPolyDim.Dim[0].Position[0], HitPolyDim.Dim[0].Position[1]), HitPolyDim.Dim[0].Position[1]), 0.3333333).y;
		//�J�v�Z���̔��a�ł���T�O��y�������Ɉړ�
		VecAdd.y += cameraR;
	}

	//�J�����̃Z�b�g
	SetCameraPositionAndTarget_UpVecY(VecAdd, target);
	return true;
};

bool Camera::UpdateCameraToEvent() {
	int nowTime = GetNowCount() - _currentTime;
	VECTOR playerPos = _targetPos;

	//�s��̊|���Z
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//�����_����̋����ɍs���ϊ�����
	VECTOR Vecter = VTransform(_pointDistance, origin);

	Vecter = VAdd(Vecter, _targetPos);

	//�����_�̈ʒu�Ɉړ�
	VECTOR nowPos;
	if (nowTime > _moveTime) {
		nowTime = _moveTime;
		if (_IsReturn) {
			_IsReturn = _IsEvent = false;
		}
	}
	nowPos.x = Easing::Linear(nowTime, _keepPos.x, Vecter.x, _moveTime);
	nowPos.y = Easing::Linear(nowTime, _keepPos.y, Vecter.y, _moveTime);
	nowPos.z = Easing::Linear(nowTime, _keepPos.z, Vecter.z, _moveTime);

	SetCameraPositionAndTarget_UpVecY(nowPos, _targetPos);
	return true;
};

void Camera::SetEventCamera(VECTOR pos, int time) {
	_targetPos = pos;
	_moveTime = time;
	_keepPos = GetCameraPosition();
	_currentTime = GetNowCount();
	_IsEvent = true;
};

void Camera::SetReturnCamera(VECTOR pos) {
	if (_IsEvent) {
		_IsReturn = true;
		_currentTime = GetNowCount();
		_targetPos = pos;
		_keepPos = GetCameraPosition();
	}
}