#include "RotationCamera.h"
RotationCamera::RotationCamera(float distance,VECTOR targetPos) : CameraBase() {
	_pointDistance.z = -distance;
	_targetPos = targetPos;
};

RotationCamera::~RotationCamera() {

};

bool RotationCamera::Process() {

	float _cameraDirYAdd = 0.01f;
	_cameraDirY += _cameraDirYAdd;

	//�J�����̈ʒu���v�Z
	MATRIX origin = MGetIdent();
	MATRIX MatrixX = MGetRotX(_cameraDirX);
	MATRIX MatrixY = MGetRotY(_cameraDirY);

	//�s��̊|���Z
	origin = MMult(origin, MatrixX);
	origin = MMult(origin, MatrixY);

	//�����_����̋����ɍs���ϊ�����
	VECTOR Vecter = VTransform(_pointDistance, origin);

	//�����_�̈ʒu�Ɉړ�
	VECTOR VecAdd = VAdd(Vecter, _targetPos);

	//�J�����̃Z�b�g
	SetCameraPositionAndTarget_UpVecY(VecAdd, _targetPos);
	return true;
};