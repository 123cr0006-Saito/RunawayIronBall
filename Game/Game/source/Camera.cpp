//----------------------------------------------------------------------
// @filename Camera.cpp
// ��date: 2023/12/14
// ��author: saito ko
// @explanation
// �v���C���[��Ǐ]�����{�I�ȃJ����
//----------------------------------------------------------------------
#include "Camera.h"
#include "Player.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param InitPos: �J�����̏����n�_
// @return �Ȃ�
//----------------------------------------------------------------------
Camera::Camera(VECTOR InitPos) : CameraBase() {

	SetCameraNearFar(20.0f, 30000.0f);
	
	_gazeShift = VGet(0, 120, 0);
	_reverseX = -1;
	_reverseY = 1;

	_cameraDistanceCount = 2;
	_startDistance = 0.0f;
	_endDistance = 0.0f;
	_zoomCount = 0;
	_IsZoom = false;

	float cameraChangeDistance[CAMERA_ZOOM_MAX] = { -400.0f, -800.0f, -1200.0f };

	for (int i = 0; i < CAMERA_ZOOM_MAX; i++) {
		_cameraChangeDistance[i] = cameraChangeDistance[i];
	}

	_pointDistance.z =  _cameraChangeDistance[_cameraDistanceCount];

	//�J�����̈ʒu���v�Z
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	VECTOR playerPos = InitPos;

	//�s��̊|���Z
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//�����_����̋����ɍs���ϊ�����
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//�����_�̈ʒu�Ɉړ�
	VECTOR VecAdd = VAdd(Vecter, playerPos);
	float shiftY = 0;


	VECTOR target = VAdd(playerPos, VAdd(_gazeShift, VGet(0, shiftY, 0)));

	SetCameraPositionAndTarget_UpVecY(VecAdd, target);
};
//----------------------------------------------------------------------
// @brief �f�X�g���N
// @return �Ȃ�
//----------------------------------------------------------------------
Camera::~Camera() {
	_input = nullptr;
};
//----------------------------------------------------------------------
// @brief �J�����̏���
// @param pos: �v���C���[�̈ʒu
// @param map: �}�b�v�̃n���h��
// @return �����������ǂ���
//----------------------------------------------------------------------
bool Camera::Process(VECTOR pos, int map) {
	// �O�t���[����X����]�̒l��ۑ�
	float oldDirX = _cameraDirX;
	// �X�e�B�b�N�̕������擾
	_stick = _input->GetAdjustedStick_L();
	//���͂��瓾���l�ňړ��l��Ԃ��֐�
	auto move_speed_process = [](float pos, float pos_max, float max_speed) {return pos * max_speed / pos_max; };

	//32768��short�^�̍ő�l �ړ����x�̍ő��0.02
	if (!_IsForwardCamera) {
	    if (_input->GetRx() != 0) {
	    	_cameraDirY += move_speed_process(_input->GetRx(), 32768, 0.02) * _reverseY;
	    }
	    if (_input->GetRy() != 0) {
	    	_cameraDirX += move_speed_process(_input->GetRy(), 32768, 0.02) * _reverseX;
	    }
	}

	//RB���͂ŃJ�����̋�����ύX
	if (_input->GetTrg(XINPUT_BUTTON_RIGHT_SHOULDER)) {
		SetCameraDistance();
	}

	//�}��1���������O�x�ɕϊ�
	if (abs(Math::RadToDeg(_cameraDirX)) > 360) {
		_cameraDirX = 0;
	}
	if (abs(Math::RadToDeg(_cameraDirY)) > 360) {
		_cameraDirY = 0;
	}

	// ��]�l�̏��
	if (_cameraDirX >= 1.39491415f) {
		_cameraDirX = 1.39491415f;
	}
	else if (_cameraDirX <= -1.39491415f) {
		_cameraDirX = -1.39491415f;
	}

	if (_input->GetTrg(XINPUT_BUTTON_LEFT_SHOULDER)) {
		SetForwardCamera();
	}

	// ZoomFlag�������Ă����zoom��process����
	ZoomProcess();
	MoveProcess();

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

		_cameraDirX = oldDirX;
	}
	MV1CollResultPolyDimTerminate(HitPolyDim);

	//�J�����̃Z�b�g
	SetCameraPositionAndTarget_UpVecY(VecAdd, target);
	return true;
};
//----------------------------------------------------------------------
// @brief �J�����̋�����ύX����
// @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetCameraDistance() {
	_cameraDistanceCount++;
	_cameraDistanceCount = (_cameraDistanceCount + CAMERA_ZOOM_MAX) % CAMERA_ZOOM_MAX;
	_startDistance = _pointDistance.z;
	_endDistance = _cameraChangeDistance[_cameraDistanceCount];
	_zoomCount = 0;
	_IsZoom = true;
	_currentTime = GetNowCount();
};
//----------------------------------------------------------------------
// @brief �Y�[���̏���
// @return �����������ǂ���
//----------------------------------------------------------------------
bool Camera::ZoomProcess() {
	if (_IsZoom) {
		float moveTime = 5.0f ;// 5�t���[���ňړ�
		_zoomCount++;
		// �ړ�
		_pointDistance.z = Easing::InQuad(_zoomCount,_startDistance,_endDistance, moveTime);
		// �I��
		if (_zoomCount >= 5) {
			_IsZoom = false;
		}
	}
	return true;
};
//----------------------------------------------------------------------
// @brief �J�����̌������X�e�B�b�N�̕����ɕύX����
// @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetForwardCamera() {
	VECTOR dir = VGet(_stick.x, 0, _stick.y);
	if (!_IsForwardCamera && VSquareSize(dir) != 0) {
		_IsForwardCamera = true;
		_forwardCount = 0;
		_startDirY = _cameraDirY;
		dir = VTransform(dir, MGetRotY(_cameraDirY));
		dir = VNorm(dir);
		_endDirY = atan2(dir.x, dir.z);
	}
};
//----------------------------------------------------------------------
// @brief �J�����̌������X�e�B�b�N�̕����ɕύX����
// @return �Ȃ�
//----------------------------------------------------------------------
void Camera::MoveProcess(){
	if (_IsForwardCamera) {
		float moveTime = 5.0f;// 5�t���[���ňړ�
		_forwardCount++;
		// �ړ�
		_cameraDirY = Easing::InQuad(_forwardCount, _startDirY, _endDirY, moveTime);
		// �I��
		if (_forwardCount >= 5) {
			_IsForwardCamera = false;
		}
	}
};