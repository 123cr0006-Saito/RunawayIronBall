//----------------------------------------------------------------------
// @filename Afterglow.cpp
// @date: 2024/12/14
// @author: saito ko
// @explanation
// ���ꂼ��̃J�����N���X�ŋ��ʂŎg�p����ϐ���������N���X
//----------------------------------------------------------------------
#include "CameraBase.h"
const float CameraBase::_cameraMinDistance = -150.0f;
const float CameraBase::_cameraMaxDistance = -2500.0f;
//----------------------------------------------------------------------
// @brief �R���X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
CameraBase::CameraBase(){
	_input = XInput::GetInstance();//�R���X�g���N�^�œ��͗p�̃N���X�̃|�C���^���擾
	_pointDistance = VGet(0, 0, -600);
	_currentTime = 0;
	_cameraDirX = 0.53f;
	_cameraDirY = 0.0f;
};
//----------------------------------------------------------------------
// @brief �f�X�g���N�^
// @return �Ȃ�
//----------------------------------------------------------------------
CameraBase::~CameraBase() {
	_input = nullptr;
};
//----------------------------------------------------------------------
// @brief �J�����̏���
// @return �Ȃ�
//----------------------------------------------------------------------
bool CameraBase::Process() {
	return true;
};