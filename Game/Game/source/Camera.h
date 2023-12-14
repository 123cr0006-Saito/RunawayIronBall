#pragma once
#include "dxlib.h"
#include "appframe.h"
class Camera
{
public:
	Camera();
	~Camera();

	virtual bool Process(VECTOR pos);

	float GetCamX() { return _cameraDirX; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���
	float GetCamY() { return _cameraDirY; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���


protected:
	XInput* _input;//�R���g���[���[�ł̓��͂��󂯕t���邽�߂�input�N���X

	float _cameraDirX;//�J�����̂w����]�s��Ŏg�p����ϐ�
	float _cameraDirY;//�J�����̂x����]�s��Ŏg�p����ϐ�

	int _reverseX;//�J��������̓��͂ɂ��X���������߂�ϐ�
	int _reverseY;//�J��������̓��͂ɂ��Y���������߂�ϐ�

	VECTOR _pointDistance;//�����_����̋���

	VECTOR _gazeShift;//�����_���v���C���[�ɂ����Ƃ��Ɋ�_�������Ȃ̂ō�������Ɉړ������邽�߂̕ϐ�

};