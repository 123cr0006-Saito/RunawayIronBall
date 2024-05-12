//----------------------------------------------------------------------
// @filename RotationCamera.cpp
// @date: 2024/03/13
// @author: saito ko
// @explanation
// �J��������]�����鏈��������N���X
//----------------------------------------------------------------------
#include "RotationCamera.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param distance �J�����ƒ����_�̋���
// @param targetPos �����_�̈ʒu
// @return ����
//----------------------------------------------------------------------
RotationCamera::RotationCamera(float distance,VECTOR targetPos) : CameraBase() {
	_pointDistance.z = -distance;
	_targetPos = targetPos;
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return ����
//----------------------------------------------------------------------
RotationCamera::~RotationCamera() {};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �������Ă��邩
//----------------------------------------------------------------------
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