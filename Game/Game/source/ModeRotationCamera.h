//----------------------------------------------------------------------
// @filename ModeRotationCamera.cpp
// ��date: 2024/03/09
// ��author: saito ko
// @explanation
// �X�e�[�W�J�n���̃J�����̉�]�������s���N���X
//----------------------------------------------------------------------
#pragma once
#include "appframe.h"
#include "RotationCamera.h"
#include "ModeFadeComeBack.h"
class ModeRotationCamera : public ModeBase
{
	typedef ModeBase base;
public:
	ModeRotationCamera(int stageNum);
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
protected:
	XInput* _input;// ���̓N���X
	int _handle;// �X�e�[�W�̖��O�̉摜�n���h��
	int _stageNum;// �X�e�[�W�̔ԍ�
	RotationCamera* _camera;// �J�����N���X
	int _currentTime;// ���݂̎���
};

