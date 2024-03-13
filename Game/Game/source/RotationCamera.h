#pragma once
#include "CameraBase.h"
class RotationCamera : 	public CameraBase
{
public:
	RotationCamera(float distance,VECTOR targetPos);
	~RotationCamera();
	bool Process()override;
protected:
	VECTOR _targetPos;
};

