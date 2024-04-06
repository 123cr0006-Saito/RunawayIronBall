//----------------------------------------------------------------------
// @filename ModeRotationCamera.cpp
// ＠date: 2024/03/09
// ＠author: saito ko
// @explanation
// ステージ開始時のカメラの回転処理を行うクラス
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
	XInput* _input;
	int _handle;
	int _stageNum;
	RotationCamera* _camera;
	int _currentTime;
};

