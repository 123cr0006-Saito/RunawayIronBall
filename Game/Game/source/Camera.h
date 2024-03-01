#pragma once
#include "dxlib.h"
#include "appframe.h"
class Camera
{
public:
	Camera(VECTOR InitPos);
	~Camera();

	virtual bool Process(VECTOR pos, int map);
	virtual bool UpdateCameraToMatrix(VECTOR pos, int map);

	virtual bool UpdateCameraToEvent();
	void SetEventCamera(VECTOR pos, int time);
	void SetReturnCamera(VECTOR pos);

	float GetCamX() { return _cameraDirX; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���
	float GetCamY() { return _cameraDirY; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���
	float GetTargetDistance() { return _pointDistance.z; }//�v���C���[�ȂǂŎg����������Ȃ����߃Q�b�^�[�Ƃ��č���Ă���
	float GetMaxLength() { return _cameraMaxDistance - _cameraMinDistance; }


protected:

	const float _cameraMinDistance = -150.0f;//�J�����̍ŏ�����
    const float _cameraMaxDistance = -2500.0f;//�J�����̍ő勗��
	XInput* _input;//�R���g���[���[�ł̓��͂��󂯕t���邽�߂�input�N���X

	float _cameraDirX;//�J�����̂w����]�s��Ŏg�p����ϐ�
	float _cameraDirY;//�J�����̂x����]�s��Ŏg�p����ϐ�

	int _reverseX;//�J��������̓��͂ɂ��X���������߂�ϐ�
	int _reverseY;//�J��������̓��͂ɂ��Y���������߂�ϐ�

	VECTOR _pointDistance;//�����_����̋���

	VECTOR _gazeShift;//�����_���v���C���[�ɂ����Ƃ��Ɋ�_�������Ȃ̂ō�������Ɉړ������邽�߂̕ϐ�

	bool _IsEvent;//�C�x���g�ȂǂŒ����_���v���C���[�ȊO�ɂȂ�Ƃ��Ɏg�p
	bool _IsReturn;//�C�x���g�t���O�������Ă���Ƃ��ɃJ���������Ƃ̍��W�ɖ߂��̂Ɏg�p
	int _currentTime;//���݂̎���
	int _moveTime;//�ړ�����
	VECTOR _targetPos, _keepPos;// �^�[�Q�b�g�̍��W�Ƃ��Ƃ��ƃJ�������������ʒu�̍��W
};