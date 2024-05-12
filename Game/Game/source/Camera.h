//----------------------------------------------------------------------
// @filename Camera.h
// @date: 2023/12/14
// @author: saito ko
// @explanation
// �v���C���[��Ǐ]�����{�I�ȃJ����
//----------------------------------------------------------------------
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

	void SetCameraDistance();
	bool ZoomProcess();
	void SetForwardCamera();
	void MoveProcess();//�J�����̈ړ����s���֐�
	
protected:
	static const int CAMERA_ZOOM_MAX = 3;//�J�����̃Y�[���i�K

	int _reverseX;//�J��������̓��͂ɂ��X���������߂�ϐ�
	int _reverseY;//�J��������̓��͂ɂ��Y���������߂�ϐ�

	bool _IsZoom;//�Y�[�������ǂ���
	bool _IsForwardCamera;//�J�����̌�����O���Ɍ����邩�ǂ���

	float _startDistance, _endDistance;//�J�����̋�����ς��邽�߂̕ϐ�
	float _startDirY, _endDirY;//�J�����̌�����ς��邽�߂̕ϐ�

	int _cameraDistanceCount; // �J�����̋�����3�i�K�ɕ�����
	float _cameraChangeDistance[CAMERA_ZOOM_MAX];//�J�����̋�����ς������̋������i�[����ϐ�
	int _zoomCount;;//���Ԗڂ̃J�����̋������̃J�E���g
	int _forwardCount;//�J�����̌�����ς��邽�߂̃C�[�W���O�J�E���g

	VECTOR _gazeShift;//�����_���v���C���[�ɂ����Ƃ��Ɋ�_�������Ȃ̂ō�������Ɉړ������邽�߂̕ϐ�
};