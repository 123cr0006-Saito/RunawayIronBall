#pragma once
#include "CameraBase.h"
class RotationCamera : 	public CameraBase
{
public:
	RotationCamera(int time);
	~RotationCamera();
	bool Process()override;
protected:
	int _endTime;
};

