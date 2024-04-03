//----------------------------------------------------------------------
// @filename Afterglow.cpp
// ��date: 2024/12/14
// ��author: saito ko
// @explanation
// ���ꂼ��̃J�����N���X�ŋ��ʂŎg�p����ϐ���������N���X
//----------------------------------------------------------------------
#include "CameraBase.h"
const float CameraBase::_cameraMinDistance = -150.0f;
const float CameraBase::_cameraMaxDistance = -2500.0f;

CameraBase::CameraBase(){
	_input = XInput::GetInstance();//�R���X�g���N�^�œ��͗p�̃N���X�̃|�C���^���擾
	_pointDistance = VGet(0, 0, -600);
	_currentTime = 0;
	_cameraDirX = 0.53f;
	_cameraDirY = 0.0f;
};

CameraBase::~CameraBase() {
	_input = nullptr;
};

bool CameraBase::Process() {
	return true;
};