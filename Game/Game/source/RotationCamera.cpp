#include "RotationCamera.h"
RotationCamera::RotationCamera(int time) : CameraBase() {
	_pointDistance.z = -2000;
	_endTime = time;
};

RotationCamera::~RotationCamera() {

};

bool RotationCamera::Process() {

	float _cameraDirYAdd = 0.04f;
	_cameraDirY += _cameraDirYAdd;

	//�J�����̈ʒu���v�Z
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	//�����Ƃ��ăv���Z�X���œ���̂͂߂�ǂ��������̂�
	//�ł���΃v���C���[�N���X������������Ă������ł�
	VECTOR target = VGet(0,500,0);

	//�s��̊|���Z
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//�����_����̋����ɍs���ϊ�����
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//�����_�̈ʒu�Ɉړ�
	VECTOR VecAdd = VAdd(Vecter, target);

	//�J�����̃Z�b�g
	SetCameraPositionAndTarget_UpVecY(VecAdd, target);

	if (GetNowCount() - _currentTime >= _endTime) {
	
	}
};