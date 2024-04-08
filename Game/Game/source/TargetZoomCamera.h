//----------------------------------------------------------------------
// @filename TargetZoomCamera.h
// ��date: 2024/03/09
// ��author: saito ko
// @explanation
// ���̃J�����̈ʒu���w��̈ʒu�Ɉړ����鏈�����s���N���X
//----------------------------------------------------------------------
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

