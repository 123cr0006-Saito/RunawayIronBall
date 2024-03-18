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
	int _handle;
	int _stageNum;
	RotationCamera* _camera;
	int _currentTime;
};

