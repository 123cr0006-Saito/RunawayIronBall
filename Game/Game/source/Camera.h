#pragma once

#include "CameraBase.h"
class Camera : public CameraBase
{
public:
	Camera(VECTOR InitPos);
	~Camera();

	virtual bool Process(VECTOR pos, int map);

	float GetCamX() { return _cameraDirX; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���
	float GetCamY() { return _cameraDirY; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���

	
protected:

	int _reverseX;//�J��������̓��͂ɂ��X���������߂�ϐ�
	int _reverseY;//�J��������̓��͂ɂ��Y���������߂�ϐ�

	VECTOR _gazeShift;//�����_���v���C���[�ɂ����Ƃ��Ɋ�_�������Ȃ̂ō�������Ɉړ������邽�߂̕ϐ�
};