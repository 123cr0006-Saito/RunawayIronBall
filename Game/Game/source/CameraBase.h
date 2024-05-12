//----------------------------------------------------------------------
// @filename Afterglow.cpp
// @date: 2024/12/14
// @author: saito ko
// @explanation
// ���ꂼ��̃J�����N���X�ŋ��ʂŎg�p����ϐ���������N���X
//----------------------------------------------------------------------
#pragma once
#include "dxlib.h"
#include "appframe.h"
class CameraBase
{
public:
	CameraBase();
	~CameraBase();
	virtual bool Process();

    //�J�����̏������s���֐�
	float GetTargetDistance() { return _pointDistance.z; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���
	static float GetMaxLength() { return _cameraMaxDistance - _cameraMinDistance; }

protected:
	static const float _cameraMinDistance;//�J�����̍ŏ�����
	static const float _cameraMaxDistance;//�J�����̍ő勗��
	XInput* _input;//�R���g���[���[�ł̓��͂��󂯕t���邽�߂�input�N���X

	float _cameraDirX;//�J�����̂w����]�s��Ŏg�p����ϐ�
	float _cameraDirY;//�J�����̂x����]�s��Ŏg�p����ϐ�
	XInput::STICK _stick;//�R���g���[���[�̃X�e�B�b�N�̓��͂��󂯕t���邽�߂̕ϐ�

	VECTOR _pointDistance;//�����_����̋���
	int _currentTime;//���݂̎���
};

