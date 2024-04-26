//----------------------------------------------------------------------
// @filename RotationCamera.h
// @date: 2024/03/13
// @author: saito ko
// @explanation
// カメラを回転させる処理をするクラス
//----------------------------------------------------------------------
#pragma once
#include "CameraBase.h"
class RotationCamera : 	public CameraBase
{
public:
	RotationCamera(float distance,VECTOR targetPos);
	~RotationCamera();
	bool Process()override;
protected:
	VECTOR _targetPos;// カメラのターゲット座標
};

