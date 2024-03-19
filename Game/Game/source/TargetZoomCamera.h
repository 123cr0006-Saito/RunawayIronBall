#pragma once
#include "CameraBase.h"
#include "Camera.h"
class TargetZoomCamera : public CameraBase
{
public:
	TargetZoomCamera(VECTOR target, VECTOR nextPos, VECTOR keepDir,VECTOR nextDir, int time);
	~TargetZoomCamera();
	virtual bool Process()override;

protected:
	float _moveTime;//�ړ�����
	VECTOR _targetPos, _nextPos, _keepPos;// �^�[�Q�b�g�̍��W�Ƃ��Ƃ��ƃJ�������������ʒu�̍��W
	VECTOR _nextDir, _keepDir; // ���̃J�����̌����Ƃ��Ƃ��Ƃ̃J�����̌���
};

