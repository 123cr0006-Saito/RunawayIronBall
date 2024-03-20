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
	static const int CAMERA_ZOOM_MAX = 3;

	int _reverseX;//�J��������̓��͂ɂ��X���������߂�ϐ�
	int _reverseY;//�J��������̓��͂ɂ��Y���������߂�ϐ�

	bool _IsZoom;
	bool _IsForwardCamera;

	float _startDistance, _endDistance;
	float _startDirY, _endDirY;

	int _cameraDistanceCount; // �J�����̋�����3�i�K�ɕ�����
	float _cameraChangeDistance[CAMERA_ZOOM_MAX];
	int _zoomCount;;
	int _forwardCount;

	VECTOR _gazeShift;//�����_���v���C���[�ɂ����Ƃ��Ɋ�_�������Ȃ̂ō�������Ɉړ������邽�߂̕ϐ�
};