#pragma once
#include "CameraBase.h"
#include "Camera.h"
class TargetZoomCamera : public CameraBase
{
public:
	TargetZoomCamera();
	TargetZoomCamera(VECTOR pos, int time, Camera* camera);
	~TargetZoomCamera();

	virtual bool UpdateCameraToEvent();
	void SetEventCamera(VECTOR pos, int time);
	void SetReturnCamera(VECTOR pos);
	
protected:
	bool _IsEvent;//�C�x���g�ȂǂŒ����_���v���C���[�ȊO�ɂȂ�Ƃ��Ɏg�p
	bool _IsReturn;//�C�x���g�t���O�������Ă���Ƃ��ɃJ���������Ƃ̍��W�ɖ߂��̂Ɏg�p

	int _moveTime;//�ړ�����
	VECTOR _targetPos, _keepPos;// �^�[�Q�b�g�̍��W�Ƃ��Ƃ��ƃJ�������������ʒu�̍��W
};

