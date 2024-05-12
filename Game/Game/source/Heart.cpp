//----------------------------------------------------------------------
// @filename Heart.cpp
// @date: 2024/03/03
// @author: saito ko
// @explanation
// �v���C���[���G�ꂽ��g�o���񕜂���I�u�W�F�N�g�N���X
//----------------------------------------------------------------------
#include "Heart.h"
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @param pos �ʒu
// @return ����
//----------------------------------------------------------------------
Heart::Heart(VECTOR pos){
	_model = ResourceServer::MV1LoadModel("Heart","res/Item/Cg_Object_Heart/Cg_Object_Heart.mv1");

	_sphere.r = 50;
	//pos�͑�����ōl���Ă��܂�
	_pos = VAdd(pos, VGet(0, _sphere.r * 2, 0));
	_sphere.centerPos = _pos;
	_currentTime = GetNowCount();
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @param ����
// @return ����
//----------------------------------------------------------------------
Heart::~Heart() {};
//----------------------------------------------------------------------
// @brief �X�V����
// @return �������Ă��邩�ǂ���
//----------------------------------------------------------------------
bool Heart::Process() {
	float nowTime = GetNowCount() - _currentTime;
	float oneLapTime = 4 * 1000;
	float direction = 2.0f * DX_PI_F * (nowTime / oneLapTime);
	float Sin = sin(direction);

	_sphere.centerPos = VAdd(_pos,VGet(0,_sphere.r * Sin,0));
	MV1SetPosition(_model, _sphere.centerPos);
	MV1SetRotationXYZ(_model,VGet(0, direction,0));
	return true;
};
//----------------------------------------------------------------------
// @brief �`�揈��
// @return �������Ă��邩�ǂ���
//----------------------------------------------------------------------
bool Heart::Render() {
	MV1DrawModel(_model);
	return true;
};
